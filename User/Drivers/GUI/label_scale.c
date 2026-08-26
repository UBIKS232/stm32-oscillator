#include "label_scale.h"
#include "liblcd.h"
#include "gui_config.h"
#include "panel_scale.h"

void label_scale_repaint(void)
{
    // 1 dark yellow background
    LCD_SetBrush(COLOR_DARK_YELLOW);
    LCD_SetCursor(LABEL_SCALE_BG_X, LABEL_SCALE_BG_Y);
    LCD_FillRect(LABEL_SCALE_BG_WIDTH, LABEL_SCALE_BG_HEIGHT);

    // 2 X-axis scale (unit: s)
    float x_scale = panel_scale_get_x();
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_SCALE_TEXT_X, LABEL_SCALE_TEXT1_Y);
    LCD_PrintFloatSI(x_scale, 5, "s");

    // 3 Y-axis scale (unit: V)
    float y_scale = panel_scale_get_y();
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_SCALE_TEXT_X, LABEL_SCALE_TEXT2_Y);
    LCD_PrintFloatSI(y_scale, 5, "V");
}