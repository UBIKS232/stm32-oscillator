#include "main.h"
#include "key_task.h"
#include "key.h"
#include "key_hw.h"
#include <string.h>

static key_hw_context_t key_2_hw = {
    .gpiox = key_2_GPIO_Port, .gpio_pin = key_2_Pin, .active_low = 1U};

static key_handle_t key_2_handle = {0};

static uint8_t key_2_read_pin(void* context) {
    key_hw_context_t* hw = (key_hw_context_t*)context;
    GPIO_PinState state = HAL_GPIO_ReadPin(hw->gpiox, hw->gpio_pin);

    if (hw->active_low) {
        return ((state == GPIO_PIN_RESET) ? 0U : 1U);
    } else {
        return ((state == GPIO_PIN_SET) ? 0U : 1U);
    }
}

static void key_2_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        extern UART_HandleTypeDef huart1;
        HAL_UART_Transmit(&huart1, (uint8_t*)"key_2 pressed.\n",
                          strlen("key_2 pressed.\n"), HAL_MAX_DELAY);
    }
}

void key_2_init(void){
    key_init(&key_2_handle, key_2_read_pin, &key_2_hw);
    key_register_callback(&key_2_handle,key_2_callback);
    key_task_register(&key_2_handle);
}