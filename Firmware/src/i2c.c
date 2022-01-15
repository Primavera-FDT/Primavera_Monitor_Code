#include "i2c.h"

void I2C_Bus_1_Peripheral(void *pvparameters) {

    while(1) {

    }

    vTaskDelete( NULL );

}

int I2C_Send( struct I2C_Client client, unsigned char *msg, uint8_t msg_len ) {

    QueueHandle_t queue;
    switch(client.bus) {
        case I2C_BUS_1:
            queue = Queue_I2C_1_TT;
            break;
        case I2C_BUS_2:
            queue = Queue_I2C_2_TT;
            break;
        default:
            return -1;
    }

    int id = I2C_Get_ID();
    if(id <= 0) {
        return -2;
    }

    struct I2C_Msg_To_Handler msg;
    msg.client = client;
    msg.id = id;
    msg.msg_len = msg_len;

    for(int i=0; i<msg_len; i++) {
        msg.msg = msg[i];
    }

    xQueueSend( queue, (void *) &msg, portMAX_DELAY );

    return id;
}

int I2C_Read( struct I2C_Client client, int id, unsigned char *msg, uint8_t *msg_len ) {
    QueueHandle_t send_queue;
    QueueHandle_t recv_queue;
    switch(client.bus) {
        case I2C_BUS_1:
            send_queue = Queue_I2C_1_TT;
            recv_queue = Queue_I2C_1_FT;
            break;
        case I2C_BUS_2:
            send_queue = Queue_I2C_2_TT;
            recv_queue = Queue_I2C_2_FT;
            break;
        default:
            return -1;
    }

    if (id <= 0) {
        return -2;
    }

    struct I2C_Msg_To_Handler msg;
    msg.client = client;
    msg.id = id;
    msg.msg_len = *msg_len;

    xQueueSend( send_queue, (void *) &msg, portMAX_DELAY );

    struct I2C_Msg_From_Handler handler_msg;
    while(1) {
        xQueuePeek( recv_queue, (void *) &handler_msg, portMAX_DELAY );
        if ( handler_msg.id == id ) {
            xQueueReceive( recv_queue, (void *) &handler_msg, portMAX_DELAY );

            *msg_len = handler_msg.msg_len;

            for(int i=0; i<handler_msg.msg_len; i++) {
                msg[i] = handler_msg.msg[i];
            }

            I2C_Free_ID(id);
            return 0;
        } else {
            taskYIELD();
        }
    }

}

int I2C_Get_ID(void) {
    for(int i=0; i<10; i++) {
        if(ids[i] == 0) {
            ids[i] = 1;
            return id + 1;
        }
    }

    return 0;
}

void I2C_Free_ID(int id) {
    ids[i-1] = 0;
}
