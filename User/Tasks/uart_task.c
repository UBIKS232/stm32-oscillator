#include "my_uart.h"
#include "uart_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define UART_UPDATE_PERIOD_MS 100

extern QueueHandle_t uart_que;

void uart_task(void* pv_arg) {
    uart_init();

    char* pc_msg = NULL;

    while (1) {
        if (xQueueReceive(uart_que, &pc_msg, portMAX_DELAY) == pdPASS) {
            uart_send(pc_msg);
        }
        vTaskDelay(pdMS_TO_TICKS(UART_UPDATE_PERIOD_MS));
    }

    vTaskDelete(NULL);
}