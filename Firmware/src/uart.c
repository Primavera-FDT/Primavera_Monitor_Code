#include "uart.h"

static int uart_initialized = 0;

QueueHandle_t user_commands;
QueueHandle_t uart_queue;
SemaphoreHandle_t uart_peripheral;
SemaphoreHandle_t uart_receive;

void EUSCIA0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR( uart_receive, &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
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

void vUARTSend(void *pvParameters) {

    if( !uart_initialized ) {
        UART_Init();
        uart_initialized = 1;
    }

    char *msg;

    while(1) {

        xQueueReceive( uart_queue, &msg, portMAX_DELAY );

        int counter = 0;
        while(msg[counter] != '\0') {
            Write_Char(msg[counter]);
            counter++;
        }

    }

    vTaskDelete( NULL );
}
