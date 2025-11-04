#ifndef SENSOR_H
#define SENSOR_H

#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
    uint8_t node_id;
    float reading;
    uint32_t timestamp_ms;
} SensorPacket;

typedef struct {
    uint8_t node_id; 
    QueueHandle_t queue;
} SensorParams;

void SensorTask_Create(uint8_t node_id, QueueHandle_t queue_handle);

#endif 