//
// Created by chenzheng on 20/06/2017.
//

#ifndef CLIB_ARRAYREP_H
#define CLIB_ARRAYREP_H

#define T Array_T

struct T {
    int length;
    int size;
    char *array;
};

extern void ArrayRep_init(T array, int length, int size, void *ary);

#undef T
#endif //CLIB_ARRAYREP_H
