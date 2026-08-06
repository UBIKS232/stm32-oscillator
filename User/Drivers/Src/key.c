#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

void key_init(key_handle_t* const handle, key_read_pin_t read_func,
              key_hw_context_t* hw_context) {
    handle->read_func = read_func;
    handle->hw_context = hw_context;
    handle->previous = 1U;
    handle->current = 1U;
    handle->pressed = 0U;
    handle->callback = NULL;
}

// 依赖注入
void key_register_callback(key_handle_t* const handle,
                           key_callback_t callback) {
    if (handle) handle->callback = callback;
}

void key_scan(key_handle_t* const handle) {
    if ((!handle) || (!handle->read_func)) return;

    handle->current = handle->read_func(handle->hw_context);

    // 上升沿
    if ((handle->previous == 0U) && (handle->current == 1U)) {
        handle->pressed = 1U;
        if (handle->callback) {
            handle->callback(KEY_EVENT_CLICK);
        }
    }

    handle->previous = handle->current;
}
