#include "liblcd.h"
#include "gui_config.h"
#include "generator.h"
#include "panel_generator.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdint.h>

// -1: not selected, 0: param1 selected, 1: param2 selected
static int8_t is_selected_index = -1;
// waveform
static panel_generator_state_t waveform = PANEL_GENRATOR_OFF;
// f
static float freq = 1.0e3f;

static const float freqs[] = {50.0f,  100.0f, 200.0f,  500.0f, 1.0e3f,
                              2.0e3f, 5.0e3f, 10.0e3f, 50.0e3f};
static const int freqs_cnt = sizeof(freqs) / sizeof(float);
static int8_t freq_index = 4;

static const char* panel_generator_get_shape_string(
    panel_generator_state_t waveform) {
    const char* pc_string = "Unknown";
    if (waveform == PANEL_GENRATOR_OFF)
        pc_string = "OFF";
    else if (waveform == PANEL_GENRATOR_SIN)
        pc_string = "Sin";
    else if (waveform == PANEL_GENRATOR_TRIANGLE)
        pc_string = "Triangle";
    else if (waveform == PANEL_GENRATOR_SQUARE)
        pc_string = "Square";
    return pc_string;
}

void panel_generator_repaint(void) {
    // 1 title bar (250,180) width 65 height 15 #FF7BFF
    LCD_SetBrush(COLOR_PURPLE);
    LCD_SetCursor(PANEL_GEN_TITLE_X, PANEL_GEN_TITLE_Y);
    LCD_FillRect(PANEL_GEN_TITLE_WIDTH, PANEL_GEN_TITLE_HEIGHT);

    // 2 title text (252,193) #000000
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(PANEL_GEN_TITLE_TEXT_X, PANEL_GEN_TITLE_TEXT_Y);
    LCD_PrintString("Generator");

    // 3 panel background (250,195) width 65 height 35 #CCCCCC
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(PANEL_GEN_BG_X, PANEL_GEN_BG_Y);
    LCD_FillRect(PANEL_GEN_BG_WIDTH, PANEL_GEN_BG_HEIGHT);

    // 4 cursor
    // param1 selected: (252,198) width 60 height 10 #00FFFF
    // param2 selected: (252,208) width 60 height 10 #00FFFF
    if (is_selected_index >= 0 && is_selected_index <= 1) {
        LCD_SetBrush(COLOR_CYAN);
        if (is_selected_index == 0) {
            LCD_SetCursor(PANEL_GEN_CURSOR_PARAM1_X, PANEL_GEN_CURSOR_PARAM1_Y);
        } else {
            LCD_SetCursor(PANEL_GEN_CURSOR_PARAM2_X, PANEL_GEN_CURSOR_PARAM2_Y);
        }
        LCD_FillRect(PANEL_GEN_CURSOR_WIDTH, PANEL_GEN_CURSOR_HEIGHT);
    }

    // 5 param1 (253,208) waveform shape
    const char* pc_parameter_string_1 =
        panel_generator_get_shape_string(waveform);
    if (is_selected_index == 0) {
        LCD_SetBrush(COLOR_CYAN);  // selected, cyan background
    } else {
        LCD_SetBrush(COLOR_GRAY);  // not selected, gray background
    }
    LCD_SetCursor(PANEL_GEN_PARAM1_TEXT_X, PANEL_GEN_PARAM1_TEXT_Y);
    LCD_PrintString(pc_parameter_string_1);

    // 6 param2 (253,218) frequency
    freq = freqs[freq_index];
    if (is_selected_index == 1) {
        LCD_SetBrush(COLOR_CYAN);  // selected, cyan background
    } else {
        LCD_SetBrush(COLOR_GRAY);  // not selected, gray background
    }
    LCD_SetCursor(PANEL_GEN_PARAM2_TEXT_X, PANEL_GEN_PARAM2_TEXT_Y);
    LCD_PrintFloatSI(freq, 6, "Hz");
}

void panel_generator_select(void) {
    extern EventGroupHandle_t lcd_event;

    if (is_selected_index < 0) {
        is_selected_index = 0;
    } else {
        is_selected_index = (is_selected_index + 1) % 3;
    }

    xEventGroupSetBits(lcd_event, PANEL_GENERATOR_BIT);
}

void panel_generator_deselect(void) {
    extern EventGroupHandle_t lcd_event;

    if (is_selected_index != -1) {
        is_selected_index = -1;
        // waveform = PANEL_GENRATOR_OFF;
        // freq_index = 4;
        xEventGroupSetBits(lcd_event, PANEL_GENERATOR_BIT);
    }
}

void panel_generator_adjust(int8_t direction) {
    extern EventGroupHandle_t lcd_event;
    if (is_selected_index == -1) return;

    switch (is_selected_index) {
        case 0:  // param1: waveform
            waveform = (waveform + direction + 4) % 4;
            gen_set_waveform(waveform);
            break;
        case 1:  // param2: frequence
            freq_index = (freq_index + direction + freqs_cnt) % freqs_cnt;
            freq = freqs[freq_index];
            gen_set_freq(freq);
            break;
        default:
            return;
    }

    xEventGroupSetBits(lcd_event, PANEL_GENERATOR_BIT);
}