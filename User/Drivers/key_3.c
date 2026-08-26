#include "key.h"
#include "buzzer.h"
#include "panel_generator.h"
#include "panel_runmode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

static void key_3_callback(key_event_t event) {
    if (event == KEY_EVENT_CLICK) {
        buzzer_beep();
        panel_cursor_select();

        panel_generator_deselect();
        panel_runmode_deselect();
        panel_scale_deselect();
    }
}

void key_3_init(void) {
    extern key_hw_context_t key_hws[KEY_NUM_COUNT];
    extern key_handle_t key_handles[KEY_NUM_COUNT];

    key_init(&key_handles[KEY_3], key_read, &key_hws[KEY_3]);
    key_register_callback(&key_handles[KEY_3], key_3_callback);
}
