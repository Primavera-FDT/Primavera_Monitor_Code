#ifndef __PRIMAVERA_MPU6050_H__
#define __PRIMAVERA_MPU6050_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "i2c.h"

#include <stdint.h>
#include "math.h"

extern QueueHandle_t shock_1_queue;
extern QueueHandle_t shock_2_queue;

extern SemaphoreHandle_t mpu_sema_1;
extern SemaphoreHandle_t mpu_sema_2;
extern SemaphoreHandle_t mpu_sema_sample_1;
extern SemaphoreHandle_t mpu_sema_sample_2;

struct MPU6050_Data {
    float Average;
    float Minimum;
    float Maximum;
    float Stddev;
};

void vMPU6050_1(void *pvParameters);
void vMPU6050_2(void *pvParameters);

#endif
