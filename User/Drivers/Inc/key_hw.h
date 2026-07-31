#ifndef _KEY_HW_H_
#define _KEY_HW_H_

#include "main.h"

typedef struct key_hw_context {
    GPIO_TypeDef* gpiox;  // GPIO 端口
    uint16_t gpio_pin;    // GPIO 引脚
    uint8_t active_low;   // 1=低电平有效, 0=高电平有效
} key_hw_context_t;

#endif  //_KEY_HW_H_