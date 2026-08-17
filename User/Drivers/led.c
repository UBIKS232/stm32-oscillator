#include "led.h"
#include "main.h"

static led_hw_context_t led_hws[LED_NUM_COUNT] = {
    [LED_1] = {.GPIOx = led_1_GPIO_Port,
               .GPIO_Pin = led_1_Pin,
               .active_low = 0U},
    [LED_3] = {.GPIOx = led_3_GPIO_Port,
               .GPIO_Pin = led_3_Pin,
               .active_low = 0U},
};

static led_ctrl_t led_ctrls[LED_NUM_COUNT] = {0};

static void led_write(led_num_t num, uint8_t level) {
    if (num >= LED_NUM_COUNT) return;

    led_hw_context_t* hw = &led_hws[num];
    HAL_GPIO_WritePin(hw->GPIOx, hw->GPIO_Pin, hw->active_low ^ level);
}

static void led_on(led_num_t num) { led_write(num, 1); }

static void led_off(led_num_t num) { led_write(num, 0); }

void led_set_mode(led_num_t num, led_mode_t mode, uint16_t period_ms) {
    if (num >= LED_NUM_COUNT) return;

    led_ctrl_t* ctrl = &led_ctrls[num];
    ctrl->mode = mode;
    ctrl->period_ms = period_ms;
    ctrl->counter_ms = 0;

    if (mode == LED_MODE_ON) {
        led_on(num);
        ctrl->current_state = 1;
    } else if (mode == LED_MODE_OFF) {
        led_off(num);
        ctrl->current_state = 0;
    } else {  // BLINK, 起始为灭
        led_off(num);
        ctrl->current_state = 0;
    }
}

void led_update(void) {
    const uint16_t LED_TICK_MS = 10;  // task delay

    for (int i = 0; i < LED_NUM_COUNT; i++) {
        led_ctrl_t* ctrl = &led_ctrls[i];

        if (ctrl->mode != LED_MODE_BLINK) continue;

        ctrl->counter_ms += LED_TICK_MS;
        if (ctrl->counter_ms >= ctrl->period_ms) {
            ctrl->counter_ms = 0;
            ctrl->current_state = !ctrl->current_state;
            ctrl->current_state ? led_on(i) : led_off(i);
        }
    }
}

void led_init(void) {
    for (int i = 0; i < LED_NUM_COUNT; i++) {
        led_ctrls[i].num = i;
        led_ctrls[i].mode = LED_MODE_OFF;
        led_ctrls[i].period_ms = 0;
        led_ctrls[i].counter_ms = 0;
        led_ctrls[i].current_state = 0;
        led_off(i);
    }
}