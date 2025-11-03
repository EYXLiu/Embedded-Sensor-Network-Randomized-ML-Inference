#include "base_station.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

static void BaseStationTask(void *pvParameters) {
    struct {
        QueueHandle_t sensorQueue;
    } *params = pvParameters;

    SensorPacket sensorPacket;

    for (;;) {
        if (xQueueReceive(params->sensorQueue, &sensorPacket, portMAX_DELAY) == pdTRUE) {
            printf("%f\n", sensorPacket.reading);
        }
    }
}

void BaseStationTask_Create(QueueHandle_t sensorQueue) {
    static struct {
        QueueHandle_t sensorQueue;
    } params;
    params.sensorQueue = sensorQueue;

    xTaskCreate(BaseStationTask, "BaseStation", configMINIMAL_STACK_SIZE, &params, 3, NULL);
}
