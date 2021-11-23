#include "FreeRTOS.h"
#include "task.h"
#include "controller.h"

int main(void) {

    // Create the main controller task.
    xTaskCreate(
            vController,
            "Controller",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL);

    // Start the scheduler
    vTaskStartScheduler();

}
