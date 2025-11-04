#include "base_station.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>

static void BaseStationTask(void *pvParameters) {
    struct {
        QueueHandle_t sensorQueue;
        QueueHandle_t mlQueue;
        uint8_t num_sensors;
    } *params = pvParameters;

    SensorPacket sensorPacket;
    SensorPacket latest_readings[params->num_sensors];
    bool has_reading[params->num_sensors];

    for (int i = 0; i < params->num_sensors; i++) {
        has_reading[i] = false;
    }

    MLPacket mlPacket;

    for (;;) {
        if (xQueueReceive(params->sensorQueue, &sensorPacket, portMAX_DELAY) == pdTRUE) {
            latest_readings[sensorPacket.node_id - 1] = sensorPacket;
            has_reading[sensorPacket.node_id - 1] = true;

            bool all_ready = true;
            for (int i = 0; i < params->num_sensors; i++) {
                if (!has_reading[i]) {
                    all_ready = false;
                    break;
                }
            }

            if (all_ready) {
                float sum = 0;
                for (int i = 0; i < params->num_sensors; i++) {
                    sum += latest_readings[i].reading;
                    has_reading[i] = false;
                }
                mlPacket.avg_reading = sensorPacket.reading;
                mlPacket.timestamp_ms = sensorPacket.timestamp_ms;
                xQueueSend(params->mlQueue, &mlPacket, portMAX_DELAY);
            }
        }
    }
}

void BaseStationTask_Create(QueueHandle_t sensorQueue, QueueHandle_t mlQueue, uint8_t num_sensors) {
    static struct {
        QueueHandle_t sensorQueue;
        QueueHandle_t mlQueue;
        uint8_t num_sensors;
    } params;
    params.sensorQueue = sensorQueue;
    params.mlQueue = mlQueue;
    params.num_sensors = num_sensors;

    xTaskCreate(BaseStationTask, "BaseStation", configMINIMAL_STACK_SIZE, &params, 3, NULL);
}
