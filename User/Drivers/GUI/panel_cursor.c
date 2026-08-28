#include "panel_cursor.h"
#include "liblcd.h"
#include "gui_config.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdint.h>

// in percentage
static int8_t cursor_pos = -1;    // -1 none, 0 x1, 1 x2, 2 y1, 3 y2
static int8_t cursor_x1_pct = 0;  // 0~100
static int8_t cursor_x2_pct = 0;  // 0~100
static int8_t cursor_y1_pct = 0;  // -100~+100
static int8_t cursor_y2_pct = 0;  // -100~+100

void panel_cursor_repaint(void) {
    // 1 magenta title bar
    LCD_SetBrush(COLOR_PURPLE);
    LCD_SetCursor(PANEL_CURSOR_TITLE_X, PANEL_CURSOR_TITLE_Y);
    LCD_FillRect(PANEL_CURSOR_TITLE_WIDTH, PANEL_CURSOR_TITLE_HEIGHT);

    // 2 title text
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(PANEL_CURSOR_TITLE_TEXT_X, PANEL_CURSOR_TITLE_TEXT_Y);
    LCD_PrintString("Cursor");

    // 3 gray panel background
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(PANEL_CURSOR_BG_X, PANEL_CURSOR_BG_Y);
    LCD_FillRect(PANEL_CURSOR_BG_WIDTH, PANEL_CURSOR_BG_HEIGHT);

    // 4 param1 (x1)
    if (cursor_pos == 0) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_CURSOR_CURSOR_PARAM1_X,
                      PANEL_CURSOR_CURSOR_PARAM1_Y);
        LCD_FillRect(PANEL_CURSOR_CURSOR_WIDTH, PANEL_CURSOR_CURSOR_HEIGHT);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_CURSOR_PARAM1_TEXT_X, PANEL_CURSOR_PARAM1_TEXT_Y);
    LCD_PrintString("x1");

    // 5 param2 (x2)
    if (cursor_pos == 1) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_CURSOR_CURSOR_PARAM2_X,
                      PANEL_CURSOR_CURSOR_PARAM2_Y);
        LCD_FillRect(PANEL_CURSOR_CURSOR_WIDTH, PANEL_CURSOR_CURSOR_HEIGHT);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_CURSOR_PARAM2_TEXT_X, PANEL_CURSOR_PARAM2_TEXT_Y);
    LCD_PrintString("x2");

    // 6 param3 (y1)
    if (cursor_pos == 2) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_CURSOR_CURSOR_PARAM3_X,
                      PANEL_CURSOR_CURSOR_PARAM3_Y);
        LCD_FillRect(PANEL_CURSOR_CURSOR_WIDTH, PANEL_CURSOR_CURSOR_HEIGHT);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_CURSOR_PARAM3_TEXT_X, PANEL_CURSOR_PARAM3_TEXT_Y);
    LCD_PrintString("y1");

    // 7 param4 (y2)
    if (cursor_pos == 3) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_CURSOR_CURSOR_PARAM4_X,
                      PANEL_CURSOR_CURSOR_PARAM4_Y);
        LCD_FillRect(PANEL_CURSOR_CURSOR_WIDTH, PANEL_CURSOR_CURSOR_HEIGHT);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_CURSOR_PARAM4_TEXT_X, PANEL_CURSOR_PARAM4_TEXT_Y);
    LCD_PrintString("y2");
}

void panel_cursor_select(void) {
    if (cursor_pos < 0) {
        cursor_pos = 0;
    } else {
        cursor_pos = (cursor_pos + 1) % 4;  // cycle 0->1->2->3->0
    }
    extern EventGroupHandle_t lcd_event;
    xEventGroupSetBits(
        lcd_event, PANEL_CURSOR_BIT | LABEL_CURSOR_BIT | PANEL_OSC_BIT);
}

void panel_cursor_deselect(void) {
    if (cursor_pos >= 0) {
        cursor_pos = -1;
        // cursor_x1_pct = 0;
        // cursor_x2_pct = 0;
        // cursor_y1_pct = 0;
        // cursor_y2_pct = 0;
        extern EventGroupHandle_t lcd_event;
        xEventGroupSetBits(lcd_event, PANEL_CURSOR_BIT | LABEL_CURSOR_BIT |
                                          PANEL_OSC_BIT);
    }
}

void panel_cursor_adjust(int8_t direction) {
    if (cursor_pos < 0) return;

    switch (cursor_pos) {
        case 0:  // x1
            cursor_x1_pct = cursor_x1_pct + direction * 5;
            if (cursor_x1_pct > 100) cursor_x1_pct = 100;
            if (cursor_x1_pct < 0) cursor_x1_pct = 0;
            break;
        case 1:  // x2
            cursor_x2_pct = cursor_x2_pct + direction * 5;
            if (cursor_x2_pct > 100) cursor_x2_pct = 100;
            if (cursor_x2_pct < 0) cursor_x2_pct = 0;
            break;
        case 2:  // y1
            cursor_y1_pct = cursor_y1_pct +direction * 5;
            if (cursor_y1_pct > 100) cursor_y1_pct = 100;
            if (cursor_y1_pct < -100) cursor_y1_pct = -100;
            break;
        case 3:  // y2
            cursor_y2_pct = cursor_y2_pct + direction * 5;
            if (cursor_y2_pct > 100) cursor_y2_pct = 100;
            if (cursor_y2_pct < -100) cursor_y2_pct = -100;
            break;
        default:
            break;
    }

    extern EventGroupHandle_t lcd_event;
    xEventGroupSetBits(
        lcd_event, PANEL_CURSOR_BIT | LABEL_CURSOR_BIT | PANEL_OSC_BIT);
}

// getter functions for label_cursor
int8_t panel_cursor_get_x1_pct(void) { return cursor_x1_pct; }
int8_t panel_cursor_get_x2_pct(void) { return cursor_x2_pct; }
int8_t panel_cursor_get_y1_pct(void) { return cursor_y1_pct; }
int8_t panel_cursor_get_y2_pct(void) { return cursor_y2_pct; }