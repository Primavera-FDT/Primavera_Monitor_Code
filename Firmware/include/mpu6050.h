#ifndef __PRIMAVERA_MPU6050_H__
#define __PRIMAVERA_MPU6050_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdint.h>

extern QueueHandle_t shock_1_queue;
extern QueueHandle_t shock_2_queue;

extern SemaphoreHandle_t mpu_sema;

extern SemaphoreHandle_t shock_1_mutex;
extern SemaphoreHandle_t shock_2_mutex;

struct MPU6050_Data {
    float Average;
    float Minimum;
    float Maximum;
    float Stddev;
};

void vMPU6050(void *pvParameters);

#endif
