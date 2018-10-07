//
// Created by chenzheng on 06/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "assert_v.h"
#include "mem.h"
#include "thread.h"
//#include "sem.h"

static T current;
static int nthreads;
static struct Thread_T root;
static T join0;

void _MONITOR(void) {}
extern void _ENDMONITOR(void);

#define T Thread_T
#define isempty(q) ((q) == NULL)

struct T {
    unsigned long *sp;
    T link;
    T *inqueue;
    T handle;
    Except_Frame* estack;
    int code;
    T join;
};

static T ready = NULL;
/* prototypes */
extern void _swtch(T from, T to);

/*
 * put的参数包括队列变量的地址, 因而可以修改该变量。
 */
static void put(T t, T* q)
{
    assert(t);
    assert(t->inqueue == NULL && t->link == NULL);
    if (*q) { // queue is not empty
        t->link = (*q)->link;
        (*q)->link = t;
    } else // queue is empty
        t->link = t;
    *q = t;
    t->inqueue = q;
}

/*
 * get从一个给定队列中移除第一个元素
 */
static T get(T* q)
{
    T t;

    assert(!isempty(*q));
    t = (*q)->link;
    if (t == *q) // only one thread
        *q = NULL;
    else
        (*q)->link = t->link;
    assert(t->inqueue == q);
    t->link = NULL;
    t->inqueue = NULL;
    return t;
}

static void delete(T t, T* q)
{
    T p;

    assert(t->link && t->inqueue == q);
    assert(!isempty(*q));
    for (p = *q; p->link != t; p = p->link)
        ;
    if (p == t)
        *q = NULL;
    else {
        p->link = t->link;
        if (*q == t)
            *q = p;
    }
    t->link = NULL;
    t->inqueue = NULL;
}

int Thread_init(int preempt, ...)
{
    assert(preempt == 0 || preempt == 1);
    assert(current == NULL);
    root.handle = &root;
    current = &root;
    nthreads = 1;
    if (preempt) {

    }
    return 1;
}

T Thread_self(void)
{
    assert(current);
    return current;
}

static void run(void)
{
    T t = current;

    current = get(&ready);
    t->estack = Except_stack;
    Except_stack = current->estack;
    _swtch(t, current);
}

void Thread_pause(void)
{
    assert(current);
    put(current, &ready);
    run();
}

int Thread_join(T t)
{
    assert(current && t != current);
    testalert();
    if (t) {
        // wait for thread to terminate
        if (t->handle == t) {
            put(current, &t->join);
            run();
            testalert();
            return current->code;
        } else
            return -1;
    } else {
        // wait for all threads to terminate
        assert(isempty(join0));
        if (nthreads > 1) {
            put(current, &join0);
            run();
            testalert();
        }
        return 0;
    }
}