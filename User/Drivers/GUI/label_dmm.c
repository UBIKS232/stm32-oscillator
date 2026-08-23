#include "liblcd.h"
#include "label_dmm.h"
#include "dmm.h"

const char* dmm_range_str[] = {
    "DMM-2V:",     "DMM-10V:",     "DMM-50V:",  "DMM-1KOhm:",
    "DMM-10KOhm:", "DMM-100KOhm:", "DMM-1MOhm:"};

char dmm_range_unit = 'V';

void label_cmm_repaint(void) {
    LCD_SetCursor(110, 215);
    LCD_SetBrush(0xcccccc);  // #cccccc
    LCD_FillRect(120, 15);

    LCD_SetCursor(116, 226);
    LCD_SetPen(0x000000);  // #000000

    dmm_handle_t handle = dmm_get_handle();
    LCD_PrintString(dmm_range_str[handle.range]);

    if(handle.range >= DMM_RANGE_1KOHM){
        dmm_range_unit = 'R';
    }

    LCD_PrintFloatSI(handle.value, 6, &dmm_range_unit);
}
