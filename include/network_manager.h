#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "FreeRTOS.h"
#include "task.h"

void NetworkManager_Create(uint32_t last_seen[], uint8_t num_sensors);

#endif