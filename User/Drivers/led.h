#ifndef _LED_H_
#define _LED_H_

#include "main.h"
#include <stdint.h>

typedef struct led_hw_context {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint8_t active_low;  // active_low ^ GPIO_Pin_State = led true state
} led_hw_context_t;

typedef enum led_num { LED_1 = 0, LED_3, LED_NUM_COUNT } led_num_t;

typedef enum led_mode {
    LED_MODE_OFF,
    LED_MODE_ON,
    LED_MODE_BLINK,
    // LED_MODE_BREATH,
    LED_MODE_COUNT
} led_mode_t;

typedef struct led_ctrl {
    led_num_t num;
    led_mode_t mode;
    uint16_t period_ms;
    uint16_t counter_ms;
    uint8_t current_state;
} led_ctrl_t;

void led_init(void);
void led_set_mode(led_num_t num, led_mode_t mode, uint16_t period_ms);
void led_update(void);

#endif  // _LED_H_