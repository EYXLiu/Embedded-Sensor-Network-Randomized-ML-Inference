#include "FreeRTOS.h"
#include "semphr.h"
#include <stdlib.h>

typedef struct {
    SemaphoreHandle_t sem;
} Event;

Event* event_create(void) {
    Event *ev= malloc(sizeof(Event));
    if (ev) {
        ev->sem = xSemaphoreCreateBinary();
        if (ev->sem == NULL) {
            free(ev);
            return NULL;
        }
    }
    return ev;
}

void event_delete(Event* ev) {
    if (!ev) return;
    vSemaphoreDelete(ev->sem);
    free(ev);
}

void event_signal(Event* ev) {
    xSemaphoreGive(ev->sem);
}

void event_wait(Event* ev) {
    xSemaphoreTake(ev->sem, portMAX_DELAY);
}
