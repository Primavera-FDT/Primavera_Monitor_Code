#include "i2c.h"
#include "uart.h"

SemaphoreHandle_t I2C_Atomic_Bus_1;
SemaphoreHandle_t I2C_Atomic_Bus_2;
QueueHandle_t I2C_Bus_1_Write_Queue;
QueueHandle_t I2C_Bus_1_Read_Queue;
QueueHandle_t I2C_Bus_2_Write_Queue;
QueueHandle_t I2C_Bus_2_Read_Queue;

void I2C_Claim_Atomic(uint8_t bus) {
    switch(bus) {
        case 0:
            xSemaphoreTake( I2C_Atomic_Bus_1, portMAX_DELAY );
            break;
        case 1:
            xSemaphoreTake( I2C_Atomic_Bus_2, portMAX_DELAY );
            break;
    }
}

void I2C_Release_Atomic(uint8_t bus) {
    switch(bus) {
        case 0:
            xSemaphoreGive( I2C_Atomic_Bus_1 );
            break;
        case 1:
            xSemaphoreGive( I2C_Atomic_Bus_2 );
            break;
    }
}

void I2C_Write( uint8_t bus,
                struct I2C_Address address,
                enum I2C_Command command,
                uint8_t byte ) {

    struct I2C_Message msg;

    switch(bus) {
        case 0:
            xSemaphoreTake( I2C_Atomic_Bus_1, portMAX_DELAY );

            msg.address = address;
            msg.command = command;
            msg.byte = byte;

            xQueueSend( I2C_Bus_1_Write_Queue, &msg, portMAX_DELAY );

            xSemaphoreGive( I2C_Atomic_Bus_1 );
            break;
        case 1:
            xSemaphoreTake( I2C_Atomic_Bus_2, portMAX_DELAY );

            msg.address = address;
            msg.command = command;
            msg.byte = byte;

            xQueueSend( I2C_Bus_2_Write_Queue, &msg, portMAX_DELAY );

            xSemaphoreGive( I2C_Atomic_Bus_2 );
            break;
    }

}

void I2C_Atomic_Write( uint8_t bus,
                       struct I2C_Address address,
                       enum I2C_Command command,
                       uint8_t byte ) {
    struct I2C_Message msg;

    switch(bus) {
        case 0:
            msg.address = address;
            msg.command = command;
            msg.byte = byte;

            xQueueSend( I2C_Bus_1_Write_Queue, &msg, portMAX_DELAY );
            break;
        case 1:
            msg.address = address;
            msg.command = command;
            msg.byte = byte;

            xQueueSend( I2C_Bus_2_Write_Queue, &msg, portMAX_DELAY );
            break;
    }
}

void I2C_Read( uint8_t bus,
               struct I2C_Address address,
               uint8_t *bytes,
               uint8_t *len ) {
    struct I2C_Payload msg;

    switch(bus) {
        case 0:
            xQueuePeek( I2C_Bus_1_Read_Queue, &msg, portMAX_DELAY );
            if(msg.address.read_address == address.read_address) {
                xQueueReceive( I2C_Bus_1_Read_Queue, &msg, portMAX_DELAY );
                for(int i=0; i<msg.len; i++) {
                    bytes[i] = msg.data[i];
                }
                *len = msg.len;
            } else {
                portYIELD();
            }
            break;
        case 1:
            xQueuePeek( I2C_Bus_2_Read_Queue, &msg, portMAX_DELAY );
            if(msg.address.read_address == address.read_address) {
                xQueueReceive( I2C_Bus_2_Read_Queue, &msg, portMAX_DELAY );
                for(int i=0; i<msg.len; i++) {
                    bytes[i] = msg.data[i];
                }
                *len = msg.len;
            } else {
                portYIELD();
            }
            break;
    }
}

struct Uart_Data start = { .data = "start\n\r", .len = 7 };
struct Uart_Data write_1 = { .data = "w 1\n\r", .len = 5 };
struct Uart_Data write_2 = { .data = "w 2\n\r", .len = 5 };
struct Uart_Data readwrite_1 = { .data = "rw 1\n\r", .len = 6 };
struct Uart_Data readwrite_2 = { .data = "rw 2\n\r", .len = 6 };
struct Uart_Data readwrite_3 = { .data = "rw 3\n\r", .len = 6 };
struct Uart_Data stop = { .data = "stp\n\r", .len = 5 };
struct Uart_Data Error = { .data = "err\n\r", .len = 5 };

void vI2C_Bus_1( void *pvParameters ) {

    // Initialize bus

    enum I2C_State state = I2C_START;
    struct I2C_Message msg;
    uint8_t buffer = 0;
    uint8_t count = 0;
    struct I2C_Payload payload;

    while(1) {
        switch(state) {
            case I2C_STOP_STATE:
                xQueueSend( uart_queue, &stop, portMAX_DELAY );
                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                state = I2C_START;
                break;
            case I2C_ERROR_STATE:
                xQueueSend( uart_queue, &Error, portMAX_DELAY );
                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                state = I2C_START;
                break;
            case I2C_START:
                xQueueReceive( I2C_Bus_1_Write_Queue, &msg, portMAX_DELAY );
                xQueueSend( uart_queue, &start, portMAX_DELAY );
                if( msg.command == I2C_WRITE ) {
                    state = I2C_WRITE_1_STATE;
                } else {
                    state = I2C_ERROR_STATE;
                }
                break;
            case I2C_WRITE_1_STATE:
                xQueueSend( uart_queue, &write_1, portMAX_DELAY );
                EUSCI_B1->I2CSA = msg.address.write_address;
                if( msg.address.width == 10 ) {
                    EUSCI_B1->CTLW0 |= ( 1 << 14 );
                } else {
                    EUSCI_B1->CTLW0 &= ~( 1 << 14 );
                }
                EUSCI_B1->CTLW0 |= ( 1 << 4 );
                EUSCI_B1->CTLW0 |= ( 1 << 1 );
                while( !(EUSCI_B1->IFG & ( 1 << 1 )));
                EUSCI_B1->TXBUF = msg.byte;
                state = I2C_WRITE_2_STATE;
            case I2C_WRITE_2_STATE:
                xQueueSend( uart_queue, &write_2, portMAX_DELAY );
                xQueueReceive( I2C_Bus_1_Write_Queue, &msg, portMAX_DELAY );
                if( msg.command == I2C_WRITE_COND ) {
                    while( !(EUSCI_B1->IFG & ( 1 << 1 )));
                    EUSCI_B1->TXBUF = msg.byte;
                    state = I2C_WRITE_2_STATE;
                } else if ( msg.command == I2C_STOP ) {
                    state = I2C_STOP_STATE;
                } else if ( msg.command == I2C_READ ) {
                    state = I2C_READ_AFTER_WRITE_1;
                } else {
                    state = I2C_ERROR_STATE;
                }
                break;
            case I2C_READ_AFTER_WRITE_1:
                xQueueSend( uart_queue, &readwrite_1, portMAX_DELAY );
                EUSCI_B1->I2CSA = msg.address.read_address;
                if( msg.address.width == 10 ) {
                    EUSCI_B1->CTLW0 |= ( 1 << 14 );
                } else {
                    EUSCI_B1->CTLW0 &= ~( 1 << 14 );
                }
                EUSCI_B1->CTLW0 ^= ~( 1 << 4 );
                EUSCI_B1->CTLW0 |= ( 1 << 1 );
                if( !(EUSCI_B1->CTLW0 & ( 1 << 5 )) ) {
                    state = I2C_READ_AFTER_WRITE_2;
                    count = msg.byte;
                } else {
                    state = I2C_ERROR_STATE;
                }
                break;
            case I2C_READ_AFTER_WRITE_2:
                xQueueSend( uart_queue, &readwrite_2, portMAX_DELAY );
                if( count == 0 ) {
                    state = I2C_READ_AFTER_WRITE_3;
                } else if( buffer == 5 ) {
                    state = I2C_READ_AFTER_WRITE_FULL;
                } else {
                    payload.data[buffer] = EUSCI_B1->RXBUF;
                    count--;
                    buffer++;
                }
                break;
            case I2C_READ_AFTER_WRITE_3:
                xQueueSend( uart_queue, &readwrite_3, portMAX_DELAY );
                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                if ( buffer > 0 ) {
                    payload.len = buffer;
                    xQueueSend( I2C_Bus_1_Read_Queue, &payload, portMAX_DELAY );
                }
                state = I2C_START;
                break;
            case I2C_READ_AFTER_WRITE_FULL:
                payload.len = 5;
                xQueueSend( I2C_Bus_1_Read_Queue, &payload, portMAX_DELAY );
                buffer = 0;
                state = I2C_READ_AFTER_WRITE_2;
                break;
        };
    }

    vTaskDelete( NULL );
}

void vI2C_Bus_2( void *pvParameters ) {

    struct I2C_Message msg;

    while(1) {

        xQueueReceive( I2C_Bus_2_Write_Queue, &msg, portMAX_DELAY );

    }

    vTaskDelete( NULL );
}
