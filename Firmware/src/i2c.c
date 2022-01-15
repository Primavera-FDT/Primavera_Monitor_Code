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
                xQueuePeek( Queue[13], (void *) &msgpack, portMAX_DELAY);
                if (address == msgpack.address) {
                    xQueueReceive( Queue[13], (void *) &msgpack, 0);
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
                xQueuePeek( Queue[14], (void *) &msgpack, portMAX_DELAY);
                if (address == msgpack.address) {
                    xQueueReceive( Queue[14], (void *) &msgpack, 0);
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

void I2C_Bus_1_Peripheral(void *pvparameters) {

    // Set up I2C device
    
    uint8_t active_requests[10];
    uint8_t requests_len = 0;

    while(1) {
        // Check active requests
        if (requests_len > 0 ) {
            unsigned char msg[20];
            uint8_t msglen = 0;

            UCBxI2CSA = active_requests[requests_len - 1];
            UCSLA10 = 7;
            UCTR &= ~1;
            UCTXSTT |= 1;

            while( msglen < 20 ) {
                if (UCRXIFG == 1) {
                    msg[msglen] = UCBxRXBUF;
                    msglen++;
                } else if (UCBSTPIFG  == 1 ) {
                    break;
                }
            }

            struct I2C_MessagePack msgpack;
            msgpack.address = active_requests[requests_len - 1];
            msgpack.msg = msg;
            msgpack.msglen = msglen;

            xQueueSend( Queue[13], (void *) &msgpack, portMAX_DELAY );
            requests_len--;
        } else if( uxQueueMessagesWaiting( Queue[11] ) > 0 ) {
            struct I2C_MessagePack msgpack;
            xQueueReceive( Queue, (void *) &msgpack, portMAX_DELAY );

            UCBxI2CSA = msgpack.address;
            UCSLA10 = 7;
            UCTR |= 1;
            UCTXSTT |= 1;

            for(int i=0; i<msgpack.msglen; i++) {
                while( UCTXIFG0 != 1 );
                UCBxTXBUF = msgpack.msg[i];
            }

            UCTXSTP = 1;

            active_requests[requests_len] = msgpack.address;
            requests_len++;

        } else {
            taskYIELD();
        }
    }

    vTaskDelete( NULL );

}

void I2C_Bus_2_Peripheral(void *pvparameters) {

    // Set up I2C device
    
    uint8_t active_requests[10];
    uint8_t requests_len = 0;

    while(1) {
        // Check active requests
        if (requests_len > 0 ) {
            unsigned char msg[20];
            uint8_t msglen = 0;

            UCBxI2CSA = active_requests[requests_len - 1];
            UCSLA10 = 7;
            UCTR &= ~1;
            UCTXSTT |= 1;

            while( msglen < 20 ) {
                if (UCRXIFG == 1) {
                    msg[msglen] = UCBxRXBUF;
                    msglen++;
                } else if (UCBSTPIFG  == 1 ) {
                    break;
                }
            }

            struct I2C_MessagePack msgpack;
            msgpack.address = active_requests[requests_len - 1];
            msgpack.msg = msg;
            msgpack.msglen = msglen;

            xQueueSend( Queue[14], (void *) &msgpack, portMAX_DELAY );
            requests_len--;
        } else if( uxQueueMessagesWaiting( Queue[12] ) > 0 ) {
            struct I2C_MessagePack msgpack;
            xQueueReceive( Queue, (void *) &msgpack, portMAX_DELAY );

            UCBxI2CSA = msgpack.address;
            UCSLA10 = 7;
            UCTR |= 1;
            UCTXSTT |= 1;

            for(int i=0; i<msgpack.msglen; i++) {
                while( UCTXIFG0 != 1 );
                UCBxTXBUF = msgpack.msg[i];
            }

            UCTXSTP = 1;

            active_requests[requests_len] = msgpack.address;
            requests_len++;

        } else {
            taskYIELD();
        }
    }

    vTaskDelete( NULL );

}
