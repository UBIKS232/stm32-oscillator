#include "key.h"
#include "buzzer.h"
// #include "my_uart.h"
#include "panel_generator.h"
#include "panel_runmode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

const char* key_up_msg = "key_up_pressed\n";

static void key_up_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        buzzer_beep();
        // uart_action(&key_up_msg);
        panel_generator_adjust(+1);
        panel_runmode_adjust(+1);
        panel_scale_adjust(+1);
        panel_cursor_adjust(+1);
    }
}

void key_up_init(void) {
    extern key_hw_context_t key_hws[KEY_NUM_COUNT];
    extern key_handle_t key_handles[KEY_NUM_COUNT];

    key_init(&key_handles[KEY_UP], key_read, &key_hws[KEY_UP]);
    key_register_callback(&key_handles[KEY_UP], key_up_callback);
}
