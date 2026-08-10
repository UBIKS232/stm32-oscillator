#include "test_sub_main.h"

static int static_global_var_1 = 100;

static int square(int input){
    return (input * input);
}

int get_next_square(){
    static int num = 1;

    int ret;
    ret = square(num);

    num++;
    return ret;
}