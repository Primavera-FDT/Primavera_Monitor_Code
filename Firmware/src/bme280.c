#include "bme280.h"

QueueHandle_t pressure_queue;
QueueHandle_t temperature_queue;
QueueHandle_t humidity_queue;

SemaphoreHandle_t bme280_sema;

void vBME280(void *pvParameters) {

    uint16_t pressure = 18000;
    uint16_t temperature = 3000;
    uint16_t humidity = 50;

    while(1) {

        xSemaphoreTake( bme280_sema, portMAX_DELAY );

        xQueueSend( pressure_queue, &pressure, portMAX_DELAY );
        xQueueSend( temperature_queue, &temperature, portMAX_DELAY );
        xQueueSend( humidity_queue, &humidity, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
