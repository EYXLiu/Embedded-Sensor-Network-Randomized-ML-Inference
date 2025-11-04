#include "FreeRTOS.h"
#include "task.h"
#include "sensor.h"
#include "base_station.h"
#include "ml_model.h"
#include <stdlib.h>
#include <time.h>

#define NUM_SENSORS 3

int main(void) {
    srand(time(NULL));

    QueueHandle_t sensorQueue = xQueueCreate(10, sizeof(SensorPacket));
    QueueHandle_t mlQueue = xQueueCreate(10, sizeof(MLPacket));

    for (uint8_t i = 1; i <= NUM_SENSORS; i++) {
        SensorTask_Create(i, sensorQueue);
    }


    BaseStationTask_Create(sensorQueue, mlQueue, NUM_SENSORS);

    MLModel_Create(mlQueue);

    vTaskStartScheduler();
    return 0;
}
