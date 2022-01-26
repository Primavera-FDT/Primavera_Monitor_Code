#ifndef __PRIMAVERA_ROT_SPEED_H__
#define __PRIMAVERA_ROT_SPEED_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdint.h>

extern QueueHandle_t rotational_speed_queue;

extern SemaphoreHandle_t rot_speed_sema;

void vRotSpeed(void *pvParameters);

#endif
