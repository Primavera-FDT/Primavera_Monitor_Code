/**********************************************************************
File     : uart_send.c
Author   : D. Theran
**********************************************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/
#include "uart_send.h"

/**********************************************************************
*                  DEFINITIONS, GLOBALS AND MACROS                   *
**********************************************************************/
struct uart_send_packet data;
char sendBuffer[256];

/**********************************************************************
*                 DEFINITIONS OF EXPORTED FUNCTIONS                  *
**********************************************************************/

//********************************************************************
void vUartSend(void *pvParameters) {
//********************************************************************
    UART_init();

    while(1) {
    
        xSemaphoreTake(Mutex[0], portMAX_DELAY);
        xQueueReceive(Queue[QUEUE_RECV_FROM_STREAM], (void *)&data, 0);
        Create_string(data, sendBuffer);

        for (int i = 0; i < strlen(sendBuffer) + 1) {
            if((EUSCI_A0->STATW & BIT0) != BIT0){  //if not busy
                EUSCI_A0->IE = (BIT3);             /*enable the transmit complete 
                                                     interrupt*/
                EUSCI_A0->TXBUF = sendBuffer[i];   //send the character
                while((EUSCI_A0->IFG & BIT3) != BIT3); /* wait while the buffer
                                                         is not empty*/
                EUSCI_A0->IFG = 0; 
                i++;
            }                       
        }

        xSemaphoreGive(Mutex[0]);
    }

    vTaskDelete( NULL );

}


//********************************************************************
void Create_string(struct uart_send_packet data, char *buf){
//********************************************************************
    char *aux_buf;

    /* Set the id of the string*/
    *buf++ = 'D';  
    *buf++ = ','; 
    
    /*Set the values of the data*/
    /*The order is: 
     * Temperature, humidity, pressure, rotational_speed, mpu1_minimum,
     * mpu1_maximum, mpu1_mean, mpu1_stddex, mpu2_minimum, mpu2_maximun, 
     * mpu2_mean, mpu2_stddev*/
    Format_int(buf, aux_buf, (int)data.temperature);
    Format_uint(buf, aux_buf, data.humidity);
    Format_uint(buf, aux_buf, data.pressure);
    Format_uint(buf, aux_buf, data.rotational_speed);
    Format_float(buf, aux_buf, data.mpu1_minimum);
    Format_float(buf, aux_buf, data.mpu1_maximun);
    Format_float(buf, aux_buf, data.mpu1_mean);
    Format_float(buf, aux_buf, data.mpu1_stddev);
    Format_float(buf, aux_buf, data.mpu2_minimum);
    Format_float(buf, aux_buf, data.mpu2_maximun);
    Format_float(buf, aux_buf, data.mpu2_mean);
    Format_float(buf, aux_buf, data.mpu2_stddev);

    /* Set the jump of line*/
    *buf++ = '\n';

    /* Set the CRC Checksum*/
    AppendCRC(buf);
    
    /*Set the end of the string*/
    *buf++ = '\0';
}

//********************************************************************
int Format_int(char *buf, char *aux_buf, int data){
//********************************************************************
    char *loc;

    *buf++ = 'I';
    *buf++ = ':';
    if(!IntToString(aux_buf, loc, data))
        return DAT_ERROR;

    unsafe_concat(buf, aux_buf);
    *buf++ = ',';

    return DAT_OK;
}

//********************************************************************
int Format_float(char *buf, char *aux_buf, float data){
//********************************************************************
    int ret;
    char *loc;

    *buf++ = 'F';
    *buf++ = ':';
    if(!FloatToString(aux_buf, loc, data))
        return DAT_ERROR;

    unsafe_concat(buf, aux_buf);
    *buf++ = ',';
    
    return DAT_OK;
}

//********************************************************************
int Format_uint(char *buf, char *aux_buf, unsigned int data){
//********************************************************************
    int ret;
    char *loc;

    *buf++ = 'F';
    *buf++ = ':';
    if(!UintToString(aux_buf, loc, data))
        return DAT_ERROR;

    unsafe_concat(buf, aux_buf);
    *buf++ = ',';
    
    return DAT_OK;
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
