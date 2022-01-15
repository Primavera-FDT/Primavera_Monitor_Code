#ifndef __PRIMAVERA_MONITOR_I2C_H__
#define __PRIMAVERA_MONITOR_I2C_H__

#include <stdint.h>

typedef enum {
    I2C_BUS_1,
    I2C_BUS_2
} I2C_Bus;

struct I2C_Address {
    uint8_t read_address;
    uint8_t write_address;
};

typedef unsigned char I2C_Message[20];

struct I2C_MsgPack {
    uint8_t id;
    struct I2C_Address address;
    I2C_Message msg;
    uint8_t msglen;
};

uint8_t I2C_Send(I2C_Bus bus, struct I2C_Address addr, I2C_Message &msg, uint8_t len);
void I2C_Receive(I2C_Bus bus, uint8_t id, struct I2C_Address addr, I2C_Message &msg, uint8_t *len);

void I2C_Peripheral(void *pvparameters);

#endif
