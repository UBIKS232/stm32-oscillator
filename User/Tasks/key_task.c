#include "key_task.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

#define KEY_UPDATE_PERIOD_MS 10

void key_task(void* pv_arg) {
    extern key_handle_t key_handles[KEY_NUM_COUNT];

    extern void key_1_init(void);
    extern void key_2_init(void);
    extern void key_3_init(void);
    extern void key_4_init(void);
    extern void key_up_init(void);
    extern void key_down_init(void);

    key_1_init();
    key_2_init();
    key_3_init();
    key_4_init();
    key_up_init();
    key_down_init();

    while (1) {
        for (int i = 0; i < KEY_NUM_COUNT; i++) {
            if (key_handles[i].read != NULL) key_scan(&key_handles[i]);
        }
        vTaskDelay(pdMS_TO_TICKS(KEY_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}