//
// Created by chenzheng on 14/11/2017.
//

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "assert_v.h"
#include "ring.h"
#include "mem.h"

#define T Ring_T

struct T
{
    struct node {
        struct node *llink, *rlink;
        void *value;
    } *head;
    int length;
};

T Ring_new(void)
{
    T ring;
    NEW0(ring);
    ring->head = NULL;
    return ring;
}

T Ring_ring(void *x, ...)
{
    va_list ap;
    T ring = Ring_new();
    va_start(ap, x);
    for ( ; x; x = va_arg(ap, void *))
        Ring_addhi(ring, x);
    va_end(ap);
    return ring;
}

void Ring_free_v1(T *ring)
{
    struct node *p, *q;
    assert(ring && *ring);
    if ((p = (*ring)->head) != NULL) {
        int n = (*ring)->length;
        for ( ; n-- > 0; p = q) {
            q = p->rlink;
            FREE(p);
        }
    }
    FREE(*ring);
}

void Ring_free_v2(T *ring)
{
    struct node *prev, *p, *q;
    assert(ring && *ring);
    if ((p = (*ring)->head) != NULL) {
        prev = p->llink;
        prev->rlink = NULL;
        do {
            q = p->rlink;
            FREE(p);
            p = q;
        } while (p != NULL);
    }
    FREE(*ring);
}

int Ring_length(T ring)
{
    assert(ring);
    return ring->length;
}

void *Ring_get(T ring, int i)
{
    struct node *q;

    assert(ring);
    assert(i >= 0 && i < ring->length);
    {
        int n;
        q = ring->head;
        if (i <= ring->length / 2)
            for (n = i; n-- > 0; )
                q = q->rlink;
        else
            for (n = ring->length - 1; n-- > 0; )
                q = q->llink;
    }
    return q->value;
}

void *Ring_put(T ring, int i, void *x)
{
    struct node *q;
    void *prev;

    assert(ring);
    assert(i >= 0 && i < ring->length);
    {
        int n;
        q = ring->head;
        if (i <= ring->length / 2)
            for (n = i; n-- > 0; )
                q = q->rlink;
        else
            for (n = ring->length - 1; n-- > 0; )
                q = q->llink;
    }
    prev = q->value;
    q->value = x;
    return prev;
}