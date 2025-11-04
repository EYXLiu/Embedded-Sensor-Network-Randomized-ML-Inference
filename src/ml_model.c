#include "ml_model.h"
#include "FreeRTOS.h"
#include "task.h"
#include "logger.h"
#include <stdio.h>

static void MLModel(void *pvParameters) {
    QueueHandle_t queue = pvParameters;
    MLPacket packet;

    for (;;) {
        if (xQueueReceive(queue, &packet, portMAX_DELAY) == pdTRUE) {
            float sum = 0;
            for (int i = 0; i < packet.num_sensors; i++) {
                sum += packet.readings[i];
            }
            float prediction = sum;
            Logger_Send("[ML] Time %u ms: Predicted = %.2f", packet.timestamp_ms, prediction);
            vPortFree(packet.readings);
            packet.readings = NULL;
        }
    }
}

void MLModel_Create(QueueHandle_t mlQueue) {
    xTaskCreate(MLModel, "MLTASK", configMINIMAL_STACK_SIZE*2, mlQueue, 2, NULL);
    Logger_Send("[ML] ML Model created");
}
