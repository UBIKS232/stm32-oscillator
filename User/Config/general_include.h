#ifndef _GENERAL_INCLUDE_H_
#define _GENERAL_INCLUDE_H_

#define TEST 0

#if (!TEST)
#include "buzzer_task.h"
#include "led_task.h"
#include "lcd_task.h"
#include "uart_task.h"
#include "key_task.h"

void initial_task(void const* argument) {
    taskENTER_CRITICAL();

    xTaskCreate(buzzer_task, "buzzer_task", 64, NULL, 1, NULL);
    xTaskCreate(led_task, "led_task", 128, NULL, 1, NULL);
    xTaskCreate(uart_task, "uart_task", 128, NULL, 1, NULL);
    xTaskCreate(lcd_task, "lcd_task", 256, NULL, 2, NULL);
    xTaskCreate(key_task, "key_task", 256, NULL, 3, NULL);

    taskEXIT_CRITICAL();

    vTaskDelete(NULL);
}
#else
#include "test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t count_sem = NULL;

void initial_task(void const* argument) {
    taskENTER_CRITICAL();

    count_sem = xSemaphoreCreateCounting(6, 1);

    xTaskCreate(tt1, "1", 128, NULL, 1, NULL);
    xTaskCreate(tt2, "2", 128, NULL, 1, NULL);
    xTaskCreate(tt3, "3", 128, NULL, 1, NULL);

    taskEXIT_CRITICAL();

    vTaskDelete(NULL);
}
#endif

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
    int i = 0;
    while (1) {
        i = -i;
    };
}

#endif  // _GENERAL_INCLUDE_H_