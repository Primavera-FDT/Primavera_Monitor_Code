#include "bme280.h"

QueueHandle_t pressure_queue;
QueueHandle_t temperature_queue;
QueueHandle_t humidity_queue;

SemaphoreHandle_t bme280_sema;

void vBME280(void *pvParameters) {

    uint16_t pressure = 12345;
    uint16_t temperature = 3000;
    uint16_t humidity = 50;

    while(1) {

        xSemaphoreTake( bme280_sema, portMAX_DELAY );

        struct I2C_Address addr = { .write_address = 0x80, .read_address = 0x81, .width = 8};

        I2C_Write(0, addr, I2C_WRITE, 0x50 );
        I2C_Write(0, addr, I2C_WRITE_COND, 0x50 );
        I2C_Write(0, addr, I2C_READ, 0x20 );

        xQueueSend( pressure_queue, &pressure, portMAX_DELAY );
        xQueueSend( temperature_queue, &temperature, portMAX_DELAY );
        xQueueSend( humidity_queue, &humidity, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
