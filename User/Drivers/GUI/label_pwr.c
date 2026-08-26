#include "liblcd.h"
#include "label_pwr.h"
#include "dmm.h"
#include "gui_config.h"

void label_pwr_repaint(void) {
    // 绘制背景 (6,215) 宽90 高15 #CCCCCC
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(LABEL_PWR_BG_X, LABEL_PWR_BG_Y);
    LCD_FillRect(LABEL_PWR_BG_WIDTH, LABEL_PWR_BG_HEIGHT);

    // 绘制文本 (18,226) #000000
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_PWR_TEXT_X, LABEL_PWR_TEXT_Y);
    LCD_PrintString("PWR:");

    // 绘制数据
    dmm_handle_t handle = dmm_get_handle();
    LCD_PrintFloatSI(handle.pwr, 6, "V");
}