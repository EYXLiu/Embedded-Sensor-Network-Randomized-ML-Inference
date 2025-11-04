#include "sensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void SensorTask(void *pvParameters) {
    SensorParams *params = pvParameters;

    SensorPacket packet;
    packet.node_id= params->node_id;

    for (;;) {
        packet.reading = (rand() % 1000) / 10.0f; //random data for now
        packet.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
        xQueueSend(params->queue, &packet, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000 + rand()%1000));
    }
}

void SensorTask_Create(uint8_t node_id, QueueHandle_t queue) {
    SensorParams *params = malloc(sizeof(SensorParams));
    params->node_id = node_id;
    params->queue = queue;

    char name[16];
    snprintf(name, sizeof(name), "Sensor%u", node_id);
    xTaskCreate(SensorTask, name, configMINIMAL_STACK_SIZE, params, 2, NULL);
}