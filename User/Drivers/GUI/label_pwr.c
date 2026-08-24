#include "liblcd.h"
#include "label_pwr.h"
#include "dmm.h"

void label_pwr_repaint(void){
    LCD_SetCursor(6, 215);
    LCD_SetBrush(0xcccccc);  // #cccccc
    LCD_FillRect(90, 15);

    LCD_SetCursor(18, 226);
    LCD_SetPen(0x000000);  // #000000

    LCD_PrintString("PWR:");

    dmm_handle_t handle = dmm_get_handle();
    LCD_PrintFloatSI(handle.pwr, 6, "V");
}
