/**********************************************************************
* File:             bme.h
* Author:           D. Theran. 
**********************************************************************/

#ifndef __BME_H__  
#define __BME_H__

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/

#include "i2c.h"
#include "controller.h"

/**********************************************************************
*                      GLOBALS AND DEFINITIONS                       *
**********************************************************************/

/*QUEUES*/
#define QUEUE_SEND_TO_STREAM_TEMP 3
#define QUEUE_SEND_TO_STREAM_HUMI 4
#define QUEUE_SEND_TO_STREAM_PRESS 5

/*MUTEX*/
#define MUTEX_TEMP 2
#define MUTEX_HUMI 3
#define MUTEX_PRESS 4

typedef uint32_t BME280_U32_t;
typedef int32_t BME280_S32_t;
typedef int64_t BME280_S64_t;

struct I2C_Client bme_client;

bme_client.read_address = 0b11101100;
bme_client.write_address = 0b11101101;
bm_client.bus = 0;
 

/**********************************************************************
*                  PROTOTYPES OF EXPORTED FUNCTIONS                  *
**********************************************************************/

/*****************************************************
*  Task to send the sensor data to the UART Module  *
*****************************************************/
void vBMESensor(void *pvParameters);

/************************************************
*  Returns the final value of the temperature  *
************************************************/
int Read_T(BME280_S32_t adc_Temp);

/*********************************************
*  Returns the final value of the Pressure  *
*********************************************/
unsigned int Read_P(BME280_S32_t adc_Press);

/*********************************************
*  Returns the final value of the humidity  *
*********************************************/
unsigned int Read_H(BME280_S32_t adc_Humi);

/**********************************************************
*  Returns the temperature in DegC as integer of 32 bits  *
***********************************************************/
BME280_S32_t BME280_Compensate_T_int32(BME280_S32_t adc_T);

/***********************************************************
*  Returns the pressure in Pa as unsigned int of 32 bits  *
***********************************************************/
BME280_U32_t BME280_Compensate_P_int64(BME280_S32_t adc_P);

/********************************************************
*  Returns humidity in %RH as unsigned int of 32 bits  *
********************************************************/
BME280_U32_t BME280_Compensate_H_int32(BME280_S32_t adc_H)


#endif //__BME_H__
