#include "i2c.h"
#include "uart.h"

SemaphoreHandle_t I2C_Atomic_Bus_1;
SemaphoreHandle_t I2C_Atomic_Bus_2;
QueueHandle_t I2C_Bus_1_Write_Queue;
QueueHandle_t I2C_Bus_1_Read_Queue;
QueueHandle_t I2C_Bus_2_Write_Queue;
QueueHandle_t I2C_Bus_2_Read_Queue;

int I2C_Read( uint8_t bus,
              struct I2C_Address address,
              uint8_t reg,
              uint8_t *data ) {

    QueueHandle_t write_queue;
    QueueHandle_t read_queue;
    switch(bus) {
        case 0:
            write_queue = I2C_Bus_1_Write_Queue;
            read_queue = I2C_Bus_1_Read_Queue;
            break;
        case 1:
            write_queue = I2C_Bus_2_Write_Queue;
            read_queue = I2C_Bus_2_Read_Queue;
            break;
    }

    struct I2C_To_Task_Msg msg = {
        .address = address,
        .data = {0,0,0,0,0},
        .length = 1,
        .reg = reg,
        .type = I2C_READ
    };

    xQueueSend( write_queue, &msg, portMAX_DELAY );

    struct I2C_From_Task_Msg I2C_Data;

    xQueueReceive( read_queue, &I2C_Data, portMAX_DELAY );

    *data = I2C_Data.data[0];

    return 0;
}

int I2C_Burst_Read( uint8_t bus,
                    struct I2C_Address address,
                    uint8_t reg,
                    uint8_t *data,
                    uint8_t *length ) {

    QueueHandle_t write_queue;
    QueueHandle_t read_queue;
    switch(bus) {
        case 0:
            write_queue = I2C_Bus_1_Write_Queue;
            read_queue = I2C_Bus_1_Read_Queue;
            break;
        case 1:
            write_queue = I2C_Bus_2_Write_Queue;
            read_queue = I2C_Bus_2_Read_Queue;
            break;
    }

    struct I2C_To_Task_Msg msg = {
        .address = address,
        .data = {0,0,0,0,0},
        .length = *length,
        .reg = reg,
        .type = I2C_BURST_READ
    };

    xQueueSend( write_queue, &msg, portMAX_DELAY );

    struct I2C_From_Task_Msg I2C_Data;
    uint8_t total_len = 0;
    uint8_t error = 0;
    while(!error) {

        xQueueReceive( read_queue, &I2C_Data, portMAX_DELAY );

        for(int i=0; i<I2C_Data.length; i++) {
            data[total_len] = I2C_Data.data[i];
            total_len++;
        }

        if(total_len == *length) {
            *length = total_len;
            return 0;
        }
    }

    return -1;
}

int I2C_Write( uint8_t bus,
               struct I2C_Address address,
               uint8_t reg,
               uint8_t data ) {

    QueueHandle_t write_queue;
    switch(bus) {
        case 0:
            write_queue = I2C_Bus_1_Write_Queue;
            break;
        case 1:
            write_queue = I2C_Bus_2_Write_Queue;
            break;
    }

    struct I2C_To_Task_Msg msg = {
        .address = address,
        .data = {data,0,0,0,0},
        .length = 1,
        .reg = reg,
        .type = I2C_WRITE
    };

    xQueueSend( write_queue, &msg, portMAX_DELAY );

    return 0;
}

void vI2C_Bus_1( void *pvParameters ) {

    // Initialize Bus
    EUSCI_B0->CTLW0 |= ( 3 << 9 );
    EUSCI_B0->CTLW0 &= ~( 1 << 15 );
    EUSCI_B0->CTLW0 &= ~( 1 << 13 );
    EUSCI_B0->CTLW0 |= ( 1 << 11 );
    EUSCI_B0->CTLW0 |= ( 3 << 6 );
    EUSCI_B0->BRW = 60;

    EUSCI_B0->CTLW0 &= ~1;

    while(1) {

        struct I2C_To_Task_Msg msg;
        struct I2C_From_Task_Msg data_msg;
        uint8_t len = 0;
        uint8_t counter = 0;
        data_msg.address = msg.address;
        xQueueReceive( I2C_Bus_1_Write_Queue, &msg, portMAX_DELAY );

        // Initialize write address
        EUSCI_B0->I2CSA = msg.address.write_address;
        EUSCI_B0->CTLW0 &= ~( 1 << 14 );
        EUSCI_B0->CTLW0 |= ( 1 << 4 );
        // Generate start
        EUSCI_B0->CTLW0 |= ( 1 << 1 );
        // Write first register address
        while( !(EUSCI_B0->IFG & ( 1 << 1 )) );
        EUSCI_B0->TXBUF = msg.reg;

        switch( msg.type ) {
            case I2C_READ:

                EUSCI_B0->I2CSA = msg.address.read_address;
                EUSCI_B0->CTLW0 &= ~( 1 << 14 );
                EUSCI_B0->CTLW0 &= ~( 1 << 4 );

                EUSCI_B0->CTLW0 |= ( 1 << 1 );

                while( !(EUSCI_B0->IFG & 1) );

                data_msg.data[0] = EUSCI_B0->RXBUF;
                data_msg.length = 1;

                EUSCI_B0->CTLW0 |= ( 1 << 2 );
                xQueueSend( I2C_Bus_1_Read_Queue, &data_msg, portMAX_DELAY );

                break;
            case I2C_BURST_READ:

                EUSCI_B0->I2CSA = msg.address.write_address;
                EUSCI_B0->CTLW0 &= ~( 1 << 14 );
                EUSCI_B0->CTLW0 &= ~( 1 << 4 );

                EUSCI_B0->CTLW0 |= ( 1 << 1 );

                while( len != msg.length ) {

                    while( !(EUSCI_B0->IFG & 1) );

                    data_msg.data[counter] = EUSCI_B0->RXBUF;
                    counter++;
                    len++;

                    if ( counter == 5 ) {
                        data_msg.length = counter;
                        xQueueSend( I2C_Bus_1_Read_Queue, &data_msg, portMAX_DELAY );
                        counter = 0;
                    }
                }

                EUSCI_B0->CTLW0 |= ( 1 << 2 );
                data_msg.length = counter;
                xQueueSend( I2C_Bus_1_Read_Queue, &data_msg, portMAX_DELAY );
                break;
            case I2C_WRITE:
                while( !(EUSCI_B0->IFG & (1 << 1 )) );
                EUSCI_B0->TXBUF = msg.data[0];
                EUSCI_B0->CTLW0 |= ( 1 << 2 );
                break;
        }

    }

    vTaskDelete( NULL );
};

void vI2C_Bus_2( void *pvParameters ) {

    // Initialize Bus
    
    ///P6->DIR &= ~(( 1 << 5 ) | ( 1 << 4 ));
    //P6->SEL0 |= (( 1 << 5 ) | ( 1 << 4 ));
    //P6->SEL1 &= ~(( 1 << 5 ) | ( 1 << 4 ));
    EUSCI_B1->CTLW0 |= ( 3 << 9 );
    EUSCI_B1->CTLW0 &= ~( 1 << 15 );
    EUSCI_B1->CTLW0 &= ~( 1 << 13 );
    EUSCI_B1->CTLW0 |= ( 1 << 11 );
    EUSCI_B1->CTLW0 |= ( 3 << 6 );
    EUSCI_B1->BRW = 60;

    EUSCI_B1->CTLW0 &= ~1;

    while(1) {

        struct I2C_To_Task_Msg msg;
        struct I2C_From_Task_Msg data_msg;
        uint8_t len = 0;
        uint8_t counter = 0;
        data_msg.address = msg.address;
        xQueueReceive( I2C_Bus_2_Write_Queue, &msg, portMAX_DELAY );

        // Initialize write address
        EUSCI_B1->I2CSA = msg.address.write_address;
        EUSCI_B1->CTLW0 &= ~( 1 << 14 );
        EUSCI_B1->CTLW0 |= ( 1 << 4 );
        // Generate start
        EUSCI_B1->CTLW0 |= ( 1 << 1 );

        if( EUSCI_B1->IFG & ( 1 << 5 )) {
            Uart_Send("error\n\r", 7 );
        }
        // Write first register address
        Uart_Send("Started I2C trans\n\r", 19);
        while( !(EUSCI_B1->IFG & ( 1 << 1 )) );
        EUSCI_B1->TXBUF = msg.reg;
        while( !(EUSCI_B1->IFG & ( 1 << 1 )) );
        Uart_Send("Written register\n\r", 18 );

        if( EUSCI_B1->IFG & ( 1 << 5 )) {
            Uart_Send("error\n\r", 7 );
        }

        switch( msg.type ) {
            case I2C_READ:

                EUSCI_B1->I2CSA = msg.address.read_address;
                EUSCI_B1->CTLW0 &= ~( 1 << 14 );
                EUSCI_B1->CTLW0 &= ~( 1 << 4 );

                EUSCI_B1->CTLW0 |= ( 1 << 1 );
                Uart_Send("Change to read\n\r", 16 ); 

                if( EUSCI_B1->IFG & ( 1 << 5 )) {
                    Uart_Send("error\n\r", 7 );
                }
                while( !(EUSCI_B1->IFG & 1) );

                data_msg.data[0] = EUSCI_B1->RXBUF;
                data_msg.length = 1;

                Uart_Send("Received byte\n\r", 15 ); 

                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                xQueueSend( I2C_Bus_2_Read_Queue, &data_msg, portMAX_DELAY );

                break;
            case I2C_BURST_READ:

                EUSCI_B1->I2CSA = msg.address.write_address;
                EUSCI_B1->CTLW0 &= ~( 1 << 14 );
                EUSCI_B1->CTLW0 &= ~( 1 << 4 );

                EUSCI_B1->CTLW0 |= ( 1 << 1 );

                while( len != msg.length ) {

                    while( !(EUSCI_B1->IFG & 1) );

                    data_msg.data[counter] = EUSCI_B1->RXBUF;
                    counter++;
                    len++;

                    if ( counter == 5 ) {
                        data_msg.length = counter;
                        xQueueSend( I2C_Bus_2_Read_Queue, &data_msg, portMAX_DELAY );
                        counter = 0;
                    }
                }

                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                data_msg.length = counter;
                xQueueSend( I2C_Bus_2_Read_Queue, &data_msg, portMAX_DELAY );
                break;
            case I2C_WRITE:
                while( !(EUSCI_B1->IFG & (1 << 1 )) );
                EUSCI_B1->TXBUF = msg.data[0];
                EUSCI_B1->CTLW0 |= ( 1 << 2 );
                break;
        }

    }

    vTaskDelete( NULL );
};
