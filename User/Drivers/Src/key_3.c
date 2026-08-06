#include "main.h"
#include "key_task.h"
#include "key.h"
// #include <string.h>

#include "buzzer.h"

static key_hw_context_t key_3_hw = {
    .gpiox = key_3_GPIO_Port, .gpio_pin = key_3_Pin, .active_low = 1U};

static key_handle_t key_3_handle = {0};

static uint8_t key_3_read_pin(key_hw_context_t* hw_context) {
    key_hw_context_t* hw = (key_hw_context_t*)hw_context;
    GPIO_PinState state = HAL_GPIO_ReadPin(hw->gpiox, hw->gpio_pin);

    if (hw->active_low) {
        return ((state == GPIO_PIN_RESET) ? 0U : 1U);
    } else {
        return ((state == GPIO_PIN_SET) ? 0U : 1U);
    }
}

static void key_3_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        // extern UART_HandleTypeDef huart1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)"key_3 pressed.\n",
        //                   strlen("key_3 pressed.\n"), HAL_MAX_DELAY);
        buzzer_beep();
    }
}

void key_3_init(void){
    key_init(&key_3_handle, key_3_read_pin, &key_3_hw);
    key_register_callback(&key_3_handle,key_3_callback);
    key_task_register(&key_3_handle);
}