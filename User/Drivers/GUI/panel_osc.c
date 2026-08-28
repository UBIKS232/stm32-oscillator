#include "liblcd.h"
#include "dmm.h"
#include "osc.h"
#include "panel_osc.h"
#include "panel_scale.h"
#include "panel_cursor.h"
#include "FreeRTOS.h"
#include "semphr.h"

WaveformParamTypeDef wave_form_param = {0};

void panel_osc_repaint(void) {
    extern SemaphoreHandle_t osc_data_mutex;

    wave_form_param.Waveform = osc_get_waveform_data();
    wave_form_param.WaveLength = osc_get_waveform_length();
    wave_form_param.SampleRate = osc_get_sample_rate();
    wave_form_param.Trigger = dmm_get_handle().trigger;
    wave_form_param.XScale = panel_scale_get_x();
    wave_form_param.YScale = panel_scale_get_y();
    wave_form_param.CursorX1Pct = panel_cursor_get_x1_pct();
    wave_form_param.CursorX2Pct = panel_cursor_get_x2_pct();
    wave_form_param.CursorY1Pct = panel_cursor_get_y1_pct();
    wave_form_param.CursorY2Pct = panel_cursor_get_y2_pct();

    xSemaphoreTake(osc_data_mutex, portMAX_DELAY);
    LCD_DrawWaveform(&wave_form_param);
    xSemaphoreGive(osc_data_mutex);
}