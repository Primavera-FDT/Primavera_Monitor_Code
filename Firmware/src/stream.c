/**********************************************************************
File     : stream.c
Author   : D. Theran
**********************************************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/
#include "stream.h"

/**********************************************************************
*                  DEFINITIONS, GLOBALS AND MACROS                   *
**********************************************************************/
struct uart_send_packet data;

/**********************************************************************
*                 DEFINITIONS OF EXPORTED FUNCTIONS                  *
**********************************************************************/

//********************************************************************
void vStream(void *pvParameters) {
//********************************************************************
    int command;
    while(1){
        
        xSemaphoreTake(Mutex[MUTEX_MAIN], portMAX_DELAY);
        xQueueReceive(Queue[QUEUE_RECV_FROM_MAIN], &command, 0);
        if(command == SEND || command == STREAM_START){
            CreateData(data);
            xQueueSend(Queue[QUEUE_SEND_TO_UART], (void *)&data, portMAX_DELAY);
            xQueueSend(Queue[QUEUE_SEND_TO_SPI], (void *)&data, portMAX_DELAY);
        }
        xSemaphoreGive(Mutex[MUTEX_MAIN]);
    }

    vTaskDelete( NULL );

}


//********************************************************************
void CreateData(struct uart_send_packet *data){
//********************************************************************
    struct mpu mpu1, mpu2;
    int temperature;
    unsigned int rot_speed, humidity, pressure;

    xSemaphoreTake(Mutex[MUTEX_TEMP], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_TEMP], (void *)&temperature, 0);
    xSemaphoreGive(Mutex[MUTEX_TEMP]);
    data->temperature = temperature;

    xSemaphoreTake(Mutex[MUTEX_HUMI], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_HUMI], (void *)&humidity, 0);
    xSemaphoreGive(Mutex[MUTEX_HUMI]);
    data->humidity = humidity;
    
    xSemaphoreTake(Mutex[MUTEX_PRESS], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_PRESS], (void *)&pressure, 0);
    xSemaphoreGive(Mutex[MUTEX_PRESS]);
    data->pressure = pressure;
    
    xSemaphoreTake(Mutex[MUTEX_ROTSPEED], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_ROTSPEED], (void *)&rot_speed, 0);
    xSemaphoreGive(Mutex[MUTEX_ROTSPEED]);
    data->rotational_speed = rot_speed;

    xSemaphoreTake(Mutex[MUTEX_MPU1], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_MPU1], (void *)&mpu1, 0);
    xSemaphoreGive(Mutex[MUTEX_MPU1]);
    data->mpu1_minimum = mpu1.minimum;
    data->mpu1_maximum = mpu1.maximun;
    data->mpu1_mean = mpu1.mean;
    data->mpu1_stddev = mpu1.stddev;

    xSemaphoreTake(Mutex[MUTEX_MPU2], portMAX_DELAY);
    xQueueReceive(Queue[QUEUE_RECV_FROM_MPU2], (void *)&mpu2, 0);
    xSemaphoreGive(Mutex[MUTEX_MPU2]);
    data->mpu2_minimum = mpu2.minimum;
    data->mpu2_maximum = mpu2.maximun;
    data->mpu2_mean = mpu2.mean;
    data->mpu2_stddev = mpu2.stddev;

}
