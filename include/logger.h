#ifndef LOGGER_H
#define LOGGER_H

#include "FreeRTOS.h"
#include "queue.h"

#define LOG_MSG_SIZE 128

typedef struct {
    char msg[LOG_MSG_SIZE];
} LogMessage;

void LoggerTask_Create(void);
void Logger_Send(const char *fmt, ...);

#endif