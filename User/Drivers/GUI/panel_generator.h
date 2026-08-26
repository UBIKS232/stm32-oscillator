#ifndef _PANEL_GENERATOR_H_
#define _PANEL_GENERATOR_H_

#include <stdint.h>

typedef enum panel_generator_state {
    PANEL_GENRATOR_OFF = 0,
    PANEL_GENRATOR_SIN,
    PANEL_GENRATOR_TRIANGLE,
    PANEL_GENRATOR_SQUARE
} panel_generator_state_t;

void panel_generator_repaint(void);
void panel_generator_select(void);
void panel_generator_deselect(void);
void panel_generator_adjust(int8_t direction);

#endif  // _PANEL_GENERATOR_H_