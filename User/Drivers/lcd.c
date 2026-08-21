#include "lcd.h"
#include "liblcd.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t lcd_sem = NULL;  // - lcd_task

void lcd_bl_control(uint8_t on) {
    HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, on);
}

static void lcd_reset(void) {
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(1));
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(125));
}

static void lcd_send_cmd(uint8_t cmd) {
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit_DMA(&hspi1, &cmd, 1);
    xSemaphoreTake(lcd_sem, portMAX_DELAY);  // 进入阻塞

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

static void lcd_send_data(uint8_t* data, uint16_t size) {
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)data, size);
    xSemaphoreTake(lcd_sem, portMAX_DELAY);  // 进入阻塞

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

LCD_InitTypeDef lcd_init_t = {0};

void lcd_init(void) {
    // 创建lcd信号量
    if (lcd_sem == NULL) {
        lcd_sem = xSemaphoreCreateBinary();
    }
    if (!lcd_sem) return;

    // 使用api初始化lcd
    lcd_init_t.reset_callback = lcd_reset;
    lcd_init_t.send_command_callback = lcd_send_cmd;
    lcd_init_t.send_data_callback = lcd_send_data;
    LCD_Init(&lcd_init_t);

    // - 特殊设置 -

    // 开启背光
    lcd_bl_control(1U);

    // 关闭反向显示(对某些变种型号的ST7789控制的屏幕)
    // INVOFF (20h): Display Inversion Off .
    lcd_send_cmd(0x20);

    // - 特殊设置 -

    // test
    LCD_SetBrush(0x000000);
    LCD_Clear();
    LCD_SetPen(0x00ff00);
    LCD_SetCursor(120, 120);
    LCD_PrintString("Hello lcd!");
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    extern SPI_HandleTypeDef hspi1;

    if (hspi == &hspi1) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        /* The event has occurred, use the semaphore to unblock the task so the
        task can process the event. */
        xSemaphoreGiveFromISR(lcd_sem, &xHigherPriorityTaskWoken);

        /* Clear the interrupt here. */

        /* Now the task has been unblocked a context switch should be performed
        if xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax
        required to perform a context switch from an ISR varies from port to
        port, and from compiler to compiler. Check the web documentation and
        examples for the port being used to find the syntax required for your
        application. */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
