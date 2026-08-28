#ifndef _DMM_H_
#define _DMM_H_

// digital multi meter

typedef enum dmm_range {
    DMM_RANGE_2VOLT = 0,
    DMM_RANGE_10VOLT,
    DMM_RANGE_50VOLT,
    DMM_RANGE_1KOHM,
    DMM_RANGE_10KOHM,
    DMM_RANGE_100KOHM,
    DMM_RANGE_1MOHM,
    DMM_RANGE_COUNT
} dmm_range_t;

typedef struct dmm_handle {
    dmm_range_t range;
    float pwr;    // 稳压源
    float value;  // 万用表读数
    float trigger;  // osc触发电压
} dmm_handle_t;

void dmm_init(void);
dmm_handle_t dmm_get_handle(void);

#endif  // _DMM_H_