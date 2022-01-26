#include "rtc.h"

QueueHandle_t date_queue;
SemaphoreHandle_t rtc_sema;

void vRTC(void *pvParameters) {

    struct Date date;

    date.Year = 2022;
    date.Month = 1;
    date.Day = 27;
    date.Hour = 22;
    date.Min = 10;
    date.Sec = 5;

    while(1) {

        xSemaphoreTake( rtc_sema, portMAX_DELAY );
        
        xQueueSend( date_queue, &date, portMAX_DELAY );
    }

    vTaskDelete( NULL );
}
