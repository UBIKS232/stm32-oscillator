#include "lcd_task.h"
#include "lcd.h"
#include "gui_config.h"
#include "dmm.h"
#include "osc.h"
#include "label_dmm.h"
#include "label_pwr.h"
#include "panel_generator.h"
#include "panel_runmode.h"
#include "panel_scale.h"
#include "label_scale.h"
#include "panel_cursor.h"
#include "label_cursor.h"
#include "panel_osc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LCD_UPDATE_PERIOD_MS 100

void lcd_task(void* pv_arg) {
    extern EventGroupHandle_t lcd_event;

    dmm_init();
    osc_init(); // osc_init should be after dmm_init!
    lcd_init();

    while (1) {
        EventBits_t event = xEventGroupWaitBits(lcd_event, ALL_REPAINT_BITS,
                                                pdTRUE, pdFALSE, portMAX_DELAY);

        if (event & LABEL_DMM_BIT) {
            label_dmm_repaint();
        }
        if (event & LABEL_PWR_BIT) {
            label_pwr_repaint();
        }
        if (event & PANEL_GENERATOR_BIT) {
            panel_generator_repaint();
        }
        if (event & PANEL_RUNMODE_BIT) {
            panel_runmode_repaint();
        }
        if (event & PANEL_SCALE_BIT) {
            panel_scale_repaint();
        }
        if (event & LABEL_SCALE_BIT) {
            label_scale_repaint();
        }
        if (event & PANEL_CURSOR_BIT) {
            panel_cursor_repaint();
        }
        if (event & LABEL_CURSOR_BIT) {
            label_cursor_repaint();
        }
        if (event & PANEL_OSC_BIT) {
            panel_osc_repaint();
        }
        vTaskDelay(pdMS_TO_TICKS(LCD_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}