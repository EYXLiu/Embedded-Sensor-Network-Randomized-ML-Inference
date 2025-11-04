#include "ml_model.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

static void MLModel(void *pvParameters) {
    QueueHandle_t queue = pvParameters;
    MLPacket packet;

    for (;;) {
        if (xQueueReceive(queue, &packet, portMAX_DELAY) == pdTRUE) {
            float prediction = packet.avg_reading * 1.05f;
            printf("[ML] Time %u ms: Predicted = %.2f\n", packet.timestamp_ms, prediction);
        }
    }
}

void MLModel_Create(QueueHandle_t mlQueue) {
    xTaskCreate(MLModel, "MLTASK", configMINIMAL_STACK_SIZE*2, mlQueue, 2, NULL);
}
