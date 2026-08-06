#ifndef _GENERAL_INCLUDE_H_
#define _GENERAL_INCLUDE_H_

extern void buzzer_init(void);

extern void key_1_init(void);
extern void key_2_init(void);
extern void key_3_init(void);
extern void key_4_init(void);
extern void key_up_init(void);
extern void key_down_init(void);

void init_hw(void) {
    buzzer_init();

    key_1_init();
    key_2_init();
    key_3_init();
    key_4_init();
    key_up_init();
    key_down_init();
}

#endif  // _GENERAL_INCLUDE_H_