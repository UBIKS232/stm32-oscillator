#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "panel_generator.h"
#include <stdint.h>

void gen_init(void);
void gen_set_freq(uint32_t freq);
void gen_set_waveform(panel_generator_state_t waveform);

#endif  //_GENERATOR_H_