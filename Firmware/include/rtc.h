#ifndef __PRIMAVERA_RTC_H__
#define __PRIMAVERA_RTC_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "i2c.h"

#include <stdint.h>

extern QueueHandle_t date_queue;

extern SemaphoreHandle_t rtc_sema;

struct Date {
    uint16_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
};

void vRTC(void *pvParameters);

#endif
