//
// Created by chenzheng on 07/12/2017.
//

#ifndef CLIB_SEM_H
#define CLIB_SEM_H

#define T Sem_T
typedef struct T {
    int count;
    void *queue;
} T;

#define LOCK(mutex) do { Sem_T *_yymutex = &(mutex); \
    Sem_wait(__yymutex);
#define END_LOCK Sem_signal(__yymutex); } while (0);

extern void Sem_init(T *s, int count);
extern T *Sem_new(int count);
extern void Sem_wait(T *s);
extern void Sem_signal(T *s);

#undef
#endif //CLIB_SEM_H
