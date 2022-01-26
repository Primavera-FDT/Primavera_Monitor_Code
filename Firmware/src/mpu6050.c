#include "mpu6050.h"

QueueHandle_t shock_1_queue;
QueueHandle_t shock_2_queue;

SemaphoreHandle_t mpu_sema;

SemaphoreHandle_t shock_1_mutex;
SemaphoreHandle_t shock_2_mutex;

void vMPU6050(void *pvParameters) {

    struct MPU6050_Data shock_1;
    struct MPU6050_Data shock_2;

    shock_1.Average = 120.0;
    shock_1.Minimum = 80.0;
    shock_1.Maximum = 150.0;
    shock_1.Stddev = 25.0;

    shock_2.Average = 120.0;
    shock_2.Minimum = 80.0;
    shock_2.Maximum = 150.0;
    shock_2.Stddev = 25.0;

    while(1) {

        xSemaphoreTake( mpu_sema, portMAX_DELAY );
        
        xQueueSend(shock_1_queue, &shock_1, portMAX_DELAY );
        xQueueSend(shock_2_queue, &shock_2, portMAX_DELAY );
    }

    vTaskDelete( NULL );
}
