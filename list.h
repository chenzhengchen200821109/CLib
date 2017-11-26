//
// Created by chenzheng on 23/06/2017.
//

#ifndef CLIB_LIST_H
#define CLIB_LIST_H

#define T List_T
typedef struct T *T;

struct T {
    T rest;
    void *first;
};

extern T List_append_v1(T list, T tail);
extern T List_append_v2(T list, T tail);
extern T List_copy_v1(T list);
extern T List_copy_v2(T list);
extern T List_list_v1(void *x, ...); // two versions of implementation of List_list()
extern T List_list_v2(void *x, ...);
extern T List_pop(T list, void **x);
extern T List_push(T list, void *x);
extern T List_reverse_v1(T list);
extern T List_reverse_v2(T *list);
extern int List_length(T list);
extern void List_free(T *list); // destroy of list
extern void List_map(T list, void apply(void **x, void *cl), void *cl);
extern void **List_toArray(T list, void *end);

#undef T
#endif //CLIB_LIST_H
