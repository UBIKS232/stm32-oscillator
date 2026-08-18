#include "lcd_task.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LCD_UPDATE_PERIOD_MS 300

extern SemaphoreHandle_t lcd_sem;

void lcd_task(void* p_arg) {
    lcd_init();

    while (1) {
        lcd_show_image();
        vTaskDelay(pdMS_TO_TICKS(LCD_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}