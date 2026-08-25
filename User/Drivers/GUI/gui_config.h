#ifndef _GUI_CONFIG_H_
#define _GUI_CONFIG_H_

#include "FreeRTOS.h"
#include "event_groups.h"

#define LABEL_DMM_BIT ((EventBits_t)(0x1 << 0))
#define LABEL_PWR_BIT ((EventBits_t)(0x1 << 1))

#endif  // _GUI_CONFIG_H_