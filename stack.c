//
// Created by chenzheng on 21/01/2017.
//

#include<stddef.h>
#include "assert_v.h"
#include "mem.h"
#include "stack.h"

#define T Stack_T

T Stack_new(int num) {
    T stk;

    assert(num > 0);
    NEW(stk); /* allocate memory for this stack */
    stk->count = 0;
    stk->capacity = num;
    stk->head = NULL;

    return stk;
}

int Stack_empty(T stk) {
    assert(stk);
    return stk->count = 0; /* does not free occupied memory */
}

void Stack_push(T stk, void *x) {
    struct elem *t;

    assert(stk);
    assert(stk->count < stk->capacity);
    NEW(t);
    t->x = x;
    t->link = stk->head;
    stk->head = t;
    stk->count++;
}

void *Stack_pop(T stk) {
    void *x;
    struct elem *t;

    assert(stk);
    assert(stk->count > 0);
    t = stk->head;
    stk->head = t->link;
    stk->count--;
    x = t->x;
    FREE(t);
    return x;
}

void Stack_free(T *stk) {
    struct elem *t, *u;

    assert(stk && *stk);
    for (t = (*stk)->head; t; t = u) {
        u = t->link;
        FREE(t);
    }
    FREE(*stk);
}