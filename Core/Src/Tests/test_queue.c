/**
 * @file test_queue.c
 * 
 * Functions tested from `queue.h`:
xQueueCreate                   [x]                           
xQueueCreateStatic             [x]                           
vQueueDelete                   [x]                           
xQueueSend                     [x]                           
xQueueSendFromISR              [ ]                           
xQueueSendToBack               [x]                           
xQueueSendToBackFromISR        [ ]                           
xQueueSendToFront              [x]                           
xQueueSendToFrontFromISR       [ ]                           
xQueueReceive                  [x]                           
xQueueReceiveFromISR           [ ]                           
uxQueueMessagesWaiting         [x]                           
uxQueueMessagesWaitingFromISR  [ ]                           
uxQueueSpacesAvailable         [x]                           
xQueueReset                    [x]                           
xQueuePeek                     [x]                           
xQueuePeekFromISR              [ ]                           
vQueueAddToRegistry            [x]                           
pcQueueGetName                 [x]                           
vQueueUnregisterQueue          [x]                           
xQueueIsQueueEmptyFromISR      [ ]                           
xQueueIsQueueFullFromISR       [ ]                           
xQueueOverwrite                [x]                           
xQueueOverwriteFromISR         [ ]   

 * Coverage: all functions except functions "fromISR"
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "test.h"
#include <string.h>

#define QUEUE_LENGTH    4
#define ITEM_SIZE       sizeof( int )

xQueueHandle globalQueuehandle = 0;
static StaticQueue_t xStaticQueue;

/**
 * This function merges the old vTaskQueueTest1 and vTaskQueueTest2:
 * - Creates a queue from a global queue handle
 * - Sends an integer to the queue
 * - Receives a int from the queue
 * - Checks if the results coincide
 * - Deletes the queue
*/
void fnTestQueue1() {
    globalQueuehandle = xQueueCreate(2, ITEM_SIZE);
    // Send number on the queue
    int i = 1;
    xQueueSend(globalQueuehandle, &i, 0);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    i = 0;
    int res = xQueueReceive(globalQueuehandle, &i, 200);
    if (i != 1 || res != pdTRUE) { // we have received the value and it is indeed 1
        Incorrect_Result();
    }
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    vQueueDelete(globalQueuehandle);
}

/**
 * This function replaces the old vTaskQueueTest3:
 * - Creates a static queue of length 4
 * - Fills the queue with [1, 1, 2, 2]
 * - Receives the first two numbers
 * - Checks the third number 
 * - Checks the number of messages waiting
 * - Resets the queue
 * - Checks the values read from the queue
 * - Deletes the queue
*/
void fnTestQueue2() {
    uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];
    QueueHandle_t xQueue;

    /* Create a queue capable of containing 4 int values. */
    xQueue = xQueueCreateStatic( QUEUE_LENGTH,
                                 ITEM_SIZE,
                                 ucQueueStorageArea,
                                 &xStaticQueue );
    int i1 = 1;
    int i2 = 2;
    vTaskDelay(5);
    for (int i = 0; i<2; i++) {
        xQueueSendToBack(xQueue, &i2, 0); // send 2
        xQueueSendToFront(xQueue, &i1, 0); // send 1
    }
    // now the queue should be (front)[1, 1, 1, 1, 1, 2, 2, 2, 2, 2](back)
    int iOut;
    for (int i = 0; i<2; i++) {
        xQueueReceive(xQueue, &iOut, 20);
    }
    int iOutPeekd;
    xQueuePeek(xQueue, &iOutPeekd, 20);
    if (uxQueueMessagesWaiting(xQueue) == 2) {
        xQueueReset(xQueue);
    }
    if(uxQueueSpacesAvailable(xQueue) == 4 && iOut == 1 && iOutPeekd == 2) {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
    else {
        Incorrect_Result();
    }
    vQueueDelete(xQueue);
}

/**
 * This function replaces the old vTaskQueueTest4:
 * - Creates a queue
 * - Adds it to registry
 * - Sends a value on the queue
 * - Checks the name of the queue
 * - Overwrites the value in the queue
 * - Unregisters the queue
 * - Deletes the queue
*/
void fnTestQueue3() {
    xQueueHandle queuehandle = xQueueCreate(1, ITEM_SIZE);

    vQueueAddToRegistry(queuehandle, "testQueue\0");
    vTaskDelay(5);

    int i1 = 1;
    xQueueSend(queuehandle, &i1, 0);

    char* name = pcQueueGetName(queuehandle);

    if (strncmp("testQueue", name, 9) != 0) {
        Incorrect_Result();
    }

    int i2 = 2;
    xQueueOverwrite(queuehandle, &i2);

    vTaskDelay(5);

    vQueueUnregisterQueue(queuehandle);
    vQueueDelete(queuehandle);

    vTaskDelay(5);
}

void vTaskTestQueue( void *pvParameters) {
    for (;;) {
        fnTestQueue1();
        fnTestQueue2();
        fnTestQueue3();
    }
}