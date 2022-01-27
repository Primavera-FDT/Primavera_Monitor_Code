#ifndef __PRIMAVERA_UART_H__
#define __PRIMAVERA_UART_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "mpu6050.h"
#include "rtc.h"

extern QueueHandle_t user_commands;
extern QueueHandle_t uart_queue;
extern SemaphoreHandle_t uart_peripheral;
extern SemaphoreHandle_t uart_receive;
extern SemaphoreHandle_t uart_send_atomic;

enum User_Command {
    ERROR,
    INVALID_COMMAND,
    SEND,
    STREAM_START,
    STREAM_STOP,
    STATUS,
    LOG,
    EJECT_SD,
    MOUNT_SD,
    TIME_SET
};

struct User_Command_Message {
    enum User_Command command;
};

struct Uart_Data {
    char data[10];
    uint8_t len;
};

struct Sensor_Data {
    uint16_t pressure;
    uint16_t temperature;
    uint8_t humidity;
    uint16_t rotational_speed;
    struct MPU6050_Data shock_1;
    struct MPU6050_Data shock_2;
    struct Date date;
};

enum Uart_Receive_State {
    UART_START,
    UART_START_COMMAND,
    UART_SIMPLE_COMMAND_NL,
    UART_SIMPLE_COMMAND_CR,
};

void Uart_Claim_Atomic();
void Uart_Claim_Release();

void Uart_Send(char *data, uint8_t len);
void Uart_Send_Atomic(char *data, uint8_t len);

char Read_Char();
int Write_Char(char c);

void UART_Init();

void vUARTReceive(void *pvParameters);
void vUARTSend(void *pvParameters);

#endif
