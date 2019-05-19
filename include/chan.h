//
// Created by chenzheng on 07/10/2018.
//

#ifndef _CHAN_H_
#define _CHAN_H_

#define T Chan_T
typedef struct T *T;

extern T Chan_new(void);
/*
 * 调用线程会阻塞,直至另一个线程对同一通道调用Chan_receive,当这样的
 * 两个线程"会合"时,数据从发送方复制到接收方,两个调用分别返回。Chan_send
 * 返回接受方接受的字节。
 */
extern int Chan_send(T c, const void* ptr, int size);
/*
 *
 */
extern int Chan_receive(T c, void* ptr, int size);

#undef T
#endif // _CHAN_H_
