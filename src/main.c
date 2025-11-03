#include "FreeRTOS.h"
#include "task.h"
#include "sensor.h"
#include "base_station.h"
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL));

    QueueHandle_t sensorQueue = xQueueCreate(10, sizeof(SensorPacket));

    SensorTask_Create(0, sensorQueue);

    BaseStationTask_Create(sensorQueue);

    vTaskStartScheduler();

    return 0;
}
