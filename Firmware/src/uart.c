#include "uart.h"

static int uart_initialized = 0;

QueueHandle_t user_commands;
QueueHandle_t uart_queue;
SemaphoreHandle_t uart_peripheral;
SemaphoreHandle_t uart_receive;
SemaphoreHandle_t uart_send_atomic;

void EUSCIA0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR( uart_receive, &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void Uart_Claim_Atomic() {
    xSemaphoreTake( uart_send_atomic, portMAX_DELAY );
}

void Uart_Claim_Release() {
    xSemaphoreGive( uart_send_atomic );
}

void Uart_Send( char * data, uint8_t len ) {
    xSemaphoreTake( uart_send_atomic, portMAX_DELAY );

    struct Uart_Data uart_data;

    uint8_t count = 0;
    uint8_t current_len = len;
    while(current_len > 0) {
        if (count == 10) {
            uart_data.len = count;
            xQueueSend( uart_queue, &uart_data, portMAX_DELAY );
            count = 0;
        } else {
            uart_data.data[count] = data[len - current_len];
            count++;
            current_len--;
        }
    }

    if( count > 0 ) {
        uart_data.len = count;
        xQueueSend( uart_queue, &uart_data, portMAX_DELAY );
    }

    xSemaphoreGive( uart_send_atomic );
}

void Uart_Send_Atomic( char * data, uint8_t len ) {
    struct Uart_Data uart_data;

    uint8_t count = 0;
    uint8_t current_len = len;
    while(current_len > 0) {
        if (count == 10) {
            uart_data.len = count;
            xQueueSend( uart_queue, &uart_data, portMAX_DELAY );
            count = 0;
        } else {
            uart_data.data[count] = data[len - current_len];
            count++;
            current_len--;
        }
    }

    if( count > 0 ) {
        uart_data.len = count;
        xQueueSend( uart_queue, &uart_data, portMAX_DELAY );
    }
}

char Read_Char(void) {
    while(!(EUSCI_A0->IFG & 1));
    return (char) EUSCI_A0->RXBUF;
}

int Write_Char(char c) {
    EUSCI_A0->TXBUF = (uint8_t) c;
    while(!(EUSCI_A0->IFG & ( 1 << 1 ) ) );
    return 0;
}

void UART_Init(void) {
    CS->KEY = CS_KEY_VAL;
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK));
    CS->CTL1 |= (CS_CTL1_SELS__DCOCLK);
    CS->KEY = 0;

    P1->DIR &= ~(( 1 << 2 ) | ( 1 << 3 ));
    P1->SEL0 |= (( 1 << 2 ) | ( 1 << 3 ));
    P1->SEL1 &= ~(( 1 << 2 ) | ( 1 << 3 ));

    EUSCI_A0->CTLW0 = 0;

    EUSCI_A0->CTLW0 |= ( 1 << 0 );
    EUSCI_A0->CTLW0 &= ~(( 1 << 9 ) | ( 1 << 10 ));
    EUSCI_A0->CTLW0 &= ~( 1 << 8 );
    EUSCI_A0->CTLW0 &= ~( 1 << 3 );
    EUSCI_A0->CTLW0 &= ~( 1 << 15 );
    EUSCI_A0->CTLW0 |= ( 2 << 6 );

    EUSCI_A0->MCTLW &= ~(0xF1);
    EUSCI_A0->MCTLW |= (0x49 << 8) | ( 1 << 0 );

    EUSCI_A0->BRW = 13;
    EUSCI_A0->IE |= ( 1 << 2 );

    EUSCI_A0->CTLW0 &= ~( 1 << 0 );

    //NVIC_EnableIRQ(EUSCIA0_IRQn);
    //NVIC_SetPriority(EUSCIA0_IRQn ,6);
}

void vUARTReceive(void *pvParameters) {

    if( !uart_initialized ) {
        UART_Init();
        uart_initialized = 1;
    }

    xSemaphoreTake( uart_receive, 0 );

    while(1) {

        xSemaphoreTake( uart_peripheral, portMAX_DELAY );

        struct User_Command_Message msg;
        char curr = Read_Char();
        if(curr == '$') {
            curr = Read_Char();
            switch(curr) {
                case 'S':
                    msg.Command = SEND;
                    break;
                case 'I':
                    msg.Command = STREAM_START;
                    break;
                case 'O':
                    msg.Command = STREAM_STOP;
                    break;
                case 'U':
                    msg.Command = STATUS;
                    break;
                case 'L':
                    msg.Command = LOG;
                    break;
                case 'P':
                    msg.Command = EJECT_SD;
                    break;
                case 'M':
                    msg.Command = MOUNT_SD;
                    break;
                case 'T':
                    msg.Command = TIME_SET;
                    break;
                default:
                    msg.Command = ERROR;
                    break;
            }
        } else {
            msg.Command = ERROR;
        }

        xSemaphoreGive( uart_peripheral );

        xQueueSend( user_commands, (void *) &msg, portMAX_DELAY );
    }

    vTaskDelete( NULL );

}

void Write_Unsigned_Int( unsigned int data ) {
    char buffer[10] = {0};
    uint8_t length = 0;

    while( data != 0 ) {
        buffer[length] = (char)((data % 10)) + '0';
        length++;
        data /= 10;
    }

    for(int i=(length-1); i>=0; i--) {
        Write_Char(buffer[i]);
    }
}

void Write_Date_Uint( uint16_t data ) {
    char buffer[4] = {0};
    uint8_t length = 0;

    while(data != 0) {
        buffer[length] = (char)((data % 10)) + '0';
        length++;
        data /= 10;
    }

    if(length == 1) {
        Write_Char('0');
        Write_Char(buffer[0]);
    } else {
        for(int i=(length-1); i>=0; i--) {
            Write_Char(buffer[i]);
        }
    }

}

void Write_Uint_As_Float( unsigned int data, uint8_t decimal_places ) {
    char buffer[10] = {0};
    uint8_t length = 0;

    while( data != 0 ) {
        buffer[length] = (char)((data % 10)) + '0';
        length++;
        data /= 10;
    }

    for(int i=(length-1); i>=decimal_places; i--) {
        Write_Char(buffer[i]);
    }

    if( decimal_places > 0 ) {
        Write_Char('.');

        for(int i=(decimal_places-1); i>=0; i--) {
            Write_Char(buffer[i]);
        }
    }
}

void Write_Float( float data, uint8_t decimal_places ) {
    for(int i=0; i<decimal_places; i++ ) {
        data *= 10;
    }

    unsigned int int_data = (unsigned int) data;

    Write_Uint_As_Float( int_data, 2 );
}

void Write_Date( struct Date date ) {
    Write_Char('S');
    Write_Char(':');
    Write_Date_Uint( date.Year );
    Write_Char('-');
    Write_Date_Uint( date.Month );
    Write_Char('-');
    Write_Date_Uint( date.Day );
    Write_Char('T');
    Write_Date_Uint( date.Hour );
    Write_Char(':');
    Write_Date_Uint( date.Min );
    Write_Char(':');
    Write_Date_Uint( date.Sec );
}

void Write_Pressure( uint16_t pressure ) {
    Write_Char('F');
    Write_Char(':');
    Write_Uint_As_Float(pressure, 2);
}

void Write_Temperature( uint16_t temperature ) {
    Write_Char('F');
    Write_Char(':');
    Write_Uint_As_Float(temperature, 2);
}

void Write_Humidity( uint8_t humidity ) {
    Write_Char('I');
    Write_Char(':');
    Write_Unsigned_Int( humidity );
}

void Write_Rotational_Speed( uint16_t rotational_speed ) {
    Write_Char('F');
    Write_Char(':');
    Write_Uint_As_Float( rotational_speed, 1 );
}

void Write_Shock_Data( struct MPU6050_Data data ) {
    Write_Char('F');
    Write_Char(':');
    Write_Float( data.Minimum, 2 );
    Write_Char(',');
    Write_Char('F');
    Write_Char(':');
    Write_Float( data.Maximum, 2 );
    Write_Char(',');
    Write_Char('F');
    Write_Char(':');
    Write_Float( data.Average, 2 );
    Write_Char(',');
    Write_Char('F');
    Write_Char(':');
    Write_Float( data.Stddev, 2 );
}

void vUARTSend(void *pvParameters) {

    if( !uart_initialized ) {
        UART_Init();
        uart_initialized = 1;
    }

    struct Uart_Data data;

    while(1) {

        xQueueReceive( uart_queue, &data, portMAX_DELAY );

        xSemaphoreTake( uart_peripheral, portMAX_DELAY );

        for(int i=0; i<data.len; i++) {
            Write_Char(data.data[i]);
        }

        xSemaphoreGive( uart_peripheral );

    }

    vTaskDelete( NULL );
}
