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
    // HAL_Delay(1);
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(125));
    // HAL_Delay(125);
}

extern SPI_HandleTypeDef hspi1;

void lcd_send_cmd(uint8_t cmd) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

void lcd_send_data(const uint8_t* data, uint16_t size) {
    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (const uint8_t*)data, sizeof(size), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

void lcd_init(void) {
    lcd_bl_control(1);
    lcd_reset();

    // test
    lcd_send_cmd(0x11);
    vTaskDelay(pdMS_TO_TICKS(125));
    // HAL_Delay(125);
    lcd_send_cmd(0x3a);
    lcd_send_data((uint8_t[]){0x55}, 1);
    lcd_send_cmd(0x21);
    lcd_send_cmd(0x29);
}
