#include "controller.h"

#define SECOND_1_DELAY 24000000
#define MILISECOND_20_DELAY 480000

void T32_INT1_IRQHandler(void) {

    BaseType_t xHigherPriorityTaskWoken_1 = pdFALSE;
    BaseType_t xHigherPriorityTaskWoken_2 = pdFALSE;

    xSemaphoreGiveFromISR( shock_1_mutex, &xHigherPriorityTaskWoken_1 );
    xSemaphoreGiveFromISR( shock_2_mutex, &xHigherPriorityTaskWoken_2 );

    TIMER32_1->INTCLR = 1;

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken_1 || xHigherPriorityTaskWoken_2 );

}

void T32_INT2_IRQHandler(void) {

    BaseType_t bme280_woken = pdFALSE;
    BaseType_t streamer_woken = pdFALSE;
    BaseType_t rtc_woken = pdFALSE;
    BaseType_t mpu_woken = pdFALSE;
    BaseType_t rot_speed_woken = pdFALSE;

    xSemaphoreGiveFromISR( bme280_sema, &bme280_woken );
    xSemaphoreGiveFromISR( streamer_sema, &streamer_woken );
    xSemaphoreGiveFromISR( rtc_sema, &rtc_woken );
    xSemaphoreGiveFromISR( mpu_sema, &mpu_woken );
    xSemaphoreGiveFromISR( rot_speed_sema, &rot_speed_woken );

    TIMER32_2->INTCLR = 1;

    BaseType_t result = bme280_woken || streamer_woken || rtc_woken || mpu_woken ||
        rot_speed_woken;

    portYIELD_FROM_ISR( result );
}

void Timers_init(void) {
    TIMER32_2->CONTROL |= ( 1 << 6 );
    TIMER32_2->CONTROL |= ( 1 << 1 );
    TIMER32_2->CONTROL |= ( 1 << 7 );
    TIMER32_2->CONTROL &= ~1;
    TIMER32_2->CONTROL &= ~( 3 << 2 );

    TIMER32_2->LOAD = SECOND_1_DELAY;

    //TIMER32_1->CONTROL |= ( 1 << 6 );
    //TIMER32_1->CONTROL |= ( 1 << 1 );
    //TIMER32_1->CONTROL |= ( 1 << 7 );

    //TIMER32_1->LOAD = MILISECOND_20_DELAY;

    //NVIC_EnableIRQ(T32_INT1_IRQn);
    //NVIC_SetPriority(T32_INT1_IRQn, 6);
    NVIC_EnableIRQ(T32_INT2_IRQn);
    NVIC_SetPriority(T32_INT2_IRQn, 6);

    //TIMER32_1->CONTROL |= ( 1 << 5);
    TIMER32_2->CONTROL |= ( 1 << 5);
}

char *send = "send\n";
char *error = "error\n";

void vController(void *pvParameters) {

    P1->DIR |= 1;
    P1->OUT &= ~1;

    while(1) {

        user_commands = xQueueCreate( 2, sizeof(struct User_Command_Message));
        uart_queue = xQueueCreate( 4, sizeof(struct Uart_Data));
        pressure_queue = xQueueCreate( 1, sizeof(uint16_t));
        temperature_queue = xQueueCreate( 1, sizeof(uint16_t));
        humidity_queue = xQueueCreate( 1, sizeof(uint16_t));
        shock_1_queue = xQueueCreate( 1 , sizeof(struct MPU6050_Data));
        shock_2_queue = xQueueCreate( 1, sizeof(struct MPU6050_Data));
        rotational_speed_queue = xQueueCreate( 1, sizeof(uint16_t));
        date_queue = xQueueCreate( 1, sizeof(struct Date));
        I2C_Bus_1_Write_Queue = xQueueCreate( 5, sizeof(struct I2C_Message));
        I2C_Bus_1_Read_Queue = xQueueCreate( 5, sizeof(struct I2C_Payload));
        I2C_Bus_2_Write_Queue = xQueueCreate( 5, sizeof(struct I2C_Message));
        I2C_Bus_2_Read_Queue = xQueueCreate( 5, sizeof(struct I2C_Payload));

        uart_peripheral = xSemaphoreCreateMutex();
        uart_receive = xSemaphoreCreateBinary();
        uart_send_atomic = xSemaphoreCreateMutex();

        bme280_sema = xSemaphoreCreateBinary();
        mpu_sema = xSemaphoreCreateBinary();

        shock_1_mutex = xSemaphoreCreateBinary();
        shock_2_mutex = xSemaphoreCreateBinary();

        rtc_sema = xSemaphoreCreateBinary();

        streamer_sema = xSemaphoreCreateBinary();

        rot_speed_sema = xSemaphoreCreateBinary();

        I2C_Atomic_Bus_1 = xSemaphoreCreateMutex();
        I2C_Atomic_Bus_2 = xSemaphoreCreateMutex();

        // Start functions
        xTaskCreate( vUARTReceive,
                     "vUARTReceive",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     4,
                     NULL);

        xTaskCreate( vUARTSend,
                     "vUARTSend",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL);

        xTaskCreate( vStreamer,
                     "vStreamer",
                     configMINIMAL_STACK_SIZE + 50,
                     NULL,
                     3,
                     NULL );

        xTaskCreate( vBME280,
                     "vBME280",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );


        xTaskCreate( vMPU6050,
                     "vMPU6050",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );

        xTaskCreate( vRotSpeed,
                     "vRotSpeed",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );

        xTaskCreate( vRTC,
                     "vRTC",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );

        xTaskCreate( vI2C_Bus_1,
                     "vI2C_Bus_1",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );

        xTaskCreate( vI2C_Bus_2,
                     "vI2C_Bus_2",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     3,
                     NULL );

        Timers_init();

        struct User_Command_Message msg;
        while(1) {
            xQueueReceive( user_commands, &msg, portMAX_DELAY );
            switch(msg.command) {
                case SEND:
                    Uart_Send("Send\n\r", 6);
                    break;
                case STREAM_START:
                    Uart_Send("S Start\n\r", 9);
                    break;
                case STREAM_STOP:
                    Uart_Send("S Stop\n\r", 8 );
                    break;
                case LOG:
                    break;
                case STATUS:
                    break;
                case MOUNT_SD:
                    break;
                case EJECT_SD:
                    break;
                case TIME_SET:
                    break;
                case ERROR:
                    Uart_Send("Err\n\r", 5 );
                    break;
                case INVALID_COMMAND:
                    Uart_Send("Invd\n\r", 6 );
                    break;
            }
        }
       
        vSemaphoreDelete( uart_peripheral );
        vSemaphoreDelete( uart_receive );
        vQueueDelete( user_commands );
        vQueueDelete( uart_queue );

    }

    vTaskDelete( NULL ); // Just for security
}

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    xTaskCreate(
            vController,
            "Controller",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL);

    __enable_interrupt();
    vTaskStartScheduler();
}
