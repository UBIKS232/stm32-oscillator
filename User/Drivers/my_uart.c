#include "main.h"
#include "my_uart.h"
#include "FreeRTOS.h"
#include "queue.h"

#include <string.h>

#define UART_QUE_LENGTH 10
#define UART_QUE_ITEM_SIZE 4
QueueHandle_t uart_que = {0};

void uart_init(void) {
    uart_que = xQueueCreate(UART_QUE_LENGTH, UART_QUE_ITEM_SIZE);
    if(uart_que == 0){
        while(1);
    }
}

void uart_action(const char* pc_msg[]) {
    xQueueSend(uart_que, pc_msg, portMAX_DELAY);
}

extern UART_HandleTypeDef huart1;

void uart_send(const char* pc_string) {
    HAL_UART_Transmit(&huart1, (const uint8_t*)pc_string, strlen(pc_string),
                      HAL_MAX_DELAY);
}
