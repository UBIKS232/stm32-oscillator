#ifndef _PANEL_RUNMODE_H_
#define _PANEL_RUNMODE_H_

#include <stdint.h>

void panel_runmode_repaint(void);
void panel_runmode_select(void);
void panel_runmode_deselect(void);
void panel_runmode_adjust(int8_t direction);

#endif  //_PANEL_RUNMODE_H_