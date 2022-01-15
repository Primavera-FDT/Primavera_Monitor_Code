/**********************************************************************
* File:             stream.h
* Author:           D. Theran. 
**********************************************************************/

#ifndef __STREAM__H__  
#define __STREAM_H__

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/

#include "controller.h"
#include "uart_send.h"
#include "uart_receive.h"

/**********************************************************************
*                      GLOBALS AND DEFINITIONS                       *
**********************************************************************/

/*QUEUES */
#define QUEUE_SEND_TO_UART 0        /*Queue to send the data to the UART module*/
#define QUEUE_RECV_FROM_MAIN 2      /*Queue to receive commands from main*/
#define QUEUE_RECV_FROM_TEMP 3      /*Queue to receive the temperature data*/
#define QUEUE_RECV_FROM_HUMI 4      /*Queue to receive the humidity data*/
#define QUEUE_RECV_FROM_PRESS 5     /*Queue to receive the pressure data*/
#define QUEUE_RECV_FROM_ROTSPEED 6  /*Queue to receive the rotational speed data*/
#define QUEUE_RECV_FROM_MPU1 7      /*Queue to receive the mpu1 data*/
#define QUEUE_RECV_FROM_MPU2 8      /*Queue to receive the mpu2 data*/
#define QUEUE_SEND_TO_SPI 9         /*Queue to send the data to the SPI module*/

/*MUTEX*/
#define MUTEX_MAIN 1            /*Mutex shared with the main*/
#define MUTEX_TEMP 2            /*Mutex shared with the temperature sensor*/
#define MUTEX_HUMI 3            /*Mutex shared with the humidity sensor*/
#define MUTEX_PRESS 4           /*Mutex shared with the pressure sensor*/
#define MUTEX_ROTSPEED 5        /*Mutex shared with the rotational speed sensor*/
#define MUTEX_MPU1 6            /*Mutex shared with the mpu1 sensor*/
#define MUTEX_MPU2 7            /*Mutex shared with the mpu2 sensor*/

/**********************************************************************
*                             STRUCTURES                             *
**********************************************************************/

/*Struct of the mpu1 and mpu2 data*/
struct mpu {
    float   minimum;
    float   maximum;
    float   mean;
    float   stddev;
};

/**********************************************************************
*                  PROTOTYPES OF EXPORTED FUNCTIONS                  *
**********************************************************************/

/*****************************************************
*  Task to send the sensor data to the UART Module  *
*****************************************************/
void vStream(void *pvParameters);

/*****************************************************
*  Fill the structure of the data from the sensors  *
*****************************************************/
void CreateData(struct uart_send_packet data);



#endif //__STREAM__H__
