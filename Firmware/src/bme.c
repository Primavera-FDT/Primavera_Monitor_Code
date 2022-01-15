/**********************************************************************
File     : bme.c
Author   : D. Theran
**********************************************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/
#include "bme.h"

/**********************************************************************
*                  DEFINITIONS, GLOBALS AND MACROS                   *
**********************************************************************/

/*Final data that comes from the sensor*/
unsigned int humidity, pressure;
int temperature;

/*API internal values*/
uint8_t id;
uint32_t adc_Temp, adc_Press, adc_Humi;
uint8_t len_rec_msg = 8;
unsigned char bme_reg = 0xF7;
unsigned char *recv_msg;
BME280_S32_t t_fine;

/*Temperature trimming values*/
unsigned short dig_T1;
short dig_T2, dig_T3;

/*Pressure trimming values*/
unsigned short dig_P1;
short dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

/*Humidity trimming values*/
unsigned char dig_H1, dig_H3, dig_H6;
short dig_H2, dig_H4, dig_H5;

/**********************************************************************
*                 DEFINITIONS OF EXPORTED FUNCTIONS                  *
**********************************************************************/

//********************************************************************
void vBMESensor(void *pvParameters) {
//********************************************************************
    
    while(1){
        
        id = I2C_Send(bme_client, &bme_reg, 1);
        I2C_Read(bme_client, id, recv_msg, &len_rec_msg);

        adc_Press = recv_msg >> 40;
        adc_Temp = (recv_msg >> 16) - (adc_Press << 24);
        adc_Humi = recv_msg - ((adc_Temp << 16) + (adc_Press << 40));

        temperature = Read_T(adc_Temp);
        pressure = Read_P(adc_Temp);
        humidity = Read_T(adc_Humi);


        xSemaphoreTake(Mutex[MUTEX_TEMP], portMAX_DELAY);
        xQueueSend(Queue[QUEUE_SEND_TO_STREAM_TEMP], (void *)&temperature, portMAX_DELAY);
        xSemaphoreGive(Mutex[MUTEX_TEMP]);

        xSemaphoreTake(Mutex[MUTEX_PRESS], portMAX_DELAY);
        xQueueSend(Queue[QUEUE_SEND_TO_STREAM_PRESS], (void *)&pressure, portMAX_DELAY);
        xSemaphoreGive(Mutex[MUTEX_PRESS]);

        xSemaphoreTake(Mutex[MUTEX_HUMI], portMAX_DELAY);
        xQueueSend(Queue[QUEUE_SEND_TO_STREAM_HUMI], (void *)&humidity, portMAX_DELAY);
        xSemaphoreGive(Mutex[MUTEX_HUMI]);
        
    }

    vTaskDelete( NULL );
}

//********************************************************************
int Read_T(BME280_S32_t adc_Temp) {
//********************************************************************

    unsigned char *recv_temp;

    unsigned char temp_reg = 0x88;
    uint8_t len_temp_reg = 6;
    BME280_S32_t adc_T = adc_Temp;

    id = I2C_Send(bme_client, &temp_reg, 1);
    I2C_Read(bme_client, id, recv_temp, &len_temp_reg);

    dig_T1 = recv_temp >> 32;
    dig_T2 = (recv_temp >> 16) - (dig_T1 << 16);
    dig_T3 = recv_temp - ((dig_T1 << 32) + (dig_T2 << 16));
    
    BME280_S32_t temp = BME280_Compensate_T_int32(adc_T);
    
    return (int) temp;

}

//********************************************************************
unsigned int Read_P(BME280_S32_t adc_Press) {
//********************************************************************

    unsigned char *recv_press;

    unsigned char press_reg = 0x8E;
    uint8_t len_press_reg = 18;
    BME280_S32_t adc_P = adc_Press;

    id = I2C_Send(bme_client, &press_reg, 1);
    I2C_Read(bme_client, id, recv_press, &len_press_reg);

    dig_P1 = (recv_press >> 128);
    dig_P2 = (recv_press >> 112) - (dig_P1 << 16);
    dig_P3 = (recv_press >> 96) - ((dig_P1 << 32) + (dig_P2 << 16));
    dig_P4 = (recv_press >> 80) - ((dig_P1 << 48) + (dig_P2 << 32) + (dig_P3 << 16));
    dig_P5 = (recv_press >> 64) - ((dig_P1 << 64) + (dig_P2 << 48) + (dig_P3 << 32) +
            (dig_P4 << 16));
    dig_P6 = (recv_press >> 48) - ((dig_P1 << 80) + (dig_P2 << 64) + (dig_P3 << 48) +
            (dig_P4 << 32) + (dig_P5 << 16));
    dig_P7 = (recv_press >> 32) - ((dig_P1 << 96) + (dig_P2 << 80) + (dig_P3 << 64) +
            (dig_P4 << 48) + (dig_P5 << 32) + (dig_P6 << 16));
    dig_P8 = (recv_press >> 16) - ((dig_P1 << 112) + (dig_P2 << 92) + (dig_P3 << 80) +
            (dig_P4 << 64) + (dig_P5 << 48) + (dig_P6 << 32) + (dig_P7 << 16));
    dig_P9 = (recv_press) - ((dig_P1 << 128) + (dig_P2 << 112) + (dig_P3 << 96) +
            (dig_P4 << 8) + (dig_P5 << 64) + (dig_P6 << 48) + (dig_P7 << 32) + (dig_P8 << 16));

    
    BME280_U32_t press = BME280_Compensate_P_int64(adc_P);
    
    return (unsigned int) press;

}

//********************************************************************
unsigned int Read_H(BME280_S32_t adc_Humi) {
//********************************************************************

    unsigned char *recv_humi;

    unsigned char humi_reg = 0xA1;
    uint8_t len_humi_reg = 8;
    BME280_S32_t adc_H = adc_Humi;

    id = I2C_Send(bme_client, &humi_reg, 1);
    I2C_Read(bme_client, id, recv_humi, &len_humi_reg);

    dig_H1 = (recv_humi >> 64);
    dig_H2 = (recv_humi >> 40) - (dig_H1 << 16);
    dig_H3 = (recv_humi >> 32) - ((dig_H1 << 24) + (dig_H2 << 8));
    dig_H4 = (recv_humi >> 20) - ((dig_H1 << 44) + (dig_H2 << 20) + (dig_H3 << 12));
    dig_H5 = (recv_humi >> 8) - ((dig_H1 << 56) + (dig_H2 << 32) + (dig_H3 << 24) +
            (dig_H4 << 12)); 
    dig_H6 = (recv_humi) - ((dig_H1 << 64) + (dig_H2 << 40) + (dig_H3 << 32) +
            (dig_H4 << 20) + (dig_H5 << 8));
    
    BME280_U32_t humi = BME280_Compensate_H_int32(adc_H);
    
    return (unsigned int) humi;

}

//********************************************************************
BME280_S32_t BME280_Compensate_T_int32(BME280_S32_t adc_T){
//********************************************************************
    
    BME280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) – ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) – ((BME280_S32_t)dig_T1)) * ((adc_T>>4) – ((BME280_S32_t)dig_T1)))
        >> 12) * ((BME280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    
    return T;
}

//********************************************************************
BME280_U32_t BME280_Compensate_P_int64(BME280_S32_t adc_P){
//********************************************************************

    BME280_S64_t var1, var2, p;
    var1 = ((BME280_S64_t)t_fine) – 128000;
    var2 = var1 * var1 * (BME280_S64_t)dig_P6;
    var2 = var2 + ((var1*(BME280_S64_t)dig_P5)<<17);
    var2 = var2 + (((BME280_S64_t)dig_P4)<<35);
    var1 = ((var1 * var1 * (BME280_S64_t)dig_P3)>>8) + ((var1 * (BME280_S64_t)dig_P2)<<12);
    var1 = (((((BME280_S64_t)1)<<47)+var1))*((BME280_S64_t)dig_P1)>>33;
    if (var1 == 0)
    {
    return 0; // avoid exception caused by division by zero
    }
    p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((BME280_S64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((BME280_S64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((BME280_S64_t)dig_P7)<<4);
    
    return (BME280_U32_t)p;

}

//********************************************************************
BME280_U32_t BME280_Compensate_H_int32(BME280_S32_t adc_H){
//********************************************************************

    BME280_S32_t v_x1_u32r;

    v_x1_u32r = (t_fine – ((BME280_S32_t)76800));
    v_x1_u32r = (((((adc_H << 14) – (((BME280_S32_t)dig_H4) << 20) – (((BME280_S32_t)dig_H5) *
        v_x1_u32r)) + ((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r *
        ((BME280_S32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)dig_H3)) >> 11) +
        ((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)dig_H2) +
        8192) >> 14));
    v_x1_u32r = (v_x1_u32r – (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
        ((BME280_S32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

return (BME280_U32_t)(v_x1_u32r>>12);
}

