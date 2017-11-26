//
// Created by chenzheng on 11/11/2017.
//

#include <stdio.h>
#include "array.h"

int main()
{
    Array_T array;
    // 0x44 = 68 0x33 = 51
    int a = 0x11223344, b = 0x55667788, c = 0x99AABBCCDD, d = 0xEEFFEEFF;
    char* presult;

    array = Array_new(10, sizeof(int));
    int num = Array_length(array);
    printf("The number of elements is %d\n", num);
    int size = Array_size(array);
    printf("The size of each element is %d\n", size);
    Array_put(array, 0, &a);
    Array_put(array, 1, &b);
    Array_put(array, 2, &c);
    Array_put(array, 3, &d);
    Array_reshape(&array, 10, sizeof(char));
    size = Array_size(array);
    printf("The size of each element is %d\n", size);
    presult = Array_get(array, 0);
    printf("result = %c\n", *presult);

    Array_free(&array);

    return 0;
}
