#include "main.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

void lcd_bl_control(uint8_t bl_on) {
    if (bl_on) {
        HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, GPIO_PIN_RESET);
    }
}

static void lcd_reset(void) {
    // ST7789
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(1));
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(125));
}

extern SPI_HandleTypeDef hspi1;

static void lcd_send_cmd(uint8_t cmd) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

static void lcd_send_data(const uint8_t* data, uint16_t size) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (const uint8_t*)data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

void lcd_init(void) {
    lcd_bl_control(1);
    lcd_reset();

    // test
    lcd_send_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(125));
    lcd_send_cmd(0x3a);
    lcd_send_data((uint8_t[]){0x55}, 1);
    lcd_send_cmd(0x21);
    lcd_send_cmd(0x29);
}

void lcd_show_image(void) {
    // 设置图像的显示范围
    // 设置列范围60~179
    lcd_send_cmd(0x2a);
    lcd_send_data((uint8_t[]){0x00, 0x3c, 0x00, 0xb3}, 4);

    // 设置行范围80~239
    lcd_send_cmd(0x2b);
    lcd_send_data((uint8_t[]){0x00, 0x50, 0x00, 0xef}, 4);

    // 淡紫色色块的数据, gcc拓展写法
    static const uint16_t lightPurpleBlock[160 * 120] = {[0 ... 160 * 120 - 1] =
                                                             0xB0FF};

    // 发送命令写内存命令0x3c
    lcd_send_cmd(0x3C);
    lcd_send_data((uint8_t*)lightPurpleBlock, 160 * 120 * 2);
}
