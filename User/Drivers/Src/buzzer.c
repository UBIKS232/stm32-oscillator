
#include "main.h"
#include "buzzer.h"
#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t buzzer_bsem = NULL;

void buzzer_init(void) {
    buzzer_bsem = xSemaphoreCreateBinary();
    if (!buzzer_bsem) return;
}

void buzzer_beep(void) {
    if (xSemaphoreGive(buzzer_bsem) != pdTRUE) return;
}

void buzzer_on(void){
    HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
}

void buzzer_off(void){
    HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
}



