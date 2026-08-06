#include "lcd.h"
#include "lcd_task.h"
#include "FreeRTOS.h"
#include "task.h"

void lcd_task(void* p_arg) {
    lcd_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}