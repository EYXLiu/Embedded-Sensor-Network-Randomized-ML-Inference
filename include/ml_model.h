#ifndef ML_MODEL_H
#define ML_MODEL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "base_station.h"

void MLModel_Create(QueueHandle_t mlQueue);

#endif