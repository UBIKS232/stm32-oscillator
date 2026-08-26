#ifndef _SCALE_PANEL_H_
#define _SCALE_PANEL_H_

#include <stdint.h>

void panel_scale_repaint(void);
void panel_scale_select(void);
void panel_scale_deselect(void);
void panel_scale_adjust(int8_t direction);
float panel_scale_get_x(void);
float panel_scale_get_y(void);

#endif  //_SCALE_PANEL_H_