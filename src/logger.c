#include "logger.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static QueueHandle_t logQueue;

static void LoggerTask(void *pvParameters) {
    LogMessage log;
    for (;;) {
        if (xQueueReceive(logQueue, &log, portMAX_DELAY) == pdTRUE) {
            printf("%s\n", log.msg);
        }
    }
}

void LoggerTask_Create(void) {
    logQueue= xQueueCreate(20, sizeof(LogMessage));
    xTaskCreate(LoggerTask, "Logger", configMINIMAL_STACK_SIZE*2, NULL, 1, NULL);
    Logger_Send("[Lgr] Logger created");
}

void Logger_Send(const char *fmt, ...) {
    if (!logQueue) return;
    LogMessage log;
    va_list args;
    va_start(args, fmt);
    vsnprintf(log.msg, LOG_MSG_SIZE, fmt, args);
    va_end(args);
    xQueueSend(logQueue, &log, portMAX_DELAY);
}
