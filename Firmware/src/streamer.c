#include "streamer.h"

char *stream = "ok\n";

SemaphoreHandle_t streamer_sema;

void vStreamer(void *pvParameters) {
    while(1) {

        uint16_t pressure;
        uint16_t temperature;
        uint16_t humidity;
        uint16_t rotational_speed;

        struct MPU6050_Data shock_1;
        struct MPU6050_Data shock_2;

        struct Date date;

        xSemaphoreTake( streamer_sema, portMAX_DELAY );

        xQueueReceive( pressure_queue, &pressure, portMAX_DELAY );
        xQueueReceive( temperature_queue, &temperature, portMAX_DELAY );
        xQueueReceive( humidity_queue, &humidity, portMAX_DELAY );
        xQueueReceive( shock_1_queue, &shock_1, portMAX_DELAY );
        xQueueReceive( shock_2_queue, &shock_2, portMAX_DELAY );
        xQueueReceive( date_queue, &date, portMAX_DELAY );
        xQueueReceive( rotational_speed_queue, &rotational_speed, portMAX_DELAY );

        P1->OUT |= 1;

        xQueueSend( uart_queue, &stream, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
