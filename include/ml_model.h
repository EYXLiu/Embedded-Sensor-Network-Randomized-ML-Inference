#ifndef ML_MODEL_H
#define ML_MODEL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "base_station.h"

#ifdef __cplusplus
extern "C" {
#endif

void MLModelTask(void* pvParameters);
void MLModel_Create(QueueHandle_t mlQueue);

#ifdef __cplusplus
}
#endif

#endif