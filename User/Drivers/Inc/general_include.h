#ifndef _GENERAL_INCLUDE_H_
#define _GENERAL_INCLUDE_H_

#include "lcd_task.h"
#include "buzzer_task.h"
#include "key_task.h"

void initial_task(void const* argument) {
    taskENTER_CRITICAL();

    xTaskCreate(buzzer_task, "buzzer_task", 64, NULL, 1, NULL);
    xTaskCreate(lcd_task, "lcd_task", 256, NULL, 1, NULL);
    xTaskCreate(key_task, "key_task", 128, NULL, 3, NULL);

    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

#endif  // _GENERAL_INCLUDE_H_