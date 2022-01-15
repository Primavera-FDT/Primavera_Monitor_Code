/**********************************************************************
File        : uart_send.h
Author        : D. Theran
**********************************************************************/

#ifndef __PRIMAVERA_UART_SEND_H__
#define __PRIMAVERA_UART_SEND_H__

/**********************************************************************
*                        FORMAT OF THE STRING                        *
**********************************************************************/

/**********************************************
*  "<ID>,<TYPE>:<VALUE>,<TYPE>:<VALUE>\nHHHH" 
*
*  ID: Identification of the string 
*       D--> data
*       C --> command
*  
*  TYPE: type of the value 
*       I --> int
*       F --> float
*       U --> unsigned int
*  
*  VALUE: value that will be sent 
*
*  HHHH: Checksum value
***********************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/
#include "controller.h"
#include "dataconv.h"

/**********************************************************************
*                      GLOBALS AND DEFINITIONS                       *
**********************************************************************/
#define QUEUE_RECV_FROM_SRTREAM 0	/*Queau where the data from the 
					  sensors will be receive */		

/**********************************************************************
*                             STRUCTURES                             *
**********************************************************************/

struct uart_send_packet {
    uint16_t            temperature;
    uint8_t             humidity;
    uint32_t            pressure;
    uint32_t            rotational_speed;
    uint32_t            mpu1_minimum;
    uint32_t            mpu1_maximum;
    uint32_t            mpu1_mean;
    float               mpu1_stddev;
    uint32_t            mpu2_minimum;
    uint32_t            mpu2_maximum;
    uint32_t            mpu2_mean;
    float               mpu2_stddev;
};

/**********************************************************************
*                  PROTOTYPES OF EXPORTED FUNCTIONS                  *
**********************************************************************/

/************************************************
*  Task to sent the data through the UART bus  *
************************************************/
void vUartSend(void *pvParameters);

/*******************************************************
*  Convert the data from the structure into a string  *
*******************************************************/
void Create_string(struct uart_send_packet data, char *buf);

/**********************************
*  Format an integer to be sent  *
**********************************/
int Format_int(char *buf, char *aux_buf, int data);

/**********************************
*  Format an float to be sent  *
**********************************/
int Format_float(char *buf, char *aux_buf, float data);

/**********************************
*  Format an float to be sent  *
**********************************/
int Format_uint(char *buf, char *aux_buf, unsigned int data);

/********************************************************************
*  Initialize all the configuration parameters of the UART module  *
********************************************************************/
void UART_init(void);


#endif //__PRIMAVERA_UART_SEND_H

//end of the file 'uart_send.h'
