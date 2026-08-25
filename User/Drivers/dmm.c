#include "main.h"
#include "dmm.h"
#include "gui_config.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"

#define DMM_QUE_LENGTH 1
#define DMM_QUE_ITEM_SIZE sizeof(dmm_handle_t)
QueueHandle_t dmm_mail = {0};

static void dmm_get_range(dmm_range_t* range) {
    *range = DMM_RANGE_1KOHM;

    if (HAL_GPIO_ReadPin(dmm_2V_GPIO_Port, dmm_2V_Pin)) {
        *range = DMM_RANGE_2VOLT;
    } else if (HAL_GPIO_ReadPin(dmm_10V_GPIO_Port, dmm_10V_Pin)) {
        *range = DMM_RANGE_10VOLT;
    } else if (HAL_GPIO_ReadPin(dmm_50V_GPIO_Port, dmm_50V_Pin)) {
        *range = DMM_RANGE_50VOLT;
    } else if (HAL_GPIO_ReadPin(dmm_10KOhm_GPIO_Port, dmm_10KOhm_Pin)) {
        *range = DMM_RANGE_10KOHM;
    } else if (HAL_GPIO_ReadPin(dmm_100KOhm_GPIO_Port, dmm_100KOhm_Pin)) {
        *range = DMM_RANGE_100KOHM;
    } else if (HAL_GPIO_ReadPin(dmm_1MOhm_GPIO_Port, dmm_1MOhm_Pin)) {
        *range = DMM_RANGE_1MOHM;
    } else {
        *range = DMM_RANGE_1KOHM;
    }
}

#define RES1 (2e6f)

static void dmm_calc_data(dmm_handle_t* handle, float readings) {
    dmm_range_t range = handle->range;
    float* value = &handle->value;
    float vadc = readings;

    float vin = vadc * 2.0f - 2.0f;
    float rx = 0.0f;

    switch (range) {
        case DMM_RANGE_2VOLT:
            *value = vin * 1.0f;
            break;
        case DMM_RANGE_10VOLT:
            *value = vin * 5.0f;
            break;
        case DMM_RANGE_50VOLT:
            *value = vin * 25.0f;
            break;
        case DMM_RANGE_1KOHM:
            rx = vin / (2.5f - vin) * 200.0f;
            *value = RES1 * rx / (RES1 - rx);
            break;
        case DMM_RANGE_10KOHM:
            rx = vin / (2.5f - vin) * 2000.0f;
            *value = RES1 * rx / (RES1 - rx);
            break;
        case DMM_RANGE_100KOHM:
            rx = vin / (2.5f - vin) * 20000.0f;
            *value = RES1 * rx / (RES1 - rx);
            break;
        case DMM_RANGE_1MOHM:
            rx = vin / (2.5f - vin) * 200000.0f;
            *value = RES1 * rx / (RES1 - rx);
            break;
        default:
            *value = 2.7182818f;  // error
            break;
    }
}

void dmm_init(void) {
    extern TIM_HandleTypeDef htim2;
    extern ADC_HandleTypeDef hadc1;

    dmm_mail = xQueueCreate(DMM_QUE_LENGTH, DMM_QUE_ITEM_SIZE);

    HAL_TIM_Base_Start(&htim2);

    HAL_ADCEx_Calibration_Start(&hadc1);
    // set ADCx->CR1JEOCIE
    HAL_ADCEx_InjectedStart_IT(&hadc1);
}

dmm_handle_t dmm_get_handle(void) {
    dmm_handle_t handle = {0};
    xQueuePeek(dmm_mail, &handle, portMAX_DELAY);
    return handle;
}

#define VREF 1.205f
#define PWR_MULTIPLIER 13.0f

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc) {
    extern ADC_HandleTypeDef hadc1;
    extern EventGroupHandle_t lcd_event;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (hadc->Instance == ADC1) {
        // get readings, with calibration
        float readings_vref =
            HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
        float readings_vin =
            HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
        float readings_pwr =
            HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
        readings_vin = (readings_vin / readings_vref) * VREF;
        readings_pwr = (readings_pwr / readings_vref) * VREF * PWR_MULTIPLIER;

        // process data
        dmm_handle_t handle = {0};
        handle.pwr = readings_pwr;
        dmm_get_range(&handle.range);
        dmm_calc_data(&handle, readings_vin);

        // set ADCx->CR1JEOCIE
        HAL_ADCEx_InjectedStart_IT(&hadc1);

        // send value and update event
        xEventGroupSetBitsFromISR(lcd_event, LABEL_DMM_BIT | LABEL_PWR_BIT,
                                  &xHigherPriorityTaskWoken);
        xQueueOverwriteFromISR(dmm_mail, &handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
