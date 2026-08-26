#include "label_cursor.h"
#include "liblcd.h"
#include "gui_config.h"
#include "panel_cursor.h"
#include "panel_scale.h"

void label_cursor_repaint(void)
{
    float x_scale = panel_scale_get_x();
    float y_scale = panel_scale_get_y();

    int8_t x1_pct = panel_cursor_get_x1_pct();
    int8_t x2_pct = panel_cursor_get_x2_pct();
    int8_t y1_pct = panel_cursor_get_y1_pct();
    int8_t y2_pct = panel_cursor_get_y2_pct();

    // Convert percentages to physical values (using LCD library functions)
    float x1_val = LCD_GetXCursorValue(x1_pct, x_scale);
    float x2_val = LCD_GetXCursorValue(x2_pct, x_scale);
    float y1_val = LCD_GetYCursorValue(y1_pct, y_scale);
    float y2_val = LCD_GetYCursorValue(y2_pct, y_scale);

    float delta_x = x2_val - x1_val;
    float delta_y = y2_val - y1_val;

    // 1 light cyan background (first row)
    LCD_SetBrush(COLOR_CYAN);
    LCD_SetCursor(LABEL_CURSOR_BG_X, LABEL_CURSOR_BG_Y);
    LCD_FillRect(LABEL_CURSOR_BG_WIDTH, LABEL_CURSOR_BG_HEIGHT);

    // 2 X1 label and value
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_CURSOR_TEXT_X1, LABEL_CURSOR_TEXT_Y1);
    LCD_PrintString("X1:");
    LCD_PrintFloatSI(x1_val, 5, "s");

    // 3 Y1 label and value
    LCD_SetCursor(LABEL_CURSOR_TEXT_X2, LABEL_CURSOR_TEXT_Y1);
    LCD_PrintString("Y1:");
    LCD_PrintFloatSI(y1_val, 5, "V");

    // 4 magenta background (second row)
    LCD_SetBrush(COLOR_PURPLE);
    LCD_SetCursor(LABEL_CURSOR_BG_X, LABEL_CURSOR_BG2_Y);
    LCD_FillRect(LABEL_CURSOR_BG_WIDTH, LABEL_CURSOR_BG_HEIGHT);

    // 5 X2 label and value
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_CURSOR_TEXT_X1, LABEL_CURSOR_TEXT_Y2);
    LCD_PrintString("X2:");
    LCD_PrintFloatSI(x2_val, 5, "s");

    // 6 Y2 label and value
    LCD_SetCursor(LABEL_CURSOR_TEXT_X2, LABEL_CURSOR_TEXT_Y2);
    LCD_PrintString("Y2:");
    LCD_PrintFloatSI(y2_val, 5, "V");

    // 7 white background (third row)
    LCD_SetBrush(COLOR_WHITE);
    LCD_SetCursor(LABEL_CURSOR_BG_X, LABEL_CURSOR_BG3_Y);
    LCD_FillRect(LABEL_CURSOR_BG_WIDTH, LABEL_CURSOR_BG_HEIGHT);

    // 8 delta X
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_CURSOR_DELTA_X, LABEL_CURSOR_DELTA_Y_ROW);
    LCD_PrintFloatSI(delta_x, 5, "s");

    // 9 delta Y
    LCD_SetCursor(LABEL_CURSOR_DELTA_Y, LABEL_CURSOR_DELTA_Y_ROW);
    LCD_PrintFloatSI(delta_y, 5, "V");
}