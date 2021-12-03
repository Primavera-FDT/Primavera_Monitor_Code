#ifndef __PRIMAVERA_UART_H__
#define __PRIMAVERA_UART_H__

SemaphoreHandle_t uart_mutex;
QueueHandle_t uart_queue_send;
QueueHandle_t uart_queue_receive;

struct uart_send_packet {
    uint16_t            temperature;
    uint8_t             humidity;
    uint32_t            pressure;
    uint32_t            rotational_speed;
    uint32_t            mpu1_minimum;
    uint32_t            mpu1_maximum;
    uint32_t            mpu1_mean;
    float               mpu1_stddev;
    uint32_t            mpu2_minimum;
    uint32_t            mpu2_maximum;
    uint32_t            mpu2_mean;
    float               mpu1_stddev;
};

void vUartSend(void *pvParameters);
void vUartReceive(void *pvParameters);

#endif
