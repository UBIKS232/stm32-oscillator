#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"

void lcd_bl_control(uint8_t bl_on);
void lcd_init(void);
void lcd_show_image(void);

#endif // _LCD_H_