#ifndef __PRIMAVERA_CONTROLLER_H__
#define __PRIMAVERA_CONTROLLER_H__

#include "FreeRTOS.h"
#include "task.h"

struct Counting_Semaphore {
    SemaphoreHandle_t Handle;
    UBaseType_t Max_Count;
    UBaseType_t Initial_Count;
};

struct Queue_Config {
    QueueHandle_t Queue;
    unsigned int Elements;
    unsigned int Size;
};

SemaphoreHandle_t Binary[] = {

};

struct Counting_Semaphore Counting[] = {

}:

SemaphoreHandle_t Mutex[] = {

};

struct Queue_Config Queue[] = {

};

// The main controller task
void vController(void *pvParameters);

#endif
