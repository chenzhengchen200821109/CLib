//
// Created by chenzheng on 21/01/2017.
// These interfaces show us that how to improve some current basic memory management package
// with some checking technique. Some memory operations include allocation, reallocation and
// destroy allocated memory.
//

#ifndef TEST_MEM_H
#define TEST_MEM_H

#include "assert_v.h"

// Define a memory failed assertion
extern const Except_T Mem_Failed; /* struct Except_T { const char *reason; } */

// Wrapper function of alloc and calloc function from original C library, but include
// some error checking. like null pointer, count and nbytes must be positive integers.
extern void *Mem_alloc(long nbytes, const char *file, int line);
extern void *Mem_calloc(long count, long nbytes, const char *file, int line);
// Define some macros for convenience.
#define ALLOC(nbytes) Mem_alloc((nbytes), __FILE__, __LINE__) /* macro for Mem_alloc function */
#define CALLOC(count, nbytes) Mem_calloc((count), (nbytes), __FILE__, __LINE__) /* macro for Mem_calloc function */
#define NEW(p) ((p) = ALLOC((long)sizeof(*(p)))) /* macro wrapper for macro ALLOC */
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof(*(p)))) /* macro wrapper for macro CALLOC */

extern void Mem_free(void *ptr, const char *file, int line);
// The following definition used a comma expression to make a pointer null pointer once the pointed pointed
// to a memory area has been free. In this case, deference this pointer will cause segment fault to warn us that
// the pointer has been freed.
#define FREE(ptr) ((void)(Mem_free((ptr), __FILE__, __LINE__), (ptr) = 0))

extern void *Mem_resize(void *ptr, long nbytes, const char *file, int line);
#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), (nbytes), __FILE__, __LINE__))

#endif //TEST_MEM_H
