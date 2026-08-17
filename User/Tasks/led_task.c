#include "led_task.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_UPDATE_PERIOD_MS 10

void led_task(void* p_arg) {
    led_init();

    led_set_mode(LED_1, LED_MODE_ON, 0);
    led_set_mode(LED_3, LED_MODE_BLINK, 500);

    TickType_t exact_time = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&exact_time, pdMS_TO_TICKS(LED_UPDATE_PERIOD_MS));
        led_update();
    }

    vTaskDelete(NULL);
}