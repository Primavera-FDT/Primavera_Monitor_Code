#include "i2c.h"

void I2C_Send(I2C_Bus bus, uint8_t address, I2C_Message *msg, uint8_t len) {

    struct I2C_MsgPack msgpack;
    msgpack.address = address;
    msgpack.msg = *msg;
    msgpack.msglen = len;

    switch(bus) {
        case I2C_BUS_1:
            xQueueSend( Queue[11], (void *) &msgpack, portMAX_DELAY);
            break;
        case I2C_BUS_2:
            xQueueSend( Queue[12], (void *) &msgpack, portMAX_DELAY);
            break;
        default:
            // Add error msg
            return;
    }
}

void I2C_Receive(I2C_Bus bus, uint8_t address, I2C_Message *msg, uint8_t *len) {
    struct I2C_MsgPack msgpack;

    switch(bus) {
        case I2C_BUS_1:
            while(1) {
                xQueuePeek( Queue[11], (void *) &msgpack, portMAX_DELAY);
                if (address == msgpack.address) {
                    xQueueReceive( Queue[11], (void *) &msgpack, 0);
                    *msg = address.msg;
                    *len = address.msglen;
                    break;
                } else {
                    taskYIELD();
                }
            }
            break;
        case I2C_BUS_2:
            while(1) {
                xQueuePeek( Queue[12], (void *) &msgpack, portMAX_DELAY);
                if (address == msgpack.address) {
                    xQueueReceive( Queue[12], (void *) &msgpack, 0);
                    *msg = address.msg;
                    *len = address.msglen;
                    break;
                } else {
                    taskYIELD();
                }
            }
            break;
        default:
            // Add error msg
            return;
    }
}
