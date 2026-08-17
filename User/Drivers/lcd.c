#include "lcd.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

extern SPI_HandleTypeDef hspi1;

static void lcd_send_cmd(uint8_t cmd) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

static void lcd_send_data(const uint8_t* data, uint16_t size) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

static void lcd_reset(void) {
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(1));
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(125));
}

void lcd_bl_control(uint8_t on) {
    HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, on);
}

void lcd_init(void) {
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
    // 设置列范围60~179
    lcd_send_cmd(0x2a);
    lcd_send_data((uint8_t[]){0x00, 0x3c, 0x00, 0xb3}, 4);
    // 设置行范围80~239
    lcd_send_cmd(0x2b);
    lcd_send_data((uint8_t[]){0x00, 0x50, 0x00, 0xef}, 4);
    // 淡紫色色块的数据
    static const uint16_t lightPurpleBlock[160 * 120] = {[0 ... 160 * 120 - 1] =
                                                             0xFFB0};
    // 发送命令写内存命令0x3c
    // lcd_send_cmd(0x3C);
    lcd_send_cmd(0x2c);
    lcd_send_data((uint8_t*)lightPurpleBlock, 160 * 120 * 2);
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