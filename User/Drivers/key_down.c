#include "main.h"
#include "key.h"

#include "buzzer.h"

extern key_hw_context_t key_hws[KEY_NUM_COUNT];
extern key_handle_t key_handles[KEY_NUM_COUNT];

static void key_down_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        // extern UART_HandleTypeDef huart1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)"key_down pressed.\n",
        //                   strlen("key_down pressed.\n"), HAL_MAX_DELAY);
        buzzer_beep();
    }
}

void key_down_init(void) {
    key_init(&key_handles[KEY_DOWN], key_read, &key_hws[KEY_DOWN]);
    key_register_callback(&key_handles[KEY_DOWN], key_down_callback);
}
