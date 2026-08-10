#include <stdlib.h>
#include <stdio.h>
#include "test_sub_main.h"

int global_var_1 = 36;
int global_var_2[100];

int main(){
    int n;

    scanf("%d", &n);
    int *p_buffer = (int *)malloc(sizeof(int) * n);

    for(int i = 0; i < n; i++){
        p_buffer[i]=get_next_square();
    }

    for(int i = 0; i < n; i++){
        printf("%d,", p_buffer[i]);
    }
    printf("\n");

    i=10;

    return 0;
}

int another_func(){
    n++;
}