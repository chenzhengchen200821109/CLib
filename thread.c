//
// Created by chenzheng on 06/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include "assert_v.h"
#include "except_v.h"
#include "mem.h"
#include "thread.h"
//#include "sem.h"

static T current;
static int nthreads;
static struct Thread_T root;
static T join0; // join0队列只包含一个线程, 来等待所有其他线程结束
static T freelist;
static int critical;

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
    T next;
    int alerted;
};

static T ready = NULL;
/* prototypes */
extern void _swtch(T from, T to);

const Except_T Thread_Alerted = { "Thread alerted" };

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

static void testalert(void)
{
    if (current->alerted) {
        current->alerted = 0;
        RAISE(Thread_Alerted);
    }
}

static void release(void)
{
    T t;
    do {
        critical++;
        while ((t = freelist) != NULL) {
            freelist = t->next;
            FREE(t);
        }
        critical--;
    } while(0);
}

static int interrupt(int sig, int code, struct sigcontext *scp)
{
    if (critical || scp->sc_pc >= (unsigned long)_MONITOR && scp->sc_pc <= (unsigned long)_ENDMONITOR)
        return 0;
    put(current, &ready);
    sigsetmask(scp->sc_mask);
    run();
    return 0;
}

int Thread_init(int preempt, ...)
{
    assert(preempt == 0 || preempt == 1);
    assert(current == NULL);
    root.handle = &root;
    current = &root;
    nthreads = 1;
    if (preempt) {
        // initialize preemptive scheduling
        {
            struct sigaction sa;
            memset(&sa, '\0', sizeof(sa));
            sa.sa_handler = (void (*)())interrupt;
            if (sigaction(SIGVTALRM, &sa, NULL) < 0)
                return 0;
        }
        {
            struct itimerval it;
            it.it_value.tv_sec = 0;
            it.it_value.tv_usec = 50;
            it.it_interval.tv_sec = 0;
            it.it_interval.tv_usec = 50;
            if (setitimer(ITIMER_VIRTUAL, &it, NULL) < 0)
                return 0;
        }
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

void Thread_exit(int code)
{
    assert(current);
    release();
    if (current != &root) {
        current->next = freelist;
        freelist = current;
    }
    current->handle = NULL;
    // resume threads waiting for current's termination
    while (!isempty(current->join)) {
        T t = get(&current->join);
        t->code = code;
        put(t, &ready);
    }
    if (!isempty(join0) && nthreads == 2) {
        assert(isempty(ready));
        put(get(&join0), &ready);
    }
    // run another thread or exit
    if (--nthreads == 0)
        exit(code);
    else
        run();
}

/*
 * Thread_alert将一线程线程标记"警报-待决"状态
 */
void Thread_alert(T t)
{
    assert(current);
    assert(t && t->handle == t);
    t->alerted = 1;
    if (t->inqueue) {
        delete(t, t->inqueue);
        put(t, &ready);
    }
}

T Thread_new(int apply(void*), void* args, int nbytes, ...)
{
    T t;

    assert(current);
    assert(apply);
    assert(args && nbytes >= 0 || args == NULL);
    if (args == NULL)
        nbytes = 0;
    // allocate resources for a new thread
    {
        int stacksize = (16 * 1024 + sizeof(*t) + nbytes + 15) & ~15;
        release();
        do {
            critical++;
            //TRY
            t = ALLOC(stacksize);
            memset(t, '\0', sizeof(*t));
            //EXCEPT(Mem_Failed);
            t = NULL;
            // END_TRY
            critical--;
        } while (0);
        if (t == NULL)
            RAISE(Thread_Failed);
        // initialize t's stack pointer
        t->sp = (void *)((char *)t + stacksize);
        while (((unsigned long)t->sp) & 15)
            t->sp--;
    }
    t->handle = t;
    if (nbytes > 0) {
        t->sp -= ((nbytes + 15U) & ~15) / sizeof(*t->sp);
        do {
            critical++;
            memcpy(t->sp, args, nbytes);
            critical--;
        } while (0);
        args = t->sp;
    }
}

