//
// Created by chenzheng on 21/01/2017.
//

#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include "assert_v.h"
#include "except_v.h"

// define the number of descriptors used in memory management
#define NDESCRIPTOR 512

// Define a descriptor with some fields
static struct descriptor {
    struct descriptor *free;
    struct descriptor *link;
    const void *ptr;
    long size;
    const char *file;
    int line;
} *htab[2048];

// alignment
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

/* handle memory failure */
const Except_T Mem_Failed = { "Allocation Failed" };

// Define a macro for hash function
#define hash(p, t) (((unsigned long)(p) >> 3) & (sizeof(t)/sizeof((t)[0])-1))

#define NALLOC ((4096 + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align))

static struct descriptor freelist = { &freelist }; /* make a ring linked list */

// given an address
static struct descriptor *find(const void *ptr) {
    struct descriptor *bp = htab[hash(ptr, htab)];

    // search
    while (bp && bp->ptr != ptr)
        bp = bp->link;
    return bp;
}

/*
 * allocate NDESCRIPTOR descriptors first
 */
static struct descriptor *dalloc(void *ptr, long size, const char *file, int line) {
    static struct descriptor *avail;
    static int nleft;

    if (nleft <= 0) {
        avail = malloc(NDESCRIPTOR * sizeof(*avail));
        if (avail == NULL)
            return NULL;
    }
    avail->ptr = ptr;
    avail->size = size;
    avail->file = file;
    avail->line = line;
    avail->free = avail->link = NULL;
    nleft--;
    return avail++;
}

void *Mem_alloc(long nbytes, const char *file, int line) {
    struct descriptor *bp;
    void *ptr;

    assert(nbytes > 0);
    // round up to an alignment boundary
    nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align));

    for (bp = freelist.free; bp; bp = bp->free) {
        if (bp->size > nbytes) {
            bp->size -= nbytes;
            ptr = (char *)bp->ptr + bp->size;
            if ((bp = dalloc(ptr, nbytes, file, line)) != NULL) {
                unsigned h = hash(ptr, htab);
                bp->link = htab[h];
                htab[h] = bp;
                return ptr;
            } else {
                if (file == NULL)
                    RAISE(Mem_Failed);
                else
                    Except_raise(&Mem_Failed, file, line);
            }
        }
        if (bp == &freelist) {
            struct descriptor *newptr;
            if ((ptr = malloc(nbytes + NALLOC)) == NULL || (newptr = dalloc(ptr, nbytes + NALLOC, __FILE__, __LINE__)) == NULL)
            {
                if (file == NULL)
                    RAISE(Mem_Failed);
                else
                    Except_raise(&Mem_Failed, file, line);
            }
            newptr->free = freelist.free;
            freelist.free = newptr;
        }
    }
    assert(0);
    return NULL;
}

void Mem_free(void *ptr, const char *file, int line) {
    // if ptr is a null pointer, nothing happens. otherwise free it
    if (ptr) {
        struct descriptor *bp;
        // check whether ptr is a effective pointer.
        if (((unsigned long)ptr)%(sizeof(union align)) != 0 || (bp = find(ptr)) == NULL || bp->free)
            Except_raise(&Assert_Failed, file, line);
        bp->free = freelist.free;
        freelist.free = bp;
        // not free ptr actually, just set its free field to indicate
        // the pointer has been deallocated and can't be used by clients.
        // free memory allocation.
        //free(ptr);
    }
}

void *Mem_calloc(long count, long nbytes, const char *file, int line) {
    void *ptr;

    assert(count > 0);
    assert(nbytes > 0);
    ptr = Mem_alloc(count * nbytes, file, line);
    memset(ptr, '\0', count * nbytes);
    return ptr;

}

void *Mem_resize(void *ptr, long nbytes, const char *file, int line) {
    struct descriptor *bp;
    void *newptr;

    assert(ptr);
    assert(nbytes > 0);
    if (((unsigned long)ptr)%(sizeof(union align)) != 0 || (bp = find(ptr)) == NULL || bp->free)
        Except_raise(&Assert_Failed, file, line);
    newptr = Mem_alloc(nbytes, file, line);
    memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
    Mem_free(ptr, file, line);
    return newptr;
}

