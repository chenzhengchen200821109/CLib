//
// Created by chenzheng on 21/01/2017.
//

#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#define T Stack_T
typedef struct T *T;

struct T {
    int count; /* number of elements on stack */
    int capacity; /* max number of elements contained on stack */
    struct elem {
        void *x;
        struct elem *link;
    } *head;
};

extern T Stack_new(int num);
extern int Stack_empty(T stk);
extern void Stack_push(T stk, void *x);
extern void *Stack_pop(T stk);
extern void Stack_free(T *stk);

#undef T
#endif
