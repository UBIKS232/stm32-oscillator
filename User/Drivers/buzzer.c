#include "main.h"
#include "buzzer.h"
#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t buzzer_sem = NULL;  // - buzzer_task

void buzzer_init(void) {
    if (buzzer_sem == NULL) {
        buzzer_sem = xSemaphoreCreateCounting(5, 0);
    }

    if (!buzzer_sem) return;
}

void buzzer_beep(void) {
    if ((!buzzer_sem) || (xSemaphoreGive(buzzer_sem) != pdTRUE)) return;
    ;
}

void buzzer_on(void) { HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 1); }

void buzzer_off(void) { HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, 0); }
