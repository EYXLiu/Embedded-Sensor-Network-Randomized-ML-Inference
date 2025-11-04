#include "base_station.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>
#include "logger.h"

static void BaseStationTask(void *pvParameters) {
    struct {
        QueueHandle_t sensorQueue;
        QueueHandle_t mlQueue;
        uint8_t num_sensors;
        uint32_t *last_seen;
    } *params = pvParameters;

    SensorPacket sensorPacket;
    SensorPacket latest_readings[params->num_sensors];
    bool has_reading[params->num_sensors];

    for (int i = 0; i < params->num_sensors; i++) {
        has_reading[i] = false;
        params->last_seen[i] = 0;
    }

    MLPacket mlPacket;

    for (;;) {
        if (xQueueReceive(params->sensorQueue, &sensorPacket, portMAX_DELAY) == pdTRUE) {
            latest_readings[sensorPacket.node_id - 1] = sensorPacket;
            has_reading[sensorPacket.node_id - 1] = true;
            params->last_seen[sensorPacket.node_id - 1] = xTaskGetTickCount();

            bool all_ready = true;
            for (int i = 0; i < params->num_sensors; i++) {
                if (!has_reading[i]) {
                    all_ready = false;
                    break;
                }
            }

            if (all_ready) {
                mlPacket.num_sensors = params->num_sensors;
                mlPacket.timestamp_ms = sensorPacket.timestamp_ms;
                mlPacket.readings = pvPortMalloc(sizeof(float) * params->num_sensors);
                if (mlPacket.readings == NULL) {
                    for (int i = 0; i < params->num_sensors; i++) {
                        has_reading[i] = false;
                    }
                    Logger_Send("[BS] Failed to create readings, skipping batch");
                    continue;
                }
                for (int i = 0; i < params->num_sensors; i++) {
                    mlPacket.readings[i] = latest_readings[i].reading;
                    has_reading[i] = false;
                }
                xQueueSend(params->mlQueue, &mlPacket, portMAX_DELAY);
            }
        }
    }
}

void BaseStationTask_Create(QueueHandle_t sensorQueue, QueueHandle_t mlQueue, uint8_t num_sensors, uint32_t last_seen[]) {
    static struct {
        QueueHandle_t sensorQueue;
        QueueHandle_t mlQueue;
        uint8_t num_sensors;
        uint32_t *last_seen;
    } params;
    params.sensorQueue = sensorQueue;
    params.mlQueue = mlQueue;
    params.num_sensors = num_sensors;
    params.last_seen = last_seen;

    xTaskCreate(BaseStationTask, "BaseStation", configMINIMAL_STACK_SIZE, &params, 3, NULL);
    Logger_Send("[Stn] Base Station created");
}
