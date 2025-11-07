#include "ml_model.h"
#include "FreeRTOS.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "../models/simple_model.cc"
#include "task.h"
#include "logger.h"
#include <stdio.h>

extern "C" void MLModelTask(void *pvParameters) {
    QueueHandle_t queue = (QueueHandle_t)pvParameters;
    MLPacket packet;

    constexpr int kTensorArenaSize = 1024 * 10;
    static uint8_t tensor_arena[kTensorArenaSize];

    const tflite::Model* model = tflite::GetModel(g_model);
    static tflite::MicroMutableOpResolver<2> resolver;
    resolver.AddFullyConnected();
    resolver.AddRelu();

    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, kTensorArenaSize);

    interpreter.AllocateTensors();
    TfLiteTensor* input = interpreter.input(0);
    TfLiteTensor* output = interpreter.output(0);

    for (;;) {
        if (xQueueReceive(queue, &packet, portMAX_DELAY) == pdTRUE) {
            float sum = 0;
            for (int i = 0; i < packet.num_sensors; i++) {
                input->data.f[i] = packet.readings[i];
            }
            interpreter.Invoke();
            taskYIELD();
            float prediction = output->data.f[0];
            Logger_Send("[ML] Time %u ms: Predicted = %.2f", packet.timestamp_ms, prediction);
            
            vPortFree(packet.readings);
            packet.readings = NULL;
        }
    }
}

extern "C" void MLModel_Create(QueueHandle_t mlQueue) {
    xTaskCreate(MLModelTask, "MLTASK", configMINIMAL_STACK_SIZE*2, mlQueue, 2, NULL);
    printf("[ML] ML Model created\n");
}
