#include "lcd.h"
#include "lcd_task.h"
#include "FreeRTOS.h"
#include "task.h"

void lcd_task(void* p_arg) {
    lcd_init();

    while (1) {
        lcd_show_image();
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(NULL);
}