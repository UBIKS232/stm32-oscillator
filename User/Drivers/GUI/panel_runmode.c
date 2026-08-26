#include "liblcd.h"
#include "gui_config.h"
#include "panel_runmode.h"
#include "FreeRTOS.h"
#include "event_groups.h"

static int8_t selected_index = -1;  // -1: not selected, 0: param selected
static uint8_t run_mode = 0;        // 0: Auto-run, 1: One-shot

void panel_runmode_repaint(void) {
    // 1 title bar
    LCD_SetBrush(COLOR_PURPLE);
    LCD_SetCursor(PANEL_RUNMODE_TITLE_X, PANEL_RUNMODE_TITLE_Y);
    LCD_FillRect(PANEL_RUNMODE_TITLE_WIDTH, PANEL_RUNMODE_TITLE_HEIGHT);

    // 2 title text
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(PANEL_RUNMODE_TITLE_TEXT_X, PANEL_RUNMODE_TITLE_TEXT_Y);
    LCD_PrintString("Run Mode");

    // 3 panel background
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(PANEL_RUNMODE_BG_X, PANEL_RUNMODE_BG_Y);
    LCD_FillRect(PANEL_RUNMODE_BG_WIDTH, PANEL_RUNMODE_BG_HEIGHT);

    // 4 cursor (only when selected)
    if (selected_index == 0) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_RUNMODE_CURSOR_X, PANEL_RUNMODE_CURSOR_Y);
        LCD_FillRect(PANEL_RUNMODE_CURSOR_WIDTH, PANEL_RUNMODE_CURSOR_HEIGHT);
    }

    // 5 parameter text
    LCD_SetCursor(PANEL_RUNMODE_PARAM1_TEXT_X, PANEL_RUNMODE_PARAM1_TEXT_Y);
    if (selected_index == 0) {
        LCD_SetBrush(COLOR_CYAN);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    if (run_mode == 0) {
        LCD_PrintString("Running");
    } else {
        LCD_PrintString("One-Shot");
    }
}

void panel_runmode_select(void) {
    extern EventGroupHandle_t lcd_event;

    selected_index = 0;
    xEventGroupSetBits(lcd_event, PANEL_RUNMODE_BIT);
}

void panel_runmode_deselect(void) {
    extern EventGroupHandle_t lcd_event;

    if (selected_index >= 0) {
        selected_index = -1;
        xEventGroupSetBits(lcd_event, PANEL_RUNMODE_BIT);
    }
}

void panel_runmode_adjust(int8_t direction) {
    extern EventGroupHandle_t lcd_event;
    if (selected_index < 0) return;

    if (selected_index == 0) {
        run_mode = (run_mode + direction + 2) % 2;
    }
    xEventGroupSetBits(lcd_event, PANEL_RUNMODE_BIT);
}