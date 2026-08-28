#include "panel_scale.h"
#include "osc.h"
#include "liblcd.h"
#include "gui_config.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdint.h>

static int8_t is_selected_index = -1;  // -1 none, 0 x-scale, 1 y-scale

// x-scale values: 10us,20us,50us,100us,200us,500us,1ms,2ms,5ms,10ms,20ms,50ms
static const float x_scales[] = {10.0e-6f,  20.0e-6f,  50.0e-6f, 100.0e-6f,
                                 200.0e-6f, 500.0e-6f, 1.0e-3f,  2.0e-3f,
                                 5.0e-3f,   10.0e-3f,  20.0e-3f, 50.0e-3f};
static int16_t x_scale_index = 5;  // default 500us
static const uint16_t x_sclaes_num = sizeof(x_scales) / sizeof(float);

// y-scale values: 50mV,100mV,200mV,500mV,1V
static const float y_scales[] = {50.0e-3f, 100.0e-3f, 200.0e-3f, 500.0e-3f,
                                 1.0f};
static int16_t y_scale_index = 3;  // default 1V
static const uint16_t y_scales_num = sizeof(y_scales) / sizeof(float);

void panel_scale_repaint(void) {
    // 1 title bar
    LCD_SetBrush(COLOR_PURPLE);
    LCD_SetCursor(PANEL_SCALE_TITLE_X, PANEL_SCALE_TITLE_Y);
    LCD_FillRect(PANEL_SCALE_TITLE_WIDTH, PANEL_SCALE_TITLE_HEIGHT);

    // 2 title text
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(PANEL_SCALE_TITLE_TEXT_X, PANEL_SCALE_TITLE_TEXT_Y);
    LCD_PrintString("Scale");

    // 3 panel background
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(PANEL_SCALE_BG_X, PANEL_SCALE_BG_Y);
    LCD_FillRect(PANEL_SCALE_BG_WIDTH, PANEL_SCALE_BG_HEIGHT);

    // 4 cursor for param1 (x)
    if (is_selected_index == 0) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_SCALE_CURSOR_PARAM1_X, PANEL_SCALE_CURSOR_PARAM1_Y);
        LCD_FillRect(PANEL_SCALE_CURSOR_WIDTH, PANEL_SCALE_CURSOR_HEIGHT);
    }

    // 5 param1 text (x)
    if (is_selected_index == 0) {
        LCD_SetBrush(COLOR_CYAN);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_SCALE_PARAM1_TEXT_X, PANEL_SCALE_PARAM1_TEXT_Y);
    LCD_PrintString("x");

    // 6 cursor for param2 (y)
    if (is_selected_index == 1) {
        LCD_SetBrush(COLOR_CYAN);
        LCD_SetCursor(PANEL_SCALE_CURSOR_PARAM2_X, PANEL_SCALE_CURSOR_PARAM2_Y);
        LCD_FillRect(PANEL_SCALE_CURSOR_WIDTH, PANEL_SCALE_CURSOR_HEIGHT);
    }

    // 7 param2 text (y)
    if (is_selected_index == 1) {
        LCD_SetBrush(COLOR_CYAN);
    } else {
        LCD_SetBrush(COLOR_GRAY);
    }
    LCD_SetCursor(PANEL_SCALE_PARAM2_TEXT_X, PANEL_SCALE_PARAM2_TEXT_Y);
    LCD_PrintString("y");
}

void panel_scale_select(void) {
    if (is_selected_index < 0) {
        is_selected_index = 0;
    } else {
        is_selected_index =
            (is_selected_index + 1) % 2;  // toggle between 0 and 1
    }
    extern EventGroupHandle_t lcd_event;
    xEventGroupSetBits(lcd_event, PANEL_SCALE_BIT | LABEL_SCALE_BIT |
                                      LABEL_CURSOR_BIT | PANEL_OSC_BIT);
}

void panel_scale_deselect(void) {
    if (is_selected_index >= 0) {
        is_selected_index = -1;
        // x_scale_index = 5;  // default 500us
        // y_scale_index = 3;  // default 1V
        extern EventGroupHandle_t lcd_event;
        xEventGroupSetBits(lcd_event, PANEL_SCALE_BIT | LABEL_SCALE_BIT |
                                          LABEL_CURSOR_BIT |
                                          PANEL_OSC_BIT);
    }
}

static const uint16_t psc_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const uint16_t arr_list[] = {143,  143,  143,  143,   225,   561,
                                    1124, 2251, 5624, 11249, 22549, 56249};

void panel_scale_adjust(int8_t direction) {
    if (is_selected_index < 0) return;

    switch (is_selected_index) {
        case 0:
            x_scale_index =
                (x_scale_index + direction + x_sclaes_num) % x_sclaes_num;
            break;
        case 1:
            y_scale_index =
                (y_scale_index + direction + y_scales_num) % y_scales_num;
            break;
        default:
            return;
    }

    // 根据x_scale_index的值设置采样率
    osc_set_sample_rate(psc_list[x_scale_index], arr_list[x_scale_index]);

    extern EventGroupHandle_t lcd_event;
    xEventGroupSetBits(lcd_event, PANEL_SCALE_BIT | LABEL_SCALE_BIT |
                                      LABEL_CURSOR_BIT | PANEL_OSC_BIT);
}

float panel_scale_get_x(void) { return x_scales[x_scale_index]; }

float panel_scale_get_y(void) { return y_scales[y_scale_index]; }