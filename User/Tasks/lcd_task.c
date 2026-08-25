#include "lcd_task.h"
#include "lcd.h"
#include "gui_config.h"
#include "dmm.h"
#include "label_dmm.h"
#include "label_pwr.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LCD_UPDATE_PERIOD_MS 100

void lcd_task(void* pv_arg) {
    extern EventGroupHandle_t lcd_event;

    dmm_init();
    lcd_init();

    while (1) {
        EventBits_t event =
            xEventGroupWaitBits(lcd_event, LABEL_DMM_BIT | LABEL_PWR_BIT,
                                pdTRUE, pdFALSE, portMAX_DELAY);

        if (event & LABEL_DMM_BIT) {
            label_dmm_repaint();
        }
        if (event & LABEL_PWR_BIT) {
            label_pwr_repaint();
        }
        vTaskDelay(pdMS_TO_TICKS(LCD_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}