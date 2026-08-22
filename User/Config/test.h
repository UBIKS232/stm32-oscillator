#ifndef _TEST_H_
#define _TEST_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t count_sem;

extern UART_HandleTypeDef huart1;

void tt1(void* pv_arg) {
    HAL_UART_Transmit(&huart1, (uint8_t*)"A waiting.\n", sizeof("A waiting.\n"),
                      HAL_MAX_DELAY);

    xSemaphoreTake(count_sem, portMAX_DELAY);

    HAL_UART_Transmit(&huart1, (uint8_t*)"A entering.\n",
                      sizeof("A entering.\n"), HAL_MAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(3000));

    xSemaphoreGive(count_sem);

    HAL_UART_Transmit(&huart1, (uint8_t*)"A leaving.\n", sizeof("A leaving.\n"),
                      HAL_MAX_DELAY);

    vTaskDelete(NULL);
}

void tt2(void* pv_arg) {
    vTaskDelay(pdMS_TO_TICKS(1000));

    HAL_UART_Transmit(&huart1, (uint8_t*)"B waiting.\n", sizeof("B waiting.\n"),
                      HAL_MAX_DELAY);

    BaseType_t x_return = xSemaphoreTake(count_sem, pdMS_TO_TICKS(50));

    if (x_return == pdFAIL) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"B quiting.\n",
                          sizeof("B quiting.\n"), HAL_MAX_DELAY);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"B entering.\n",
                          sizeof("B entering.\n"), HAL_MAX_DELAY);
    }

    vTaskDelete(NULL);
}

void tt3(void* pv_arg) {
    vTaskDelay(pdMS_TO_TICKS(2000));

    HAL_UART_Transmit(&huart1, (uint8_t*)"C waiting.\n", sizeof("C waiting.\n"),
                      HAL_MAX_DELAY);

    BaseType_t x_return = xSemaphoreTake(count_sem, portMAX_DELAY);

    if (x_return == pdFAIL) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"C quiting.\n",
                          sizeof("C quiting.\n"), HAL_MAX_DELAY);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"C entering.\n",
                          sizeof("C entering.\n"), HAL_MAX_DELAY);
    }

    vTaskDelete(NULL);
}
#endif  // _TEST_H_