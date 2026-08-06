#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

typedef enum key_event {
    KEY_EVENT_CLICK = 0,
    // KEY_EVENT_DOUBLE_CLICK,
    // KEY_EVENT_LONG_PRESS,
} key_event_t;


// hw配置
typedef struct key_hw_context {
    GPIO_TypeDef* gpiox;  // GPIO 端口
    uint16_t gpio_pin;    // GPIO 引脚
    uint8_t active_low;   // 1=低电平有效, 0=高电平有效
} key_hw_context_t;


// 调用hw api读取pin状态
typedef uint8_t (*key_read_pin_t)(key_hw_context_t* hw_context);

// key回调
typedef void (*key_callback_t)(key_event_t event);

typedef struct key_handle {
    // 硬件层
    key_read_pin_t read_func;
    key_hw_context_t* hw_context;

    // 逻辑层
    uint8_t previous;  // 1=释放, 0=按下
    uint8_t current;
    uint8_t pressed;  // 当前是否处于按下状态

    // 任务层
    key_callback_t callback;
} key_handle_t;

void key_init(key_handle_t* const handle, key_read_pin_t read_func, key_hw_context_t* hw_context);
void key_register_callback(key_handle_t* const handle, key_callback_t callback);
void key_scan(key_handle_t* const handle);

#endif  // _KEY_H_