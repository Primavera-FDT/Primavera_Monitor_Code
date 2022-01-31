#include "streamer.h"

SemaphoreHandle_t streamer_sema;

static uint8_t count = 0;
static char buffer[10] = {0};

void Write_To_Char_Buffer(char c) {
    if( count == 10 ) {
        Uart_Send_Atomic( buffer, count );
        count = 0;
    }

    buffer[count] = c;
    count++;
}

void Flush_Char_Buffer() {
    if( count > 0 ) {
        Uart_Send_Atomic(buffer, count);
        count = 0;
    }
}

void Write_Date_Uint( unsigned int data ) {
    char buffer[4] = {0};
    uint8_t length = 0;

    while(data != 0) {
        buffer[length] = (char)((data % 10)) + '0';
        length++;
        data /= 10;
    }

    if(length == 1) {
        Write_To_Char_Buffer('0');
        Write_To_Char_Buffer(buffer[0]);
    } else {
        for(int i=(length-1); i>=0; i--) {
            Write_To_Char_Buffer(buffer[i]);
        }
    }
}

void Write_Uint_As_Float( unsigned int data, uint8_t decimal_places ) {
    char buffer[10] = {0};
    uint8_t length = 0;

    while( data != 0 ) {
        buffer[length] = (char)((data % 10)) + '0';
        length++;
        data /= 10;
    }

    for(int i=(length-1); i>=decimal_places; i--) {
        Write_To_Char_Buffer(buffer[i]);
    }

    if( decimal_places > 0 ) {
        Write_To_Char_Buffer('.');

        for(int i=(decimal_places-1); i>=0; i--) {
            Write_To_Char_Buffer(buffer[i]);
        }
    }
}

void Write_Float( float data, uint8_t decimal_places ) {
    for(int i=0; i<decimal_places; i++ ) {
        data *= 10;
    }

    unsigned int int_data = (unsigned int) data;
    Write_Uint_As_Float( int_data, decimal_places );
}

void Write_Pressure_To_Buffer( uint16_t pressure ) {
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Uint_As_Float(pressure, 2);
}

void Write_Temperature_To_Buffer( uint16_t temperature ) {
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Uint_As_Float(temperature, 2);
}

void Write_Humidity_To_Buffer( uint8_t humidity ) {
    Write_To_Char_Buffer('I');
    Write_To_Char_Buffer(':');
    Write_Uint_As_Float( humidity, 0 );
}

void Write_Rotational_Speed_To_Buffer( uint16_t rotational_speed ) {
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Uint_As_Float( rotational_speed, 1 );
}

void Write_Shock_To_Buffer( struct MPU6050_Data *data ) {
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Float( data->Minimum, 2 );
    Write_To_Char_Buffer(',');
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Float( data->Maximum, 2 );
    Write_To_Char_Buffer(',');
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Float( data->Average, 2 );
    Write_To_Char_Buffer(',');
    Write_To_Char_Buffer('F');
    Write_To_Char_Buffer(':');
    Write_Float( data->Stddev, 2 );
}

void Write_Date_To_Buffer( struct Date *date ) {
    Write_To_Char_Buffer('S');
    Write_To_Char_Buffer(':');
    Write_Date_Uint( date->Year );
    Write_To_Char_Buffer('-');
    Write_Date_Uint( date->Month );
    Write_To_Char_Buffer('-');
    Write_Date_Uint( date->Day );
    Write_To_Char_Buffer('T');
    Write_Date_Uint( date->Hour );
    Write_To_Char_Buffer(':');
    Write_Date_Uint( date->Min );
    Write_To_Char_Buffer(':');
    Write_Date_Uint( date->Sec );
}

void vStreamer(void *pvParameters) {

    struct Sensor_Data data;

    while(1) {

        xSemaphoreTake( streamer_sema, portMAX_DELAY );

        //xQueueReceive( pressure_queue, &data.pressure, portMAX_DELAY );
        //xQueueReceive( temperature_queue, &data.temperature, portMAX_DELAY );
        //xQueueReceive( humidity_queue, &data.humidity, portMAX_DELAY );
        //xQueueReceive( shock_1_queue, &data.shock_1, portMAX_DELAY );
        //xQueueReceive( shock_2_queue, &data.shock_2, portMAX_DELAY );
        xQueueReceive( date_queue, &data.date, portMAX_DELAY );
        //xQueueReceive( rotational_speed_queue, &data.rotational_speed, portMAX_DELAY );

        Uart_Claim_Atomic();

        Write_To_Char_Buffer('D');
        Write_To_Char_Buffer(' ');

        //Write_Pressure_To_Buffer( data.pressure );
        //Write_To_Char_Buffer(',');
        //Write_Temperature_To_Buffer( data.temperature );
        //Write_To_Char_Buffer(',');
        //Write_Humidity_To_Buffer( data.humidity );
        //Write_To_Char_Buffer(',');
        //Write_Shock_To_Buffer( &(data.shock_1) );
        //Write_To_Char_Buffer(',');
        //Write_Shock_To_Buffer( &(data.shock_2) );
        //Write_To_Char_Buffer(',');
        Write_Date_To_Buffer( &(data.date) );
        Write_To_Char_Buffer(',');
        //Write_Rotational_Speed_To_Buffer( data.rotational_speed );
        Write_To_Char_Buffer('\n');
        Write_To_Char_Buffer('\r');

        Flush_Char_Buffer();

        Uart_Claim_Release();
        
    }

    vTaskDelete( NULL );
}
