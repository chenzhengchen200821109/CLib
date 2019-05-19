//
// Created by chenzheng on 23/06/2017.
//

#include "list.h"
#include "mem.h"
#include "assert_v.h"
#include <stdarg.h>
#include <stddef.h>

#define T List_T

T List_push(T list, void *x)
{
    T p;

    NEW(p); // memory allocation
    p->first = x;
    p->rest = list; // add to head of list

    return p;
}

// using double pointer for List_list() implementation
T List_list_v1(void *x, ...)
{
    va_list ap;
    T list, *p = &list;

    va_start(ap, x);
    for ( ; x; x = va_arg(ap, void *)) {
        NEW(*p);
        (*p)->first = x;
        p = &(*p)->rest;
    }
    *p = NULL;
    va_end(ap);

    return list;
}

T List_list_v2(void *x, ...)
{
    va_list ap;
    T list = NULL, prev, node;

    va_start(ap, x);
    for ( ; x; x = va_arg(ap, void *)) {
        NEW(node);
        node->first = x;
        node->rest = NULL;
        if (list == NULL)
            list = node;
        else
            prev->rest = node;
        prev = node;
    }
    va_end(ap);

    return list;
}

T List_append_v1(T list, T tail)
{
    T *p = &list;

    while (*p)
        p = &(*p)->rest;
    *p = tail;

    return list;
}

T List_append_v2(T list, T tail)
{
    T head = NULL, prev, node;

    for ( ; list; list = list->rest) {
        NEW(node);
        node->first = list->first;
        node->rest = NULL;
        if (head == NULL)
            head = node;
        else
            prev->rest = node;
        prev = node;
    }
    for (; tail; tail = tail->rest) {
        NEW(node);
        node->first = tail->first;
        node->rest = NULL;
        if (head == NULL)
            head = node;
        else
            prev ->rest = node;
        prev = node;
    }

    return head;
}

T List_copy_v1(T list)
{
    T head, *p = &head;

    for ( ; list; list = list->rest) {
        NEW(*p);
        (*p)->first = list->first;
        p = &(*p)->rest;
    }
    *p = NULL;

    return head;
}

T List_copy_v2(T list)
{
    T head = NULL, prev, node;

    for ( ; list; list = list->rest) {
        NEW(node);
        node->first = list->first;
        node->rest = NULL;
        if (head == NULL)
            head = node;
        else
            prev->rest = node;
        prev = node;
    }

    return head;
}

T List_pop(T list, void **x)
{
    if (list) {
        T head = list->rest;
        if (x)
            *x = list->first;
        FREE(list);
        return head;
    } else
        return list;
}

T List_reverse_v1(T list)
{
    T head = NULL, next;

    for ( ; list; list = next) {
        next = list->rest;
        list->rest = head;
        head = list;
    }

    return head;
}

// using double pointer to implement List_reverse()
T List_reverse_v2(T *list)
{
    T head = NULL, next;

    for ( ; *list; *list = next) {
        next = (*list)->rest;
        (*list)->rest = head;
        head = *list;
    } // *list points to NULL after for loop
    *list = head;

    return head;
}

int List_length(T list)
{
    int n;

    for (n = 0; list; list = list->rest)
        n++;

    return n;
}

void List_free(T *list)
{
    T next;

    assert(list);
    for ( ; *list; *list = next) {
        next = (*list)->rest;
        FREE(*list);
    }
}

void List_map(T list, void apply(void **x, void *cl), void *cl) {
    assert(apply);
    for ( ; list; list = list->rest)
        apply(&list->first, cl);
}

void **List_toArray(T list, void *end)
{
    int i, n = List_length(list);

    void **array = ALLOC((n + 1) * sizeof(*array)); // clients are responsible to invoke FREE().

    for (i = 0; i < n; i++) {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;

    return array;
}
