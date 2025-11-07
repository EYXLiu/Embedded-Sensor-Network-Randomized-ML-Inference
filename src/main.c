#include "FreeRTOS.h"
#include "task.h"
#include "sensor.h"
#include "base_station.h"
#include "ml_model.h"
#include "network_manager.h"
#include "logger.h"
#include <stdlib.h>
#include <time.h>

#define NUM_SENSORS 3

int main(void) {
    srand(time(NULL));

    QueueHandle_t sensorQueue = xQueueCreate(10, sizeof(SensorPacket));
    QueueHandle_t mlQueue = xQueueCreate(10, sizeof(MLPacket));
    uint32_t last_seen[NUM_SENSORS] = {0};

    LoggerTask_Create();

    for (uint8_t i = 1; i <= NUM_SENSORS; i++) {
        SensorTask_Create(i, sensorQueue);
    }

    BaseStationTask_Create(sensorQueue, mlQueue, NUM_SENSORS, last_seen);
    MLModel_Create(mlQueue);
    NetworkManager_Create(last_seen, NUM_SENSORS);

    vTaskStartScheduler();
    return 0;
}
