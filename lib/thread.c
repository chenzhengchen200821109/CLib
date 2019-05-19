//
// Created by chenzheng on 06/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
#include <execinfo.h>
#include "assert_v.h"
#include "except_v.h"
#include "mem.h"
#include "thread.h"
#include "error.h"
#include "sem.h"

#define isempty(q) ((q) == NULL)

typedef int (*tcontext_handler_t)(void* s1, void* s2);

enum
{
    eip = 0,
    ebx,
    ecx,
    edx,
    edi,
    esi,
    ebp,
    esp
};

struct TContext
{
    int buffer[8];
    int ss_size;
    char* ss_sp;
};

typedef struct parameter
{
    const void* thread;
    const void* arg;
} parameter_t;

/* thread control block */
struct Thread {
    //unsigned long *sp; /* must be first */
    void* stack_esp;
    void* stack_ebp;
    int (*startup)(void *);
    void* arg;
    struct TContext context;
    struct Thread* link;
    struct Thread** inqueue; /* pointer to the queue variable - ready */
    struct Thread* handle; /* t identifies an existing thread only if t is equal to t->handle */   
    //Except_Frame* estack;
    int code;
    struct Thread* join;
    struct Thread* next;
    int alerted;
};

static struct Thread* current; /* the thread that currently holds the processor */
static int nthreads; /* the number of of existing threads */
static struct Thread root; /* main thread */
static struct Thread* join0; // join0队列只包含一个线程, 来等待所有其他线程结束
static struct Thread* freelist;
static int critical;

/*
 * The vacuous function _MONITOR and the external function _ENDMONITOR
 * are used only for their address. These addresses encompass critical
 * sections - thread code that must not be interrupted.
 */
void _MONITOR(void) {}
extern void _ENDMONITOR(void);

/* ready queue, which holds running threads that do not have processors */
static struct Thread* ready = NULL;
/*
 * _swtch is like setjmp and longjmp. When thread A calls _swtch, control
 * transfers to thread B. When B calls _swtch to resume A, A's call to 
 * _swtch returns.
 */
extern void _swtch(struct TContext* from, struct TContext* to);

void TContext_init(struct TContext* context)
{
    memset(context, 0, sizeof(*context));
}

int TContext_make(struct TContext* context, tcontext_handler_t pfn, const void* thread, const void* arg)
{
    char* sp = context->ss_sp + context->ss_size - sizeof(parameter_t);
    sp = (char *)((unsigned int)sp & ~16L);

    parameter_t* param = (parameter_t *)sp;
    param->thread = thread;
    param->arg = arg;

    memset(context->buffer, 0, sizeof(context->buffer));
    context->buffer[esp] = (int)((char *)sp - sizeof(void *));
    context->buffer[eip] = (int)(char *)pfn;

    return 0;

}

//const Except_T Thread_Alerted = { "Thread alerted" };

/*
 * put的参数包括队列变量的地址, 因而可以修改该变量。
 * put(t, &ready) appends t to the ready queue.
 */
static void put(struct Thread* t, struct Thread** queue)
{
    assert(t);
    assert(t->inqueue == NULL && t->link == NULL);
    if (*queue) { // queue is not empty
        t->link = (*queue)->link;
        (*queue)->link = t;
    } else // queue is empty
        t->link = t;
    *queue = t;
    t->inqueue = queue;
}

/*
 * get从一个给定队列中移除第一个元素
 * get() removes the first element from a given queue.
 */
static struct Thread* get(struct Thread** queue)
{
    struct Thread* t;

    assert(!isempty(*queue));
    t = (*queue)->link;
    if (t == *queue) // only one thread
        *queue = NULL;
    else
        (*queue)->link = t->link;
    assert(t->inqueue == queue);
    t->link = NULL;
    t->inqueue = NULL;
    return t;
}

static void delete(struct Thread* t, struct Thread** queue)
{
    struct Thread* p;

    assert(t->link && t->inqueue == queue);
    assert(!isempty(*queue));
    for (p = *queue; p->link != t; p = p->link)
        ;
    if (p == t)
        *queue = NULL;
    else {
        p->link = t->link;
        if (*queue == t)
            *queue = p;
    }
    t->link = NULL;
    t->inqueue = NULL;
}

static void run(void)
{
    struct Thread* t = current;

    current = get(&ready);
    //t->estack = Except_stack;
    //Except_stack = current->estack;
    _swtch(&(t->context), &(current->context));
}

static void testalert(void)
{
    if (current->alerted) {
        current->alerted = 0;
        err_sys("Thread Alerted", __FILE__, __LINE__);
    }
}

/*
 * release all threads.
 */
static void release(void)
{
    struct Thread* t;
    do {
        critical++;
        while ((t = freelist) != NULL) {
            freelist = t->next;
            FREE(t->stack_ebp);
            FREE(t);
        }
        critical--;
    } while(0);
}

static int interrupt(int sig, struct sigcontext ctx)
{
    if (critical || (ctx.eip >= (unsigned int)_MONITOR &&
            ctx.eip <= (unsigned int)_ENDMONITOR))
        return 0;
    put(current, &ready);
    //sigsetmask(ctx.sig);
    run();
    return 0;
}

/* 
 * Thread_init() must be called only once.
 */
int Thread_init(int preempt)
{
    stack_t ss;
    assert(preempt == 0 || preempt == 1);
    assert(current == NULL);
    root.handle = &root;
    current = &root;
    nthreads = 1;
    if (preempt) {
        // initialize preemptive scheduling
        {
            ss.ss_sp = malloc(SIGSTKSZ);
            if (ss.ss_sp == NULL)
                return -1;
            ss.ss_size = SIGSTKSZ;
            ss.ss_flags = 0;
            if (sigaltstack(&ss, NULL) == -1)
                return -1;
            struct sigaction sa;
            memset(&sa, '\0', sizeof(sa));
            sa.sa_handler = (void (*)(int))interrupt;
            sa.sa_flags = SA_ONSTACK;
            /*
             * ITIMER_VIRTUAL - This timer counts down against
             * the user-mode CPU time consumed by the process.
             * (The measurement includes CPU time consumed by all
             * threads in the process.) At each expiration, a 
             * SIGVTALRM signal is generated.
             */
            if (sigaction(SIGALRM, &sa, NULL) < 0)
                return -1;
        }
        {
            struct itimerval it;
            it.it_value.tv_sec = 0;
            it.it_value.tv_usec = 50;
            it.it_interval.tv_sec = 0;
            it.it_interval.tv_usec = 50;
            if (setitimer(ITIMER_REAL, &it, NULL) < 0)
                return -1;
        }
    }
    return preempt;
}

struct Thread* Thread_self(void)
{
    assert(current);
    return current;
}


/*
 * All of the Thread functions that can cause a context
 * switch call run, and they put the current thread on
 * ready or another appropriate queue before calling run.
 */
void Thread_pause(void)
{
    assert(current);
    put(current, &ready);
    run();
}

/*
 * Thread_join() waits for thread t to terminate and returns
 * t's exit code.
 */
int Thread_join(struct Thread* t)
{
    assert(current && t != current);
    //testalert();
    if (t) {
        // wait for thread to terminate
        if (t->handle == t) {
            put(current, &t->join);
            run();
            //testalert();
            return current->code;
        } else
            return -1;
    } else {
        // wait for all threads to terminate if t is NULL
        assert(isempty(join0));
        if (nthreads > 1) {
            put(current, &join0);
            run();
            //testalert();
        }
        return 0;
    }
}

/*
 * Thread_exit() has numerous jobs to do: It must deallocate the 
 * resources associated with the calling thread, resume the threads
 * waiting for the calling thread to terminated an arrange for them
 * to get the exit code, and check whether the calling thread is the
 * second to last or last thread in the system.
 */
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
        struct Thread* t = get(&current->join);
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
void Thread_alert(struct Thread* t)
{
    assert(current);
    assert(t && t->handle == t);
    t->alerted = 1;
    if (t->inqueue) {
        delete(t, t->inqueue);
        put(t, &ready);
    }
}

static int Thread_startup(struct Thread* t, void* arg)
{
    int result;
    if (t->startup)
        result = t->startup(t->arg);
    Thread_exit(result);
    return 0;
}

/*
 * Thread_new() has four tasks: allocate the resources for a
 * new thread, initialize the new thread's state so that it can
 * be started by a return from _swtch, increment nthreads, and 
 * append the new thread to ready.
 */
struct Thread* Thread_new(int apply(void*), void* args)
{
    struct Thread* t;
    int stacksize;
    void* stack_top;

    assert(current);
    assert(apply);
    // allocate resources for a new thread
    {
        stacksize = (16 * 1024);
        release();
        do {
            critical++;
            //TRY
            t = ALLOC(sizeof(*t));
            memset(t, '\0', sizeof(*t));
            stack_top = ALLOC(stacksize);
            //EXCEPT(Mem_Failed);
            //t = NULL;
            // END_TRY
            critical--;
        } while (0);
        if (t == NULL || stack_top == NULL)
            err_exit(errno, "ALLOC error", __FILE__, __LINE__);
            //RAISE(Thread_Failed);
        /*
         * initialize t's stack pointer so that it's aligned on a 16-byte
         * boundary, which accommodates most platforms.
         */
    }
    t->stack_esp = (char *)stack_top + stacksize;
    t->stack_ebp = (char *)stack_top;
    t->startup = apply;
    t->arg = args;
    nthreads++;

    TContext_init(&(t->context));
    t->context.ss_size = stacksize;
    t->context.ss_sp = t->stack_ebp;
    TContext_make(&(t->context), (tcontext_handler_t)Thread_startup, t, args);
    put(t, &ready);

    return t;
}

