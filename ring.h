//
// Created by chenzheng on 14/11/2017.
//

#ifndef CLIB_RING_H
#define CLIB_RING_H

#define T Ring_T
typedef struct T *T;

extern T Ring_New(void);
extern T Ring_ring(void *x, ...);
extern void Ring_free(T *ring);
extern int Ring_length(T ring);
extern void *Ring_get(T ring, int i);
extern void *Ring_put(T ring, int i, void *x);
extern void *Ring_add(T ring, int pos, void *x);
extern void *Ring_addlo(T ring, void *x);
extern void *Ring_addhi(T ring, void *x);
extern void *Ring_remove(T ring, int i);
extern void *Ring_remlo(T ring);
extern void *Ring_remhi(T ring);
extern void Ring_rotate(T ring, int n);

#undef
#endif //CLIB_RING_H
