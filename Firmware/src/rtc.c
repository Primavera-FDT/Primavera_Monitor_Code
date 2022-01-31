#include "rtc.h"

QueueHandle_t date_queue;
SemaphoreHandle_t rtc_sema;

void vRTC(void *pvParameters) {

    struct I2C_Address address = {
        .write_address = 0xC0,
        .read_address = 0xC1,
        .width = 8,
    };

    while(1) {

        struct Date date = {
            .Year = 0,
            .Month = 0,
            .Day = 0,
            .Hour = 0,
            .Min = 0,
            .Sec = 0
        };
        uint8_t data;

        xSemaphoreTake( rtc_sema, portMAX_DELAY );

        I2C_Read( 1, address, 0x00, &data );

        date.Sec += (data & 0x0F);
        date.Sec += (((data & 0x70) >> 4) * 10);

        I2C_Read( 1, address, 0x01, &data );

        date.Min += (data & 0x0F);
        date.Min += (((data & 0x70) >> 4) * 10);

        I2C_Read( 1, address, 0x02, &data );

        date.Hour += (data & 0x0F);
        date.Hour += ( data & 0x10 ) ? 10 : 0;
        date.Hour += ( data & 0x20 ) ? 10 : 0;

        I2C_Read( 1, address, 0x04, &data );

        date.Day += ( data & 0x0F );
        date.Day += (((data & 0x30 ) >> 4 ) * 10);

        I2C_Read( 1, address, 0x05, &data );

        date.Month += ( data & 0x0F );
        date.Month += ( data & 0x10 ) ? 10 : 0;

        I2C_Read( 1, address, 0x06, &data );

        date.Year += ( data & 0x0F );
        date.Year += ((( data & 0xF0 ) >> 4) * 10);
        date.Year += 1970;
        
        xQueueSend( date_queue, &date, portMAX_DELAY );
    }

    vTaskDelete( NULL );
}
