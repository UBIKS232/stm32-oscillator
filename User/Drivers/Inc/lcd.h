#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>

void lcd_init(void);
void lcd_bl_control(uint8_t bl_on);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(const uint8_t* data, uint16_t size);

#endif // _LCD_H_