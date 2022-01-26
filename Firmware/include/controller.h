#ifndef __PRIMAVERA_CONTROLLER_H__
#define __PRIMAVERA_CONTROLLER_H__

#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"
#include "streamer.h"
#include "rtc.h"
#include "rot_speed.h"
#include "mpu6050.h"
#include "bme280.h"

void vController(void *pvParameters);

#endif
