#include "mpu6050.h"

QueueHandle_t shock_1_queue;
QueueHandle_t shock_2_queue;

SemaphoreHandle_t mpu_sema_1;
SemaphoreHandle_t mpu_sema_2;

SemaphoreHandle_t mpu_sema_sample_1;
SemaphoreHandle_t mpu_sema_sample_2;

void vMPU6050_1(void *pvParameters) {

    struct MPU6050_Data shock;

    struct I2C_Address address;

    float samples[10];

    while(1) {

        for(int i=0; i<10; i++) {
            uint16_t x = 0;
            uint16_t y = 0;
            uint16_t z = 0;
            float x_f = 0.0;
            float y_f = 0.0;
            float z_f = 0.0;
            uint8_t data;
            xSemaphoreTake( mpu_sema_sample_1, portMAX_DELAY );

            I2C_Read(1, address, 0x3C, &data );
            x = data;
            I2C_Read(1, address, 0x3B, &data );
            x += (uint16_t)data << 8;

            I2C_Read(1, address, 0x3E, &data );
            y = data;
            I2C_Read(1, address, 0x3D, &data );
            y += (uint16_t)data << 8;

            I2C_Read(1, address, 0x40, &data );
            z = data;
            I2C_Read(1, address, 0x3F, &data );
            z += (uint16_t)data << 8;

            samples[i] = sqrt(x_f*x_f + y_f*y_f + z_f*z_f);
            shock.Average += samples[i];
            if(samples[i] > shock.Maximum ) {
                shock.Maximum = samples[i];
            } else if( samples[i] < shock.Minimum ) {
                shock.Minimum = samples[i];
            }

        }

        shock.Average /= 10.0;

        for(int i=0; i<10; i++) {
            shock.Stddev += (samples[i] - shock.Average) * (samples[i] - shock.Average);
        }
        shock.Stddev /= 10.0;
        shock.Stddev = sqrt(shock.Stddev);

        xSemaphoreTake( mpu_sema_1, portMAX_DELAY );

        xQueueSend( shock_1_queue, &shock, portMAX_DELAY );
    }

    vTaskDelete( NULL );
}

void vMPU6050_2(void *pvParameters) {

    struct MPU6050_Data shock;

    struct I2C_Address address;

    float samples[10];

    while(1) {

        for(int i=0; i<10; i++) {
            uint16_t x = 0;
            uint16_t y = 0;
            uint16_t z = 0;
            float x_f = 0.0;
            float y_f = 0.0;
            float z_f = 0.0;
            uint8_t data;
            xSemaphoreTake( mpu_sema_sample_2, portMAX_DELAY );

            I2C_Read(1, address, 0x3C, &data );
            x = data;
            I2C_Read(1, address, 0x3B, &data );
            x += (uint16_t)data << 8;

            I2C_Read(1, address, 0x3E, &data );
            y = data;
            I2C_Read(1, address, 0x3D, &data );
            y += (uint16_t)data << 8;

            I2C_Read(1, address, 0x40, &data );
            z = data;
            I2C_Read(1, address, 0x3F, &data );
            z += (uint16_t)data << 8;

            samples[i] = sqrt(x_f*x_f + y_f*y_f + z_f*z_f);
            shock.Average += samples[i];
            if(samples[i] > shock.Maximum ) {
                shock.Maximum = samples[i];
            } else if( samples[i] < shock.Minimum ) {
                shock.Minimum = samples[i];
            }

        }

        shock.Average /= 10.0;

        for(int i=0; i<10; i++) {
            shock.Stddev += (samples[i] - shock.Average) * (samples[i] - shock.Average);
        }
        shock.Stddev /= 10.0;
        shock.Stddev = sqrt(shock.Stddev);

        xSemaphoreTake( mpu_sema_2, portMAX_DELAY );

        xQueueSend( shock_2_queue, &shock, portMAX_DELAY );
    }

    vTaskDelete( NULL );
}
