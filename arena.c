//
// Created by chenzheng on 19/03/2017.
//

#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include "assert_v.h"
#include "except_v.h"

#define T Arena_T

const Except_T Arena_NewFailed = { "Arena Creation Failed" };
const Except_T Arena_Failed = { "Arena Allocation Failed" };

/*
 * Arena structure
 */

struct T {
    T prev;
    char *avail;
    char *limit;
};

union align {
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

union header {
    struct T b;
    union align a;
};

static T freechunks;
static int nfree;

/*
 * Arena_new allocate memory for an instance of Arena_T struct and return it.
 */
T Arena_new(void) {
    T arena = malloc(sizeof(*arena));
    if (arena == NULL)
        RAISE(Arena_NewFailed);
    arena->prev = NULL;
    arena->avail = arena->limit = NULL;
    return arena;
}

void Arena_dispose(T *ap) {
    assert(ap && *ap);
    Arena_free(*ap);
    free(*ap);
    *ap = NULL;
}

void *Arena_alloc(T arena, long nbytes, const char *file, int line) {
    assert(arena);
    assert(nbytes > 0);
    nbytes = ((nbytes + sizeof(union align) - 1)/
            (sizeof(union align))) * (sizeof(union align));
    while (nbytes > arena->limit - arena->avail) {
        T ptr;
        char *limit;
        if ((ptr = freechunks) != NULL) {
            freechunks = freechunks->prev;
            nfree--;
            limit = ptr->limit;
        } else {
            long m = sizeof(union header) + nbytes + 10 * 1024;
            ptr = malloc(m);
            if (ptr == NULL) {
                if (file == NULL)
                    RAISE(Arena_Failed);
                else
                    Except_raise(&Arena_Failed, file, line);
            }
            limit = (char *)ptr + m;
        }
        *ptr = *arena;
        arena->avail = (char *)((union header *)ptr + 1);
    }
    arena->avail += nbytes;
    return arena->avail - nbytes;
}
