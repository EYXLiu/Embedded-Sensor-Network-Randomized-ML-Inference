#include "network_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "logger.h"
#include <stdlib.h>
#include <stdbool.h>

#define SENSOR_TIMEOUT pdMS_TO_TICKS(5000)

static uint32_t *sensors;
static uint8_t sensor_count = 1;

static void NetworkManagerTask(void *pvParameters) {
    (void) pvParameters;

    bool *offline = malloc(sensor_count * sizeof(bool));
    for (int i = 0; i < sensor_count; i++) offline[i] = false;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        uint32_t current_tick = xTaskGetTickCount();
        
        for (int i = 0; i < sensor_count; i++) {
            if ((current_tick - sensors[i]) > SENSOR_TIMEOUT) {
                if (!offline[i]) {
                    offline[i] = true;
                    Logger_Send("[NetMgr] Sensor %d timeout detected", i+1);
                }
            } else if (offline[i]) {
                offline[i] = false;
                Logger_Send("[NetMgr] Sensors %d back online", i+1);
            }
        }
    }
}

void NetworkManager_Create(uint32_t last_seen[], uint8_t num_sensors) {
    sensors = last_seen;
    sensor_count = num_sensors;
    xTaskCreate(NetworkManagerTask, "NetMgr", configMINIMAL_STACK_SIZE*2, NULL, 1, NULL);
    Logger_Send("[NetMgr] Network Manager created");
}
