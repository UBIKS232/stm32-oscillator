#include "main.h"
#include "key.h"

key_hw_context_t key_hws[KEY_NUM_COUNT] = {
    [KEY_1] = {.GPIOx = key_1_GPIO_Port,
               .GPIO_Pin = key_1_Pin,
               .active_low = 1U},
    [KEY_2] = {.GPIOx = key_2_GPIO_Port,
               .GPIO_Pin = key_2_Pin,
               .active_low = 1U},
    [KEY_3] = {.GPIOx = key_3_GPIO_Port,
               .GPIO_Pin = key_3_Pin,
               .active_low = 1U},
    [KEY_4] = {.GPIOx = key_4_GPIO_Port,
               .GPIO_Pin = key_4_Pin,
               .active_low = 1U},
    [KEY_UP] = {.GPIOx = key_up_GPIO_Port,
                .GPIO_Pin = key_up_Pin,
                .active_low = 1U},
    [KEY_DOWN] = {.GPIOx = key_down_GPIO_Port,
                  .GPIO_Pin = key_down_Pin,
                  .active_low = 1U},
};

key_handle_t key_handles[KEY_NUM_COUNT] = {0};

// 1 is active
uint8_t key_read(const key_hw_context_t* hw) {
    GPIO_PinState state = HAL_GPIO_ReadPin(hw->GPIOx, hw->GPIO_Pin);
    return (state ^ hw->active_low);  // A ^ B ^ B = A
}

void key_init(key_handle_t* const handle, key_read_pin_t read,
              const key_hw_context_t* const hw) {
    handle->read = read;
    handle->hw = hw;
    handle->previous = 1U;
    handle->current = 1U;
    handle->callback = NULL;
}

void key_register_callback(key_handle_t* const handle,
                           key_callback_t callback) {
    if (handle) handle->callback = callback;
}

void key_scan(key_handle_t* const handle) {
    if ((!handle) || (!handle->read)) return;

    handle->current = handle->read(handle->hw);

    // 仅仅处理单击的情况
    if ((handle->previous == 0U) &&
        (handle->current == 1U)) {  // 捕捉逻辑上升沿
        if (handle->callback) {
            handle->callback(KEY_EVENT_CLICK);
        }
    }
    handle->previous = handle->current;
}