#ifndef __PRIMAVERA_H__
#define __PRIMAVERA_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "uart.h"

#include "bme280.h"
#include "mpu6050.h"
#include "rtc.h"
#include "rot_speed.h"

#include <stdint.h>
#include <stddef.h>

extern SemaphoreHandle_t streamer_sema;

void vStreamer(void *pvParameters);


#endif
