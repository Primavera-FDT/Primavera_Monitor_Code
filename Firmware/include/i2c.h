#ifndef __PRIMAVERA_MONITOR_I2C_H__
#define __PRIMAVERA_MONITOR_I2C_H__

#include "queue.h"
#include "sempr.h"
#include <stdint.h>

// Globals

QueueHandle_t Queue_I2C_1_TT
QueueHandle_t Queue_I2C_1_FT
QueueHandle_t Queue_I2C_2_TT
QueueHandle_t Queue_I2C_2_FT

SemaphoreHandle_t UCB_1
SemaphoreHnalde_t UCB_2

// Public Facing API's

/* Enum I2C_Bus
 *
 *  Parameters:
 *      - I2C_BUS_1 : Used to notify the first I2C Bus.
 *      - I2C_BUS_2 : Used to notify the second I2C Bus.
 */
typedef enum {
    I2C_BUS_1,
    I2C_BUS_2
} I2C_Bus;

/* Structure I2C_Client
 *
 * Parameters:
 *  - read_address (uint16_t) : The address the master can use to read
 *  data, usually offset by a single bit from the write address.
 *  - write_address (uint16_t) : The address the master can use to write data
 *  to the device.
 *
 * Note: Although most addresses are 7-bit, the addressses are larger to
 * accomodate 10-bit addresses.
 *
 * TODO: Add config options for multi-byte read/write and special device
 * considerations.
 */
struct I2C_Client {
    uint16_t read_address;
    uint16_t write_address;
    enum I2C_Bus bus;
};

/*  Function I2C_Send
 *
 *  Parameters:
 *      - client (struct I2C_Client) : The device the users wants to write data
 *      to.
 *      - msg (unsigned char *) : A pointer to a msg that needs to be send.
 *      the length of msg is determined by msg_len. It cannot be more than 20 bytes
 *      in length.
 *      - msg_len (uint8_t) : The length of the message, if it is larger than 20
 *      it will be truncated to 20.
 *
 *  Returns:
 *      - positive integer (id) on success
 *      - negative integer (error_code) on failure
 *
 */
int I2C_Send( struct I2C_Client client, unsigned char *msg, uint8_t msg_len );

/* Function I2C_Read
 *
 *  Parameters:
 *      - bus (I2C_Bus) : Determines the I2C bus being used.
 *      - client (struct I2C_Client) : The device the users want to read data
 *      from.
 *      - msg (unsigned char *) : This is the buffer where the message from the
 *      device is written into, msg_len determines the length of this message.
 *      It is up to the implementer to ensure that the msg buffer can hold
 *      enough data that is send. The device will at most send 20 bytes at
 *      a time.
 *      - msg_len (uint8_t *) : This is a pointer to an uint8_t where the
 *      function will write the length of the message to. If the integer
 *      msg_len is pointing to does not equal 0, The I2C_peripheral will read
 *      upto msg_len of bytes
 *
 *  Returns:
 *      - 0 on success
 *      - error_code on failure
 */
int I2C_Read( struct I2C_Client client, int id,  unsigned char * msg, uint8_t *msg_len );

/* Task I2C_Bus_1_Peripheral
 *
 *  Parameters:
 *      - pvparameters (void *) : Is not used for this task.
 *
 *  Returns: Nothing
 *
 */
void I2C_Bus_1_Peripheral(void *pvparameters);

/* Task I2C_Bus_2 Peripheral
 *
 *  Parameters:
 *      - pvparameters (void *) : Is not used for this task.
 *
 *  Returns: Nothing
 *
 */
void I2C_Bus_2_Peripheral(void *pvparameters);

// Internal API

uint8_t ids[10] = {0,0,0,0,0,0,0,0,0,0};

int I2C_Get_ID(void);
void I2C_Free_ID(int id);

struct I2C_Msg_To_Handler {
    struct I2C_Client client;
    int id;
    unsigned char msg[20];
    uint8_t msg_len;
};

struct I2C_Msg_From_Handler {
    int id;
    unsigned char msg[20];
    uint8_t msg_len;
};

#endif
