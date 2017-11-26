//
// Created by chenzheng on 05/03/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include "assert_v.h"
#include "except_v.h"

#define T Except_T

Except_Frame *Except_stack = NULL;

void Except_raise(const T *e, const char *file, int line) {
    Except_Frame *p = Except_stack;
    
    assert(e);
    if (p == NULL) {
        fprintf(stderr, "Uncaught exception");
        if (e->reason) 
            fprintf(stderr, " %s", e->reason);
        else
            fprintf(stderr, " at 0x%p", e);
        if (file && line > 0)
            fprintf(stderr, " raise at %s: %d\n", file, line);
        fprintf(stderr, "aborting...\n");
        fflush(stderr);
        abort();
    } 
}
