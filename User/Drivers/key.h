#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#include <stdint.h>

typedef struct key_hw_context {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint8_t active_low;  // active_low ^ GPIO_Pin_State = led true state
} key_hw_context_t;

typedef enum key_num {
    KEY_1 = 0,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_UP,
    KEY_DOWN,
    KEY_NUM_COUNT
} key_num_t;

typedef enum key_event {
    KEY_EVENT_CLICK = 0,
    KEY_EVENT_DOUBLE_CLICK,
    KEY_EVENT_LONG_PRESS,
    KEY_EVENT_COUNT
} key_event_t;

typedef void (*key_callback_t)(key_event_t event);
typedef uint8_t (*key_read_pin_t)(const key_hw_context_t* hw);

typedef struct key_handle {
    uint8_t (*read)(const key_hw_context_t* hw);

    const key_hw_context_t* hw;

    uint8_t previous;
    uint8_t current;

    key_callback_t callback;
} key_handle_t;

uint8_t key_read(const key_hw_context_t* hw);
void key_init(key_handle_t* const handle, key_read_pin_t read,
              const key_hw_context_t* const hw);
void key_register_callback(key_handle_t* const handle, key_callback_t callback);
void key_scan(key_handle_t* const handle);

#endif  //_KEY_H_