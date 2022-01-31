#ifndef __PRIMAVERA_I2C_H__
#define __PRIMAVERA_I2C_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t I2C_Atomic_Bus_1;
extern SemaphoreHandle_t I2C_Atomic_Bus_2;
extern QueueHandle_t I2C_Bus_1_Write_Queue;
extern QueueHandle_t I2C_Bus_1_Read_Queue;
extern QueueHandle_t I2C_Bus_2_Write_Queue;
extern QueueHandle_t I2C_Bus_2_Read_Queue;

struct I2C_Address {
    uint16_t read_address;
    uint16_t write_address;
    uint8_t width;
};

enum I2C_Message_Type {
    I2C_READ,
    I2C_BURST_READ,
    I2C_WRITE,
    I2C_BURST_WRITE
};

struct I2C_To_Task_Msg {
    struct I2C_Address address;
    uint8_t data[5];
    uint8_t length;
    uint8_t reg;
    enum I2C_Message_Type type;
};

struct I2C_From_Task_Msg {
    struct I2C_Address address;
    uint8_t data[5];
    uint8_t length;
};

int I2C_Read( uint8_t bus,
              struct I2C_Address address,
              uint8_t reg,
              uint8_t *data);

int I2C_Burst_Read( uint8_t bus,
                    struct I2C_Address address,
                    uint8_t reg,
                    uint8_t *data,
                    uint8_t *length );

int I2C_Write( uint8_t bus,
               struct I2C_Address address,
               uint8_t reg,
               uint8_t data );

void vI2C_Bus_1(void *pvParameters );
void vI2C_Bus_2(void *pvParameters );

#endif
