#include "key.h"
#include "buzzer.h"
#include "panel_generator.h"
#include "panel_runmode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

static void key_down_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        buzzer_beep();
        panel_generator_adjust(-1);
        panel_runmode_adjust(-1);
        panel_scale_adjust(-1);
        panel_cursor_adjust(-1);
    }
}

void key_down_init(void) {
    extern key_hw_context_t key_hws[KEY_NUM_COUNT];
    extern key_handle_t key_handles[KEY_NUM_COUNT];

    key_init(&key_handles[KEY_DOWN], key_read, &key_hws[KEY_DOWN]);
    key_register_callback(&key_handles[KEY_DOWN], key_down_callback);
}
