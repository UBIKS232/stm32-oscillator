#ifndef _OSC_H_
#define _OSC_H_

#include <stdint.h>

float* osc_get_waveform_data(void);
int16_t osc_get_waveform_length(void);
void osc_set_sample_rate(uint16_t psc, uint16_t arr);  // TIM3
uint32_t osc_get_sample_rate(void);
void osc_init(void);

#endif  // _OSC_H_