#include "generator.h"
#include "generator_data.h"
#include "main.h"
#include "panel_generator.h"

#include <stdint.h>
#include <math.h>

#define GEN_DATA_LEN 1000

static uint16_t gen_data[GEN_DATA_LEN] = {0};
static uint16_t gen_data_len = GEN_DATA_LEN;

// waveform
static panel_generator_state_t state = PANEL_GENRATOR_OFF;
// f
static uint32_t f = 10e3;

static void gen_sample_rate(void) {
    extern TIM_HandleTypeDef htim5;

    uint32_t rate = GEN_DATA_LEN * f;  // !!理想的采样率是波形频率*点数
    gen_data_len = GEN_DATA_LEN;       // 默认波形长度1000点

    if (rate > 500e3)  // 如果采样率大于500kHz(硬件限制)
    {
        rate = 500e3;

        rate = round((float)rate / f);  // 重新计算波形长度
    }

    // 根据采样率计算TIM的PSC和ARR
    uint16_t psc = 0;
    uint16_t arr = 72e6 / rate;

    // 将PSC和ARR的值设置到硬件当中
    __HAL_TIM_SET_PRESCALER(&htim5, psc);   // 设置PSC的值
    __HAL_TIM_SET_AUTORELOAD(&htim5, arr);  // 设置ARR的值
}

static void gen_restart(void) {
    extern DAC_HandleTypeDef hdac;
    extern TIM_HandleTypeDef htim5;

    // 先停止定时器和DAC
    HAL_TIM_Base_Stop(&htim5);
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

    gen_sample_rate();  // 调整采样率

    // 开启DAC
    switch (state) {
        case PANEL_GENRATOR_SIN:
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (const uint32_t*)wave_sin,
                              GEN_DATA_LEN, DAC_ALIGN_12B_R);
            break;
        case PANEL_GENRATOR_TRIANGLE:
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (const uint32_t*)wave_tri,
                              GEN_DATA_LEN, DAC_ALIGN_12B_R);
            break;
        case PANEL_GENRATOR_SQUARE:
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (const uint32_t*)wave_rect,
                              GEN_DATA_LEN, DAC_ALIGN_12B_R);
            break;
        default:
        case PANEL_GENRATOR_OFF:
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (const uint32_t*)wave_off,
                              GEN_DATA_LEN, DAC_ALIGN_12B_R);
            break;
    }

    // 开启TIM5
    HAL_TIM_Base_Start(&htim5);
}
void gen_set_freq(uint32_t freq) {
    f = freq;
    gen_restart();
}

void gen_set_waveform(panel_generator_state_t waveform) {
    state = waveform;
    gen_restart();
}

void gen_init(void) { gen_restart(); }