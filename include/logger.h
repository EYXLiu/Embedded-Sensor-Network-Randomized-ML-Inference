#ifndef LOGGER_H
#define LOGGER_H

#include "FreeRTOS.h"
#include "queue.h"

#define LOG_MSG_SIZE 128

typedef struct {
    char msg[LOG_MSG_SIZE];
} LogMessage;

void LoggerTask_Create(void);

#ifdef __cplusplus
extern "C" {
#endif

void Logger_Send(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif