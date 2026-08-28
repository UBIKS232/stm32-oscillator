/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define key_up_Pin GPIO_PIN_13
#define key_up_GPIO_Port GPIOC
#define key_down_Pin GPIO_PIN_14
#define key_down_GPIO_Port GPIOC
#define key_3_Pin GPIO_PIN_15
#define key_3_GPIO_Port GPIOC
#define key_4_Pin GPIO_PIN_0
#define key_4_GPIO_Port GPIOC
#define led_3_Pin GPIO_PIN_1
#define led_3_GPIO_Port GPIOC
#define led_1_Pin GPIO_PIN_3
#define led_1_GPIO_Port GPIOC
#define osc_trigger_Pin GPIO_PIN_3
#define osc_trigger_GPIO_Port GPIOA
#define osc_trigger_EXTI_IRQn EXTI3_IRQn
#define dmm_50V_Pin GPIO_PIN_12
#define dmm_50V_GPIO_Port GPIOB
#define dmm_10V_Pin GPIO_PIN_13
#define dmm_10V_GPIO_Port GPIOB
#define dmm_2V_Pin GPIO_PIN_14
#define dmm_2V_GPIO_Port GPIOB
#define dmm_10KOhm_Pin GPIO_PIN_15
#define dmm_10KOhm_GPIO_Port GPIOB
#define dmm_100KOhm_Pin GPIO_PIN_6
#define dmm_100KOhm_GPIO_Port GPIOC
#define dmm_1MOhm_Pin GPIO_PIN_7
#define dmm_1MOhm_GPIO_Port GPIOC
#define buzzer_Pin GPIO_PIN_8
#define buzzer_GPIO_Port GPIOC
#define key_1_Pin GPIO_PIN_9
#define key_1_GPIO_Port GPIOC
#define key_2_Pin GPIO_PIN_8
#define key_2_GPIO_Port GPIOA
#define lcd_cs_Pin GPIO_PIN_12
#define lcd_cs_GPIO_Port GPIOC
#define lcd_nss_Pin GPIO_PIN_2
#define lcd_nss_GPIO_Port GPIOD
#define lcd_sck_Pin GPIO_PIN_3
#define lcd_sck_GPIO_Port GPIOB
#define lcd_rst_Pin GPIO_PIN_4
#define lcd_rst_GPIO_Port GPIOB
#define lcd_mosi_Pin GPIO_PIN_5
#define lcd_mosi_GPIO_Port GPIOB
#define lcd_bl_Pin GPIO_PIN_6
#define lcd_bl_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
