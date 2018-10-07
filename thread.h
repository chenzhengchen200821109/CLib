//
// Created by chenzheng on 06/12/2017.
//

#ifndef CLIB_THREAD_H
#define CLIB_THREAD_H

#endif //CLIB_THREAD_H

#include "except_v.h"
#define T Thread_T
typedef struct T *T;

extern const Except_T Thread_Failed;
extern const Except_T Thread_Alerted;

extern int Thread_init(int preempt, ...);
extern T Thread_new(int apply(void *), void *args, int nbytes, ...);
/*
 * 线程调用Thread_exit(code)函数后, 将结束该线程的执行。此后, (借助于Thread_join)
 * 等待该线程结束的线程将恢复执行, code的值将作为调用Thread_join的结果返回给这些恢复
 * 执行的线程。在最后一个线程调用Thread_exit时, 整个程序通过调用exit(code)结束。
 */
extern void Thread_exit(int code);
extern void Thread_alert(T t);
extern T Thread_self(void);
extern int Thread_join(T t);
extern void Thread_pause(void);

#undef
#endif