#include "controller.h"

void vController(void *pvParameters) {

    while(1) {

        // Create Binaries
        unsigned int BinaryLen = sizeof(Binary) / sizeof(Binary[0]);
        for( unsigned int i=0; i<BinaryLen; i++) {
            Binary[i].handle = xSemaphoreCreateBinary();
        }
        
        // Create Counting Binaries
        unsigned int CountingLen = sizeof(Counting) / sizeof(Counting[0]);
        for( unsigned int i=0; i<CountingLen; i++) {
            struct CountingConfig m = Counting[i];
            Counting[i].handle = xSemaphoreCreateCounting(m.MaxCount, m.InitialCount);
        }

        // Create Mutexes
        unsigned int MutexLen = sizeof(Mutex) / sizeof(Mutex[0]);
        for( unsigned int i=0; i<MutexLen; i++) {
            Mutex[i].handle = xSemaphoreCreateMutex();
        }
        
        // Create Queues
        unsigned int QueueLen = sizeof(Queue) / sizeof(Queue[0]);
        for( unsigned int i=0; i<QueueLen; i++) {
            struct QueueConfig m = Queue[i];
            Queue[i].handle = xQueueCreate( m.length, m.size );
        }

        // Start functions

        while(1) {

            // Wait for commands from UART
            switch() {
                case SEND:
                    // SEND command to streamer
                    break;
                default:
                    break;
            }

        }

        // Stop functions
        
        // Destroy Queues
        for( unsigned int i=0; i<QueueLen; i++) {
            vQueueDelete( Queue[i].handle );
        }
        
        // Destroy Mutexes
        for( unsigned int i=0; i<MutexLen; i++) {
            vSemaphoreDelete( Mutex[i].handle );
        }

        // Destroy Counting Binaries
        for( unsigned int i=0; i<CountingLen; i++) {
            vSemaphoreDelete( Counting[i].handle );
        }

        // Destroy Binaries
        for( unsigned int i=0; i<BinaryLen; i++) {
            vSemaphoreDelete( Binary[i].handle );
        }

        // Reset handling

    }

    vTaskDelete( NULL ); // Just for security
}
