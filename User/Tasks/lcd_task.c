#include "lcd_task.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "dmm.h"
#include "label_dmm.h"
#include "label_pwr.h"

#define LCD_UPDATE_PERIOD_MS 100

extern SemaphoreHandle_t lcd_sem;

void lcd_task(void* pv_arg) {
    dmm_init();
    lcd_init();

    while (1) {
        label_cmm_repaint();
        label_pwr_repaint();
        vTaskDelay(pdMS_TO_TICKS(LCD_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}