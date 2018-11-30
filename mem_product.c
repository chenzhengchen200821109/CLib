//
// Created by chenzheng on 21/07/2017.
//

/*
 * This product implementation does not include error checking.
 */
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
//#include "assert_v.h"
//#include "except_v.h"
#include "mem.h"
#include "error.h"

//const Except_T Mem_Failed = { "Allocation Failed" };

struct descriptor
{
    struct descriptor* allocated;
    int freed;
    const void* ptr;
    long size;
    /* for debug */
    const char* file;
    int line;
};

static struct descriptor allocatedlist;

static struct descriptor* find(const void* ptr)
{
    struct descriptor* bp = &allocatedlist;
    while (bp != NULL) {
        if (bp->ptr == ptr)
            break;
        bp = bp->allocated;
    } 
    return bp;
}

static struct descriptor* dalloc(void* ptr, long size, const char* file, int line)
{
    struct descriptor* pdescriptor;
    if ((pdescriptor = malloc(sizeof(*pdescriptor)))  == NULL)
        return NULL;
    pdescriptor->ptr = ptr;
    pdescriptor->size = size;
    pdescriptor->file =file;
    pdescriptor->line = line;
    pdescriptor->freed = 0;
    pdescriptor->allocated = NULL;

    return pdescriptor;
}

void *Mem_alloc(long nbytes, const char *file, int line)
{
    void *ptr;
    struct descriptor* pd;

    assert(nbytes > 0);
    assert(file != NULL);
    assert(line > 0);
    if ((ptr = malloc(nbytes)) == NULL || (pd = dalloc(ptr, nbytes, __FILE__, __LINE__)) == NULL)
        err_exit(errno, "malloc error", file, line);
    if (allocatedlist.allocated == NULL)
        allocatedlist.allocated = pd;
    else {
        pd->allocated = allocatedlist.allocated;
        allocatedlist.allocated = pd;
    }
    return ptr;
}

void *Mem_calloc(long count, long nbytes, const char *file, int line)
{
    void *ptr;

    assert(count > 0);
    assert(nbytes > 0);
    return (ptr = Mem_alloc(count * nbytes, file, line));
}

void Mem_free(void *ptr, const char *file, int line)
{
    struct descriptor* pd;
    if (ptr) {
        if ((pd = find(ptr)) && pd->freed == 0) {
            pd->freed = 1;
            free(ptr);
        } else if (pd == NULL)
            err_cont(0, "invalid pointer", file, line);
        else 
            err_cont(0, "already freed", file, line);
    }
}

void *Mem_resize(void *ptr, long nbytes, const char *file, int line)
{
    struct descriptor* pd;

    assert(ptr);
    assert(nbytes > 0);
    if (ptr) {
        if ((pd = find(ptr)) && pd->freed == 0) {
            if ((ptr = realloc(ptr, nbytes)) == NULL)
                err_exit(errno, "realloc error", file, line);
        }
    }
    return ptr;
}


