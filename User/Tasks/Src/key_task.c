#include "key.h"
#include "key_task.h"
#include "FreeRTOS.h"
#include "task.h"

#define MAX_KEY_NUM 6

static key_handle_t* h_key_list[MAX_KEY_NUM];
static uint8_t h_key_count = 0;

void key_task_register(key_handle_t* handle) {
    if (h_key_count < MAX_KEY_NUM) {
        h_key_list[h_key_count++] = handle;
    }
}

void key_task(void* p_arg) {
    while (1) {
        for (int i = 0; i < h_key_count; i++) {
            key_scan(h_key_list[i]);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}