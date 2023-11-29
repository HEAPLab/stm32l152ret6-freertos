/*
TODO
- Task notificaitons    (used by stream and message buffers)
- Stream buffers        (used by message buffers)
- Message buffers
- Event groups          (not required)
- Timers
x Queue sets            (DISABLED)
x 
*/
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "test.h"
#include "task.h"
#include "message_buffer.h"
#include "timers.h"

// BUFFERS
MessageBufferHandle_t xMessageBuffer = NULL;
/**
 * Creates a MessageBuffer, performs a
 * blocking write and deletes the buffer
 * at completion.
*/
void vTaskBufferTestSend ( void * pvParameters ) {

    for ( ;; ) {
        // When a message is written to the message buffer an additional sizeof( size_t ) bytes are also written to store the message's length. sizeof( size_t ) is typically 4 bytes on a 32-bit architecture, so on most 32-bit architectures a 10 byte message will take up 14 bytes of message buffer space.
        vTaskDelay(5);
        xMessageBuffer = xMessageBufferCreate(8); // we allow space for an int and the size_t 
        int spaceAvailable = xMessageBufferSpacesAvailable(xMessageBuffer);
        int isEmpty = xMessageBufferIsEmpty(xMessageBuffer); // should be pdTRUE

        xMessageBufferSend(xMessageBuffer, &spaceAvailable, sizeof(int), 0);

        while (xMessageBufferIsFull(xMessageBuffer) == pdTRUE) { // wait for the other task to read the buffer
            vTaskDelay(5);
        }

        vMessageBufferDelete(xMessageBuffer);

        if (spaceAvailable != 8 && isEmpty != 0) {
            Incorrect_Result();
        }
    }
}


void vTaskBufferTestReceive ( void * pvParameters ) {

    for ( ;; ) {
        while (xMessageBuffer == NULL) {
            vTaskDelay(5);
        }

        int data;
        xMessageBufferReceive(xMessageBuffer, &data, sizeof(int), pdMS_TO_TICKS(1000));

        //xMessageBufferReset(xMessageBuffer);
        while (xMessageBuffer != NULL) { // wait for the other task to delete it
            vTaskDelay(5);
        }

        if (data != 8) {
            Incorrect_Result();
        }
    }
}

// TIMERS
void vTimerCallback( TimerHandle_t xTimer ) {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

#if configUSE_TIMERS == 1
void vTaskTimerTest ( void * pvParameters ) {
    int isDeleted = pdPASS;

    TimerHandle_t xTimer;

    int period;

    for ( ;; ) {
        if (isDeleted == pdPASS) {
            xTimer = xTimerCreate
                    ( /* Just a text name, not used by the RTOS
                        kernel. */
                        "Timer",
                        /* The timer period in ticks, must be
                        greater than 0. */
                        100,
                        /* The timers will auto-reload themselves
                        when they expire. */
                        pdFALSE,
                        /* The ID is used to store a count of the
                        number of times the timer has expired, which
                        is initialised to 0. */
                        ( void * ) 0,
                        /* Each timer calls the same callback when
                        it expires. */
                        vTimerCallback
                    );

            period = xTimerGetPeriod(xTimer);
        }
        while(xTimerIsTimerActive(xTimer) != pdFALSE) {
            vTaskDelay(5);
        }
        vTaskDelay(5);

        int periodChanged = xTimerChangePeriod(xTimer, 200, 1000);
        vTimerSetReloadMode(xTimer, pdTRUE);

        int reloadMode = uxTimerGetReloadMode(xTimer);

        if (period != 100) {
            if (periodChanged == pdPASS) {
                for (int i = 0; i<100; i++) {
                    if (xTimerGetPeriod(xTimer) == 200) {
                        continue;
                    }
                    vTaskDelay(5);
                }
            }
            else {
                Incorrect_Result();
            }
        }

        isDeleted = xTimerDelete(xTimer, 1000);
        if (isDeleted == pdFAIL) {
            Incorrect_Result();
        }
    }
}
#endif