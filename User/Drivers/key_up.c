#include "key.h"

#include "my_uart.h"
#include "buzzer.h"

const char* key_up_msg = "key_up_pressed\n";

static void key_up_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        uart_action(&key_up_msg);
        buzzer_beep();
    }
}

void key_up_init(void) {
    extern key_hw_context_t key_hws[KEY_NUM_COUNT];
    extern key_handle_t key_handles[KEY_NUM_COUNT];

    key_init(&key_handles[KEY_UP], key_read, &key_hws[KEY_UP]);
    key_register_callback(&key_handles[KEY_UP], key_up_callback);
}
