#ifndef BASE_STATION_H
#define BASE_STATION_H

#include "FreeRTOS.h"
#include "queue.h"
#include "sensor.h"

typedef struct {
    float avg_reading;
    uint32_t timestamp_ms;
} MLPacket;

void BaseStationTask_Create(QueueHandle_t sensorQueue, QueueHandle_t mlQueue, uint8_t num_sensors, uint32_t lase_seen[]);

#endif