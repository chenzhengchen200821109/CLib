#include "thread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <signal.h>

int thread_func_a(void* arg)
{
    int sum = 0;
    int i;
    struct timeval tv;

    while (1) {
        tv.tv_sec = 5;
        tv.tv_usec = 10;
        sum += i;
        printf("thread_func_a\n");
        //Thread_pause();
        select(0, NULL, NULL, NULL, &tv);
    }
    return 0;
}

int thread_func_b(void* arg)
{
    int sum = 0;
    int i;
    struct timespec req;

    req.tv_sec = 5;
    req.tv_nsec = 10000;

    while (1) {
        sum += i;
        printf("thread_func_b\n");
        //Thread_pause();
        //nanosleep(&req, NULL);
        raise(SIGALRM);
    }
}

int thread_func(void* arg)
{
    int sum = 0;
    int i = 0;
    int id = (int)arg;

    for (i = 0; i < 1000; i++) {
        sum += i;
        printf("thread_func[%d], [%d]\n", id, sum);
        //Thread_pause();
    }

    return 0;
}

int main()
{
    int preempt;
    //struct Thread* t1, *t2;

    preempt = Thread_init(1);
    assert(preempt == 1);

    //t1 = Thread_new(thread_func_a, NULL);
    //t2 = Thread_new(thread_func_b, NULL);
    //Thread_exit(0);
    //Thread_pause();

    int i;

    for (i = 0; i < 10; i++) {
        Thread_new(thread_func, (void *)i);
    }
    Thread_exit(0);

    return 0;
}
