#include "uart.h"

static int uart_initialized = 0;

char circle_buffer[20] = {0};
uint8_t last = 0;
uint8_t first = 0;

QueueHandle_t user_commands;
QueueHandle_t uart_queue;
SemaphoreHandle_t uart_peripheral;
SemaphoreHandle_t uart_receive;
SemaphoreHandle_t uart_send_atomic;

void EUSCIA0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR( uart_receive, &xHigherPriorityTaskWoken );

    circle_buffer[last] = EUSCI_A0->RXBUF;
    last++;
    if( last == 20 ) {
        last = 0;
    }

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

    uart_data.len = len;
    for(int i=0; i<len; i++) {
        uart_data.data[i] = data[i];
    }

    xQueueSend( uart_queue, &uart_data, portMAX_DELAY );
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

    EUSCI_A0->CTLW0 &= ~( 1 << 0 );
    EUSCI_A0->IE |= 1;


    NVIC_EnableIRQ(EUSCIA0_IRQn);
    NVIC_SetPriority(EUSCIA0_IRQn ,6);
}

void vUARTReceive(void *pvParameters) {

    if( !uart_initialized ) {
        UART_Init();
        uart_initialized = 1;
    }

    struct User_Command_Message msg;
    enum Uart_Receive_State state = UART_START;

    xSemaphoreTake( uart_receive, 0 );

    while(1) {

        if( last == first ) {
            xSemaphoreTake( uart_receive, portMAX_DELAY );
        } else {
            xSemaphoreTake( uart_receive, 0 );
        }

        char curr_char = circle_buffer[first];
        first++;
        if( first == 20 ) {
            first = 0;
        }

        switch(state) {
            case UART_START:
                if(curr_char == '$') {
                    state = UART_START_COMMAND;
                } else {
                    msg.command = ERROR;
                    xQueueSend( user_commands, &msg, portMAX_DELAY );
                }
                break;
            case UART_START_COMMAND:
                switch(curr_char) {
                    case 'S':
                        msg.command = SEND;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'I':
                        msg.command = STREAM_START;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'O':
                        msg.command = STREAM_STOP;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'U':
                        msg.command = STATUS;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'L':
                        msg.command = LOG;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'P':
                        msg.command = EJECT_SD;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    case 'M':
                        msg.command = MOUNT_SD;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                    // Time_Set : T
                    default:
                        msg.command = INVALID_COMMAND;
                        state = UART_SIMPLE_COMMAND_NL;
                        break;
                }
                break;
            case UART_SIMPLE_COMMAND_NL:
                if( curr_char == '\n') {
                    state = UART_SIMPLE_COMMAND_CR;
                } else {
                    msg.command = ERROR;
                    xQueueSend( user_commands, &msg, portMAX_DELAY );
                    state = UART_START;
                }
                break;
            case UART_SIMPLE_COMMAND_CR:
                if( curr_char == '\r') {
                    state = UART_START;
                    xQueueSend( user_commands, &msg, portMAX_DELAY );
                } else {
                    msg.command = ERROR;
                    xQueueSend( user_commands, &msg, portMAX_DELAY );
                    state = UART_START;
                }
                break;
        };
    }

    vTaskDelete( NULL );

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
