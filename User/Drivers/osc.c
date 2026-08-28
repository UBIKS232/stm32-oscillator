#include "osc.h"
#include "gui_config.h"
#include "main.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"

#include <stdint.h>

#define OSC_WAVEFORM_LEN 1024

TimerHandle_t osc_timer = {0};
TimerHandle_t osc_force_timer = {0};

SemaphoreHandle_t osc_data_mutex = {0};
static uint16_t osc_raw_data[OSC_WAVEFORM_LEN] = {0};
static float osc_waveform_data[OSC_WAVEFORM_LEN] = {0};

float* osc_get_waveform_data(void) { return osc_waveform_data; }

int16_t osc_get_waveform_length(void) { return OSC_WAVEFORM_LEN; }

void osc_timer_callback(TimerHandle_t timer) {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
    HAL_NVIC_ClearPendingIRQ(EXTI3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void osc_force_timer_callback(TimerHandle_t timer) {
    __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_3);
}

void osc_set_sample_rate(uint16_t psc, uint16_t arr) {
    extern TIM_HandleTypeDef htim3;

    HAL_TIM_Base_Stop(&htim3);

    __HAL_TIM_SET_PRESCALER(&htim3, psc);
    __HAL_TIM_SET_AUTORELOAD(&htim3, arr);

    HAL_TIM_Base_Start(&htim3);
}

uint32_t osc_get_sample_rate(void) {
    extern TIM_HandleTypeDef htim3;

    uint16_t psc = htim3.Instance->PSC;
    uint16_t arr = htim3.Instance->ARR;

    // 采样率 = 72MHz / (psc+1) / (arr+1)
    return (uint32_t)(72.0e6f / (psc + 1) / (arr + 1));
}

void osc_init(void) {
    osc_timer = xTimerCreate("OSC", pdMS_TO_TICKS(10), pdTRUE, (uint8_t*)0x43,
                             osc_timer_callback);
    osc_force_timer = xTimerCreate("OSCFORCE", pdMS_TO_TICKS(100), pdTRUE,
                                   (uint8_t*)0x43, osc_force_timer_callback);
    osc_data_mutex = xSemaphoreCreateMutex();

    extern TIM_HandleTypeDef htim3;
    HAL_TIM_Base_Start(&htim3);

    xTimerStart(osc_force_timer, portMAX_DELAY);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    extern ADC_HandleTypeDef hadc1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (GPIO_Pin == osc_trigger_Pin) {
        // clear pending bit and stop exti3 it
        HAL_NVIC_DisableIRQ(EXTI3_IRQn);

        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)osc_raw_data,
                          sizeof(osc_raw_data));

        // start timer to block sample
        xTimerStartFromISR(osc_timer, &xHigherPriorityTaskWoken);
        xTimerResetFromISR(osc_force_timer, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    extern EventGroupHandle_t lcd_event;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (hadc->Instance == ADC1) {
        if (xSemaphoreTakeFromISR(osc_data_mutex, &xHigherPriorityTaskWoken) ==
            pdPASS) {
            for (int i = 0; i < OSC_WAVEFORM_LEN; i++) {
                osc_waveform_data[i] =
                    (osc_raw_data[i] / 4095.0f * 3.3f) * 2.0f - 2.5f;
            }
            xSemaphoreGiveFromISR(osc_data_mutex, &xHigherPriorityTaskWoken);
            xEventGroupSetBitsFromISR(lcd_event, PANEL_OSC_BIT,
                                      &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
