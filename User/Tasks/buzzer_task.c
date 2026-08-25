#include "buzzer_task.h"
#include "buzzer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define BUZZER_UPDATE_PERIOD_MS 100

void buzzer_task(void* pv_arg) {
    extern SemaphoreHandle_t buzzer_sem;

    buzzer_init();

    while (1) {
        if (xSemaphoreTake(buzzer_sem, portMAX_DELAY) == pdTRUE) {
            buzzer_on();
            vTaskDelay(pdMS_TO_TICKS(BUZZER_UPDATE_PERIOD_MS));
            buzzer_off();
        }
    }

    vTaskDelete(NULL);
}