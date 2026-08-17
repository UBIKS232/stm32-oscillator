#include "main.h"
#include "key.h"

#include "buzzer.h"

extern key_hw_context_t key_hws[KEY_NUM_COUNT];
extern key_handle_t key_handles[KEY_NUM_COUNT];

static void key_1_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        // extern UART_HandleTypeDef huart1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)"key_1 pressed.\n",
        //                   strlen("key_1 pressed.\n"), HAL_MAX_DELAY);
        buzzer_beep();
    }
}

void key_1_init(void) {
    key_init(&key_handles[KEY_1], key_read, &key_hws[KEY_1]);
    key_register_callback(&key_handles[KEY_1], key_1_callback);
}
