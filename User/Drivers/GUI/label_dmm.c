#include "liblcd.h"
#include "label_dmm.h"
#include "dmm.h"
#include "gui_config.h"

const char* dmm_range_str[] = {"DMM-2V:",  "DMM-10V:",  "DMM-50V:", "DMM-1K:",
                               "DMM-10K:", "DMM-100K:", "DMM-1M:"};

char dmm_range_unit = 'V';

void label_dmm_repaint(void) {
    // 绘制背景 (110,215) 宽120 高15 #CCCCCC
    LCD_SetBrush(COLOR_GRAY);
    LCD_SetCursor(LABEL_DMM_BG_X, LABEL_DMM_BG_Y);
    LCD_FillRect(LABEL_DMM_BG_WIDTH, LABEL_DMM_BG_HEIGHT);

    // 绘制文本 (116,226) #000000
    LCD_SetPen(COLOR_BLACK);
    LCD_SetCursor(LABEL_DMM_TEXT_X, LABEL_DMM_TEXT_Y);

    // 绘制数据
    dmm_handle_t handle = dmm_get_handle();
    LCD_PrintString(dmm_range_str[handle.range]);
    if (handle.range >= DMM_RANGE_1KOHM) {
        dmm_range_unit = 'R';
    } else {
        dmm_range_unit = 'V';
    }
    LCD_PrintFloatSI(handle.value, 6, &dmm_range_unit);
}