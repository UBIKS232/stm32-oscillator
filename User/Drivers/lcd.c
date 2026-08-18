#include "lcd.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t lcd_sem = NULL;  // - lcd_task

static void lcd_reset(void) {
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(1));
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(125));
}

void lcd_bl_control(uint8_t on) {
    HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, on);
}

static void lcd_send_cmd(uint8_t cmd) {
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit_DMA(&hspi1, &cmd, 1);
    xSemaphoreTake(lcd_sem, portMAX_DELAY);  // 进入阻塞

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

static void lcd_send_data(const uint8_t* data, uint16_t size) {
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)data, size);
    xSemaphoreTake(lcd_sem, portMAX_DELAY);  // 进入阻塞

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
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

#define BLOCK_SIZE 100

void lcd_init(void) {
    // 创建lcd信号量, 后面会立即用到
    if (lcd_sem == NULL) {
        lcd_sem = xSemaphoreCreateBinary();
    }
    if (!lcd_sem) return;

    // ST7789
    lcd_bl_control(1);

    lcd_reset();

    // 退出睡眠模式
    lcd_send_cmd(0x11);

    vTaskDelay(pdMS_TO_TICKS(125));

    // 设置色彩格式为RGB565
    lcd_send_cmd(0x3a);
    lcd_send_data((uint8_t[]){0x55}, 1);

    // 关闭反向显示(对某些变种型号的ST7789)
    // lcd_send_cmd(0x21);

    // 开启屏幕显示
    lcd_send_cmd(0x29);

    // test
    // // 设置列范围
    // lcd_send_cmd(0x2a);
    // lcd_send_data((uint8_t[]){0x00, 0x00, 0x00, 0xef}, 4);
    // // 设置行范围
    // lcd_send_cmd(0x2b);
    // lcd_send_data((uint8_t[]){0x00, 0x00, 0x01, 0x3f}, 4);
    // // 色块数据
    // static const uint16_t refresh_screen[320 * 240] = {[0 ...(320 * 240 - 1)]
    // =
    //                                                        0xffff};
    // // 发送命令写内存, 命令0x2c复位指针
    // lcd_send_cmd(0x2c);
    // lcd_send_data((uint8_t*)refresh_screen, sizeof(refresh_screen));

    // 应该显示一个100*100的色块
    // 按照示波器的使用方向看应该是左右两边分别为红色和绿色
    // 设置列范围
    lcd_send_cmd(0x2a);
    lcd_send_data((uint8_t[]){0x00, 0x46, 0x00, 0xA9}, 4);
    // 设置行范围
    lcd_send_cmd(0x2b);
    lcd_send_data((uint8_t[]){0x00, 0x6E, 0x00, 0xD1}, 4);
    // 色块数据
    // 注意: 要按照倒置的字节顺序写颜色, 如:
    // green: 0x07e0 -> 0xe007, red: 0xf800 -> 0x00f8
    // 原因: ST7789的RGB565格式要求先发送高字节(MSB), 再发送低字节(LSB)
    static const uint16_t block[BLOCK_SIZE * BLOCK_SIZE] = {
        [0 ...(BLOCK_SIZE * BLOCK_SIZE / 2 - 1)] = 0xe007,
        [(BLOCK_SIZE * BLOCK_SIZE / 2)...(BLOCK_SIZE * BLOCK_SIZE - 1)] =
            0x00f8};
    // 发送命令写内存, 命令0x2c复位指针
    lcd_send_cmd(0x2c);
    lcd_send_data((uint8_t*)block, sizeof(block));
}

#include "A.h"
#include "B.h"
static uint8_t image_flg = 0;

void lcd_show_image(void) {
    if (image_flg == 0) {
        image_flg = 1;
        // 设置列范围
        lcd_send_cmd(0x2a);
        lcd_send_data((uint8_t[])A_COL_ARRAY, 4);
        // 设置行范围
        lcd_send_cmd(0x2b);
        lcd_send_data((uint8_t[])A_ROW_ARRAY, 4);
        // 写屏幕控制器内存
        // lcd_send_cmd(0x3c); // 从上次结束位置写入
        lcd_send_cmd(0x2c);  // 复位地址指针, 从设置的起始点写入
        lcd_send_data((uint8_t*)&A, sizeof(A));
    } else {
        image_flg = 0;
        lcd_send_cmd(0x2a);
        lcd_send_data((uint8_t[])B_COL_ARRAY, 4);
        lcd_send_cmd(0x2b);
        lcd_send_data((uint8_t[])B_ROW_ARRAY, 4);
        // 写屏幕控制器内存
        // lcd_send_cmd(0x3c);
        lcd_send_cmd(0x2c);
        lcd_send_data((uint8_t*)&B, sizeof(B));
    }
}