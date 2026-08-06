#include "buzzer.h"
#include "buzzer_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static void buzzer_beep_once(void) {
    buzzer_on();
    vTaskDelay(pdMS_TO_TICKS(100));
    buzzer_off();
}

extern SemaphoreHandle_t buzzer_bsem;

void buzzer_task(void* p_arg) {
    buzzer_init();

    while (1) {
        if (xSemaphoreTake(buzzer_bsem, portMAX_DELAY) == pdTRUE) {
            buzzer_beep_once();
        }
    }

    vTaskDelete(NULL);
}