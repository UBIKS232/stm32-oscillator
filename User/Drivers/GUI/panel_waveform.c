#include "panel_waveform.h"
#include "liblcd.h"
#include "panel_scale.h"
#include "panel_cursor.h"

void panel_waveform_repaint(void) {
    WaveformParamTypeDef waveformParam = {0};

    waveformParam.XScale = panel_scale_get_x();
    waveformParam.YScale = panel_scale_get_y();
    waveformParam.CursorX1Pct = panel_cursor_get_x1_pct();
    waveformParam.CursorX2Pct = panel_cursor_get_x2_pct();
    waveformParam.CursorY1Pct = panel_cursor_get_y1_pct();
    waveformParam.CursorY2Pct = panel_cursor_get_y2_pct();

    LCD_DrawWaveform(&waveformParam);
}