#ifndef __PRIMAVERA_BME280_H__
#define __PRIMAVERA_BME280_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>

extern QueueHandle_t pressure_queue;
extern QueueHandle_t temperature_queue;
extern QueueHandle_t humidity_queue;

extern SemaphoreHandle_t bme280_sema;

void vBME280(void *pvParameters);

#endif
