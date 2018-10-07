//
// Created by chenzheng on 07/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "assert_v.h"
#include "fmt.h"
#include "thread.h"
#include "sem.h"

#define NBUMP 30000

struct args
{
    Sem_T *mutex;
    int *ip;
};

int unsafe(void *cl)
{
    int i, *ip = cl;

    for (i = 0; i < NBUMP; i++)
        *ip = *ip + 1;
    return EXIT_SUCCESS;
}

int safe(void *cl)
{
    struct args *p = cl;
    int i;

    for (i = 0; i < NBUMP; i++) {
        LOCK(*p->mutex);
            *p->ip = *p->ip + 1;
        END_LOCK;
    }
    return EXIT_SUCCESS;
}