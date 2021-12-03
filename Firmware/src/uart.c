#include "uart.h"

char send_buffer[256];
char receive_buffer[64];

void vUartSend(void *pvParameters) {

    while(1) {

        // Get packet

        // Construct string
        
        // Calculate CRC
        
        // Mutex

        // Send buffer
    
        // Mutex
    }

    vTaskDelete( NULL );

}

void vUartReceive(void *pvParameters) {

    while(1) {

        // Mutex
        // Check for message
        // Mutex
        
        // Check CRC

        // Send command to main

    }

    vTaskDelete( NULL );

}
