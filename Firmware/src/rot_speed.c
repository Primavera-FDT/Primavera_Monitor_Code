#include "rot_speed.h"

QueueHandle_t rotational_speed_queue;
SemaphoreHandle_t rot_speed_sema;

void vRotSpeed( void *pvParameters ) {

    uint16_t speed = 6000;

    while(1) {

        xSemaphoreTake( rot_speed_sema, portMAX_DELAY );

        xQueueSend( rotational_speed_queue, &speed, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
