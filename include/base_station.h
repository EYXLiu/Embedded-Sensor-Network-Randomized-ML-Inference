#ifndef BASE_STATION_H
#define BASE_STATION_H

#include "FreeRTOS.h"
#include "queue.h"
#include "sensor.h"

typedef struct {
    float reading;
    uint32_t timestamp_ms;
} MLPacket;

void BaseStationTask_Create(QueueHandle_t sensorQueue);

#endif