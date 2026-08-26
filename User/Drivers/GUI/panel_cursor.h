#ifndef _PANEL_CURSOR_H_
#define _PANEL_CURSOR_H_

#include <stdint.h>

void panel_cursor_repaint(void);
void panel_cursor_select(void);
void panel_cursor_deselect(void);
void panel_cursor_adjust(int8_t direction);

int8_t panel_cursor_get_x1_pct(void);
int8_t panel_cursor_get_x2_pct(void);
int8_t panel_cursor_get_y1_pct(void);
int8_t panel_cursor_get_y2_pct(void);

#endif  //_PANEL_CURSOR_H_