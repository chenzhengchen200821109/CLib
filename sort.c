//
// Created by chenzheng on 06/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "assert_v.h"
#include "fmt.h"
#include "thread.h"
#include "mem.h"

struct args
{
    int *a;
    int lb, ub;
};

int cutoff = 10000;

int partition(int a[], int i, int j)
{
    int v, k, t;

    j++;
    k = i;
    v = a[k];
    while (i < j) {
        i++; while (a[i] < v && i < j) i++;
        j--; while (a[j] > v) j--;
        if (i < j) { t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    t = a[k]; a[k] = a[j]; a[j] = t;
    return j;
}

int quick(void *cl)
{
    struct args *p = cl;
    int lb = p->lb, ub = p->ub;

    if (lb < ub) {
        int k = partition(p->a, lb, ub);
        p->lb = lb;
        p->ub = k - 1;
        if (k - lb > cutoff) {
            Thread_T t;
            t = Thread_new(quick, p, sizeof(*p), NULL);
            Fmt_print("thread %p sorted %d...%d\n", t, lb, k - 1);
        } else
            quick(p);
    }
}

void sort(int *x, int n, int argc, char *argv[])
{
    struct args args;

    if (argc >= 3)
        cutoff = atoi(argv[2]);
    args.a = x;
    args.lb = 0;
    args.ub = n - 1;
    quick(&args);
    Thread_join(NULL);
}

int main(int argc, char *argv)
{
    int i, n = 100000, *x, preempt;

    preempt = Thread_init(1, NULL);
    assert(preempt == 1);
    if (argc >= 2)
        n = atoi(argv[1]);
    x = CALLOC(n, sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++)
        x[i] = rand();
    sort(x, n, argc, argv);
    for (i = 1; i < n; i++)
        if (x[i] < x[i-1])
            break;
    assert(i == n);
    Thread_exit(EXIT_SUCCESS);
    return (EXIT_SUCCESS);
}