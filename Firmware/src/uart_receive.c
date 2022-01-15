/**********************************************************************
*File               : uart_receive.c 
*Author             : D. Theran
**********************************************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/

#include "uart_receive.h"

/**********************************************************************
*                  DEFINITION OF EXPORTED FUNCTIONS                  *
**********************************************************************/

//********************************************************************
void vUartReceive(void *pvParameters){
//********************************************************************
    UART_init();
    char command;
    
    while(1){

        xSemaphoreTake(Mutex[0], portMAX_DELAY);
        EUSCI_A0->IE = (BIT2);                 //enable the start bit interrupt
        while((EUSCI_A0->IFG & BIT2) != BIT2); /* Wait until a start bit is 
                                                  sent*/
        EUSCI_A0->IE |= BIT0;                  //enable the receive interrupt
        command = (char) EUSCI_A0->RXBUF;
        while((EUSCI_A0->IFG & BIT0) != BIT0); /* Wait until the character has 
                                                  been completely received*/
        SendCommand(command);
        xSemaphoreGive(Mutex[0]);         
    }

    vTaskDelete( NULL );
}


//********************************************************************
void UART_init(void){
//********************************************************************
    EUSCI_A0->CTLW0 = (BITA | BIT9 | BIT7); /*Enable the UART mode with 
                                              automatic baud rate detection
                                              and source clock SMCLK*/
    
    EUSCI_A0->BRW = (BIT3 | BIT4 | BIT5 | BIT8); /*Set the clock prescaler
                                                   to 312 in decimal*/

    EUSCI_A0->MCTLW = (BIT7 | BIT0); /* Set the first modulation stage in 8,
                                        and enable the oversamplig*/   
}


//********************************************************************
void SendCommand(char command){
//********************************************************************
    switch (command) {
        case 'S':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)SEND, portMAX_DELAY);
            break;
        case 'I':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)STREAM_START, portMAX_DELAY);
            break;
        case 'O':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)STREAM_STOP, portMAX_DELAY);
            break;
        case 'U':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)STATUS, portMAX_DELAY);
            break;
        case 'L':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)LOG, portMAX_DELAY);
            break;
        case 'P':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)EJECT_SD, portMAX_DELAY);
            break;
        case 'M':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)MOUNT_SD, portMAX_DELAY);
            break;
        case 'T':
            xQueueSend(Queue[QUEUE_SEND_TO_MAIN], (void *)TIME_SET, portMAX_DELAY);
            break;
    }

}
