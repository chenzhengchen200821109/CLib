#include <stdio.h>
#include "mem.h"

int main()
{
    int a;
    int* pa = &a;
    FREE(pa);
    int* pb;
    pa = (int *)ALLOC(100);
    pb = pa;
    FREE(pa);
    FREE(pb);
    printf("Hello world\n");
    return 0;
}

