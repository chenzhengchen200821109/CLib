//
// Created by chenzheng on 24/06/2017.
//

#ifndef CLIB_SEQ_H
#define CLIB_SEQ_H

#define T Seq_T
typedef struct T *T;

extern T Seq_new(int hint);
extern T Seq_seq(void *x, ...);
extern void Seq_free_v1(T *seq);
extern void Seq_free_v2(T *seq);
extern int Seq_length(T seq);
extern void *Seq_get(T seq, int i);
extern void *Seq_put(T seq, int i, void *x);
extern void *Seq_addlo(T seq, void *x);
extern void *Seq_addhi(T seq, void *x);
extern void *Seq_remlo(T seq);
extern void *Seq_remhi(T seq);

#undef T
#endif //CLIB_SEQ_H
