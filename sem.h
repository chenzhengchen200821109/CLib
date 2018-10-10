//
// Created by chenzheng on 07/12/2017.
//

#ifndef _SEM_H_
#define _SEM_H_

/*
 * 信号量支持的两个操作是wait和signal。signal(s)在逻辑上相当于将s原子化地加1。
 * wait(s)等待s变为正数, 然后将其原子化地减1。
 */
#define T Sem_T
typedef struct T {
    int count;
    void *queue;
} T;

/*
 * common usage：LOCK(mutex)
 *                  statements
 *               END_LOCK
 * 但是如果statements引发异常, 将导致mutex得不到释放。
 */
#define LOCK(mutex) do { Sem_T *_yymutex = &(mutex); \
    Sem_wait(__yymutex);
#define END_LOCK Sem_signal(__yymutex); } while (0);

// 在同一信号量上调用Sem_init多次, 属于未检查的运行时错误。
extern void Sem_init(T *s, int count);
extern T *Sem_new(int count);
extern void Sem_wait(T *s);
extern void Sem_signal(T *s);

#undef
#endif // _SEM_H_
