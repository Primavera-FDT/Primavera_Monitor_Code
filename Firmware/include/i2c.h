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

enum I2C_State {
    I2C_STOP_STATE,
    I2C_ERROR_STATE,
    I2C_START,
    I2C_WRITE_1_STATE,
    I2C_WRITE_2_STATE,
    I2C_READ_AFTER_WRITE_1,
    I2C_READ_AFTER_WRITE_2,
    I2C_READ_AFTER_WRITE_3,
    I2C_READ_AFTER_WRITE_FULL
};

enum I2C_Command {
    I2C_WRITE,
    I2C_WRITE_COND,
    I2C_READ,
    I2C_STOP,
};

struct I2C_Address {
    uint16_t write_address;
    uint16_t read_address;
    uint8_t width;
};

struct I2C_Message {
    struct I2C_Address address;
    enum I2C_Command command;
    uint8_t byte;
};

struct I2C_Payload {
    struct I2C_Address address;
    uint8_t data[5];
    uint8_t len;
};

void I2C_Claim_Atomic(uint8_t bus);
void I2C_Release_Atomic(uint8_t bus);

void I2C_Write( uint8_t bus,
                struct I2C_Address address,
                enum I2C_Command command,
                uint8_t byte );

void I2C_Atomic_Write( uint8_t bus,
                       struct I2C_Address address,
                       enum I2C_Command command,
                       uint8_t byte );

void I2C_Read( uint8_t bus,
               struct I2C_Address address,
               uint8_t *bytes,
               uint8_t *len );

void vI2C_Bus_1(void *pvParameters);
void vI2C_Bus_2(void *pvParameters);

#endif
