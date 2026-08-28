#ifndef _GUI_CONFIG_H_
#define _GUI_CONFIG_H_

#include "FreeRTOS.h"
#include "event_groups.h"

// event bits
#define LABEL_DMM_BIT ((EventBits_t)(0x1 << 0))  // Bit for DMM label update
#define LABEL_PWR_BIT ((EventBits_t)(0x1 << 1))  // Bit for power label update
#define PANEL_GENERATOR_BIT \
    ((EventBits_t)(0x1 << 2))  // Bit for generator panel update
#define PANEL_RUNMODE_BIT \
    ((EventBits_t)(0x1 << 3))  // Bit for runmode panel update
#define PANEL_SCALE_BIT ((EventBits_t)(0x1 << 4))  // Bit for scale panel update
#define LABEL_SCALE_BIT ((EventBits_t)(0x1 << 5))  // Bit for scale label update
#define PANEL_CURSOR_BIT \
    ((EventBits_t)(0x1 << 6))  // Bit for cursor panel update
#define LABEL_CURSOR_BIT \
    ((EventBits_t)(0x1 << 8))  // Bit for cursor label update
#define PANEL_OSC_BIT \
    ((EventBits_t)(0x1 << 7))  // Bit for waveform panel update
#define ALL_REPAINT_BITS                                                       \
    (LABEL_DMM_BIT | LABEL_PWR_BIT | PANEL_GENERATOR_BIT | PANEL_RUNMODE_BIT | \
     PANEL_SCALE_BIT | LABEL_SCALE_BIT | PANEL_CURSOR_BIT | LABEL_CURSOR_BIT | \
     PANEL_OSC_BIT)  // All repaint event bits

// common panel dimensions and positions
#define PANEL_COMMON_X 250       // Common X origin for all panels
#define PANEL_COMMON_Y 8         // Common Y origin for topmost panel (runmode)
#define PANEL_COMMON_Y_SHIFT 57  // Vertical offset between panels
#define PANEL_COMMON_WIDTH \
    65  // Common width for all panel backgrounds and titles
#define PANEL_TITLE_HEIGHT 15  // Common height for panel title bars
#define PANEL_BG_HEIGHT 35     // Common height for panel background areas
#define PANEL_CURSOR_COMMON_HEIGHT \
    10  // Common height for cursor highlight bars

// Panel definitions (ordered: runmode, scale, cursor, generator)

// panel_runmode coordinates
#define PANEL_RUNMODE_X PANEL_COMMON_X         // X of runmode panel
#define PANEL_RUNMODE_Y PANEL_COMMON_Y         // Y of runmode panel
#define PANEL_RUNMODE_TITLE_X PANEL_RUNMODE_X  // X of runmode title background
#define PANEL_RUNMODE_TITLE_Y PANEL_RUNMODE_Y  // Y of runmode title background
#define PANEL_RUNMODE_TITLE_WIDTH \
    PANEL_COMMON_WIDTH  // Width of runmode title background
#define PANEL_RUNMODE_TITLE_HEIGHT \
    PANEL_TITLE_HEIGHT  // Height of runmode title background
#define PANEL_RUNMODE_TITLE_TEXT_X \
    (PANEL_RUNMODE_X + 2)  // X of runmode title text
#define PANEL_RUNMODE_TITLE_TEXT_Y \
    (PANEL_RUNMODE_Y + 13)                  // Y of runmode title text
#define PANEL_RUNMODE_BG_X PANEL_RUNMODE_X  // X of runmode main background
#define PANEL_RUNMODE_BG_Y \
    (PANEL_RUNMODE_Y +     \
     PANEL_RUNMODE_TITLE_HEIGHT)  // Y of runmode main background
#define PANEL_RUNMODE_BG_WIDTH \
    PANEL_COMMON_WIDTH  // Width of runmode main background
#define PANEL_RUNMODE_BG_HEIGHT \
    PANEL_BG_HEIGHT  // Height of runmode main background
#define PANEL_RUNMODE_CURSOR_X (PANEL_RUNMODE_X + 2)  // X of runmode cursor bar
#define PANEL_RUNMODE_CURSOR_Y \
    (PANEL_RUNMODE_BG_Y + 11)          // Y of runmode cursor bar
#define PANEL_RUNMODE_CURSOR_WIDTH 60  // Width of runmode cursor bar
#define PANEL_RUNMODE_CURSOR_HEIGHT \
    PANEL_CURSOR_COMMON_HEIGHT  // Height of runmode cursor bar
#define PANEL_RUNMODE_PARAM1_TEXT_X \
    (PANEL_RUNMODE_X + 5)  // X of runmode parameter text
#define PANEL_RUNMODE_PARAM1_TEXT_Y \
    (PANEL_RUNMODE_BG_Y + 20)  // Y of runmode parameter text

// panel_scale coordinates
#define PANEL_SCALE_X PANEL_COMMON_X  // X of scale panel
#define PANEL_SCALE_Y \
    (PANEL_COMMON_Y + \
     PANEL_COMMON_Y_SHIFT * 1)  // Y of scale panel background (now title Y)
#define PANEL_SCALE_BG_X PANEL_SCALE_X  // X of scale background
#define PANEL_SCALE_BG_Y \
    (PANEL_SCALE_Y +     \
     PANEL_TITLE_HEIGHT)  // Y of scale background (was PANEL_SCALE_Y)
#define PANEL_SCALE_BG_WIDTH PANEL_COMMON_WIDTH  // Width of scale background
#define PANEL_SCALE_BG_HEIGHT PANEL_BG_HEIGHT    // Height of scale background
#define PANEL_SCALE_TITLE_X PANEL_SCALE_X        // X of scale title background
#define PANEL_SCALE_TITLE_Y \
    PANEL_SCALE_Y  // Y of scale title background (was PANEL_SCALE_Y - 15)
#define PANEL_SCALE_TITLE_WIDTH \
    PANEL_COMMON_WIDTH  // Width of scale title background
#define PANEL_SCALE_TITLE_HEIGHT \
    PANEL_TITLE_HEIGHT  // Height of scale title background
#define PANEL_SCALE_TITLE_TEXT_X \
    (PANEL_SCALE_X + 15)  // X of scale title text (275)
#define PANEL_SCALE_TITLE_TEXT_Y \
    (PANEL_SCALE_TITLE_Y + 13)  // Y of scale title text (80)
#define PANEL_SCALE_PARAM1_TEXT_X \
    (PANEL_SCALE_X + 15)  // X of scale first parameter text (275)
#define PANEL_SCALE_PARAM1_TEXT_Y \
    (PANEL_SCALE_BG_Y + 16)  // Y of scale first parameter text (98)
#define PANEL_SCALE_PARAM2_TEXT_X \
    (PANEL_SCALE_X + 35)  // X of scale second parameter text (295)
#define PANEL_SCALE_PARAM2_TEXT_Y \
    (PANEL_SCALE_BG_Y + 16)         // Y of scale second parameter text (98)
#define PANEL_SCALE_CURSOR_WIDTH 9  // Width of scale cursor bar
#define PANEL_SCALE_CURSOR_HEIGHT \
    PANEL_CURSOR_COMMON_HEIGHT  // Height of scale cursor bar
#define PANEL_SCALE_CURSOR_PARAM1_X \
    (PANEL_SCALE_X + 14)  // X of scale cursor for first param (274)
#define PANEL_SCALE_CURSOR_PARAM1_Y \
    (PANEL_SCALE_BG_Y + 7)  // Y of scale cursor for first param (89)
#define PANEL_SCALE_CURSOR_PARAM2_X \
    (PANEL_SCALE_X + 34)  // X of scale cursor for second param (294)
#define PANEL_SCALE_CURSOR_PARAM2_Y \
    (PANEL_SCALE_BG_Y + 7)  // Y of scale cursor for second param (89)

// panel_cursor coordinates
#define PANEL_CURSOR_BG_X PANEL_COMMON_X  // X of cursor panel background
#define PANEL_CURSOR_BG_Y                          \
    ((PANEL_COMMON_Y + PANEL_COMMON_Y_SHIFT * 2) + \
     PANEL_TITLE_HEIGHT)  // Y of cursor panel background (was just the shift)
#define PANEL_CURSOR_BG_WIDTH \
    PANEL_COMMON_WIDTH  // Width of cursor panel background
#define PANEL_CURSOR_BG_HEIGHT \
    PANEL_BG_HEIGHT  // Height of cursor panel background
#define PANEL_CURSOR_TITLE_X PANEL_CURSOR_BG_X  // X of cursor title background
#define PANEL_CURSOR_TITLE_Y \
    (PANEL_COMMON_Y +        \
     PANEL_COMMON_Y_SHIFT *  \
         2)  // Y of cursor title background (was PANEL_CURSOR_BG_Y - 15)
#define PANEL_CURSOR_TITLE_WIDTH \
    PANEL_COMMON_WIDTH  // Width of cursor title background
#define PANEL_CURSOR_TITLE_HEIGHT \
    PANEL_TITLE_HEIGHT  // Height of cursor title background
#define PANEL_CURSOR_TITLE_TEXT_X \
    (PANEL_CURSOR_BG_X + 12)  // X of cursor title text (272)
#define PANEL_CURSOR_TITLE_TEXT_Y \
    (PANEL_CURSOR_TITLE_Y + 13)  // Y of cursor title text (140)
#define PANEL_CURSOR_PARAM1_TEXT_X \
    (PANEL_CURSOR_BG_X + 12)  // X of cursor param1 text (272)
#define PANEL_CURSOR_PARAM1_TEXT_Y \
    (PANEL_CURSOR_BG_Y + 16)  // Y of cursor param1 text (158)
#define PANEL_CURSOR_PARAM2_TEXT_X \
    (PANEL_CURSOR_BG_X + 33)  // X of cursor param2 text (293)
#define PANEL_CURSOR_PARAM2_TEXT_Y \
    (PANEL_CURSOR_BG_Y + 16)  // Y of cursor param2 text (158)
#define PANEL_CURSOR_PARAM3_TEXT_X \
    (PANEL_CURSOR_BG_X + 12)  // X of cursor param3 text (272)
#define PANEL_CURSOR_PARAM3_TEXT_Y \
    (PANEL_CURSOR_BG_Y + 29)  // Y of cursor param3 text (171)
#define PANEL_CURSOR_PARAM4_TEXT_X \
    (PANEL_CURSOR_BG_X + 33)  // X of cursor param4 text (293)
#define PANEL_CURSOR_PARAM4_TEXT_Y \
    (PANEL_CURSOR_BG_Y + 29)          // Y of cursor param4 text (171)
#define PANEL_CURSOR_CURSOR_WIDTH 16  // Width of cursor highlight bar
#define PANEL_CURSOR_CURSOR_HEIGHT \
    PANEL_CURSOR_COMMON_HEIGHT  // Height of cursor highlight bar
#define PANEL_CURSOR_CURSOR_PARAM1_X \
    (PANEL_CURSOR_BG_X + 11)  // X of cursor bar for param1 (271)
#define PANEL_CURSOR_CURSOR_PARAM1_Y \
    (PANEL_CURSOR_BG_Y + 6)  // Y of cursor bar for param1 (148)
#define PANEL_CURSOR_CURSOR_PARAM2_X \
    (PANEL_CURSOR_BG_X + 32)  // X of cursor bar for param2 (292)
#define PANEL_CURSOR_CURSOR_PARAM2_Y \
    (PANEL_CURSOR_BG_Y + 6)  // Y of cursor bar for param2 (148)
#define PANEL_CURSOR_CURSOR_PARAM3_X \
    (PANEL_CURSOR_BG_X + 11)  // X of cursor bar for param3 (271)
#define PANEL_CURSOR_CURSOR_PARAM3_Y \
    (PANEL_CURSOR_BG_Y + 19)  // Y of cursor bar for param3 (161)
#define PANEL_CURSOR_CURSOR_PARAM4_X \
    (PANEL_CURSOR_BG_X + 32)  // X of cursor bar for param4 (292)
#define PANEL_CURSOR_CURSOR_PARAM4_Y \
    (PANEL_CURSOR_BG_Y + 19)  // Y of cursor bar for param4 (161)

// panel_generator coordinates
#define PANEL_GEN_X PANEL_COMMON_X  // X of generator panel
#define PANEL_GEN_Y   \
    (PANEL_COMMON_Y + \
     PANEL_COMMON_Y_SHIFT * 3)         // Y of generator panel background
#define PANEL_GEN_TITLE_X PANEL_GEN_X  // X of generator title background
#define PANEL_GEN_TITLE_Y PANEL_GEN_Y  // Y of generator title background
#define PANEL_GEN_TITLE_WIDTH \
    PANEL_COMMON_WIDTH  // Width of generator title background
#define PANEL_GEN_TITLE_HEIGHT \
    PANEL_TITLE_HEIGHT  // Height of generator title background
#define PANEL_GEN_TITLE_TEXT_X (PANEL_GEN_X + 2)   // X of generator title text
#define PANEL_GEN_TITLE_TEXT_Y (PANEL_GEN_Y + 13)  // Y of generator title text
#define PANEL_GEN_BG_X PANEL_GEN_X  // X of generator main background
#define PANEL_GEN_BG_Y \
    (PANEL_GEN_Y + PANEL_GEN_TITLE_HEIGHT)  // Y of generator main background
#define PANEL_GEN_BG_WIDTH \
    PANEL_COMMON_WIDTH  // Width of generator main background
#define PANEL_GEN_BG_HEIGHT \
    PANEL_BG_HEIGHT                // Height of generator main background
#define PANEL_GEN_CURSOR_WIDTH 60  // Width of generator cursor bar
#define PANEL_GEN_CURSOR_HEIGHT \
    PANEL_CURSOR_COMMON_HEIGHT  // Height of generator cursor bar
#define PANEL_GEN_CURSOR_PARAM1_X \
    (PANEL_GEN_X + 2)  // X of generator cursor for param1
#define PANEL_GEN_CURSOR_PARAM1_Y \
    (PANEL_GEN_BG_Y + 3)  // Y of generator cursor for param1
#define PANEL_GEN_CURSOR_PARAM2_X \
    (PANEL_GEN_X + 2)  // X of generator cursor for param2
#define PANEL_GEN_CURSOR_PARAM2_Y \
    (PANEL_GEN_BG_Y + 13)  // Y of generator cursor for param2
#define PANEL_GEN_PARAM1_TEXT_X \
    (PANEL_GEN_X + 3)  // X of generator first parameter text
#define PANEL_GEN_PARAM1_TEXT_Y \
    (PANEL_GEN_BG_Y + 13)  // Y of generator first parameter text
#define PANEL_GEN_PARAM2_TEXT_X \
    (PANEL_GEN_X + 3)  // X of generator second parameter text
#define PANEL_GEN_PARAM2_TEXT_Y \
    (PANEL_GEN_BG_Y + 23)  // Y of generator second parameter text

// Label definitions

// label_dmm coordinates
#define LABEL_DMM_X 110                      // X of DMM label background
#define LABEL_DMM_Y 215                      // Y of DMM label background
#define LABEL_DMM_BG_X LABEL_DMM_X           // X of DMM background (same)
#define LABEL_DMM_BG_Y LABEL_DMM_Y           // Y of DMM background (same)
#define LABEL_DMM_BG_WIDTH 120               // Width of DMM background
#define LABEL_DMM_BG_HEIGHT 15               // Height of DMM background
#define LABEL_DMM_TEXT_X (LABEL_DMM_X + 6)   // X of DMM text
#define LABEL_DMM_TEXT_Y (LABEL_DMM_Y + 11)  // Y of DMM text

// label_pwr coordinates
#define LABEL_PWR_X 6                        // X of power label background
#define LABEL_PWR_Y 215                      // Y of power label background
#define LABEL_PWR_BG_X LABEL_PWR_X           // X of power background (same)
#define LABEL_PWR_BG_Y LABEL_PWR_Y           // Y of power background (same)
#define LABEL_PWR_BG_WIDTH 90                // Width of power background
#define LABEL_PWR_BG_HEIGHT 15               // Height of power background
#define LABEL_PWR_TEXT_X (LABEL_PWR_X + 6)   // X of power text
#define LABEL_PWR_TEXT_Y (LABEL_PWR_Y + 11)  // Y of power text

// label_scale coordinates
#define LABEL_SCALE_BG_X 187  // X of scale label background
#define LABEL_SCALE_BG_Y \
    PANEL_COMMON_Y  // Y of scale label background (aligned with panels)
#define LABEL_SCALE_BG_WIDTH 60   // Width of scale label background
#define LABEL_SCALE_BG_HEIGHT 25  // Height of scale label background
#define LABEL_SCALE_TEXT_X \
    (LABEL_SCALE_BG_X + 8)  // X of scale label text (195)
#define LABEL_SCALE_TEXT1_Y \
    (LABEL_SCALE_BG_Y + 12)  // Y of first line of scale text (18)
#define LABEL_SCALE_TEXT2_Y \
    (LABEL_SCALE_BG_Y + 23)  // Y of second line of scale text (29)

// label_cursor coordinates
#define LABEL_CURSOR_BG_X 8  // X of cursor label background
#define LABEL_CURSOR_BG_Y \
    PANEL_COMMON_Y                 // Y of cursor label background (aligned)
#define LABEL_CURSOR_BG_WIDTH 174  // Width of cursor label background
#define LABEL_CURSOR_BG_HEIGHT 10  // Height of first row (cyan)
#define LABEL_CURSOR_BG2_Y \
    (LABEL_CURSOR_BG_Y + 15)  // Y of second row (magenta) (21)
#define LABEL_CURSOR_BG3_Y \
    (LABEL_CURSOR_BG_Y + 26)     // Y of third row (white) (32)
#define LABEL_CURSOR_TEXT_X1 13  // X of first column text
#define LABEL_CURSOR_TEXT_X2 99  // X of second column text
#define LABEL_CURSOR_TEXT_Y1 \
    (LABEL_CURSOR_BG_Y + 9)  // Y of first row text (15)
#define LABEL_CURSOR_TEXT_Y2 \
    (LABEL_CURSOR_BG_Y + 24)  // Y of second row text (30)
#define LABEL_CURSOR_DELTA_X (LABEL_CURSOR_BG_X + 0)  // X of delta label (8)
#define LABEL_CURSOR_DELTA_Y \
    (LABEL_CURSOR_BG_X + 112)  // Y of delta label (120) [note: uses BG_X]
#define LABEL_CURSOR_DELTA_Y_ROW \
    (LABEL_CURSOR_BG_Y + 36)  // Y of delta value row (42)

#endif  //_GUI_CONFIG_H_