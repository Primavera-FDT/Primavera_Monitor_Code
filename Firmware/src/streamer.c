#include "streamer.h"

char *stream = "ok\n";

SemaphoreHandle_t streamer_sema;

void vStreamer(void *pvParameters) {

    struct Sensor_Data data;

    while(1) {

        xSemaphoreTake( streamer_sema, portMAX_DELAY );

        xQueueReceive( pressure_queue, &data.pressure, portMAX_DELAY );
        xQueueReceive( temperature_queue, &data.temperature, portMAX_DELAY );
        xQueueReceive( humidity_queue, &data.humidity, portMAX_DELAY );
        xQueueReceive( shock_1_queue, &data.shock_1, portMAX_DELAY );
        xQueueReceive( shock_2_queue, &data.shock_2, portMAX_DELAY );
        xQueueReceive( date_queue, &data.date, portMAX_DELAY );
        xQueueReceive( rotational_speed_queue, &data.rotational_speed, portMAX_DELAY );

        //xQueueSend( uart_queue, &data, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
