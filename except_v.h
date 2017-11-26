//
// Created by chenzheng on 05/03/2017.
//

#ifndef TEST_EXCEPT_H
#define TEST_EXCEPT_H

#include<setjmp.h>

#define T Except_T

typedef struct T {
    const char *reason;
} T;

enum { Except_entered = 0, Except_raised, Except_handled, Except_finalized };

void Except_raise(const T *e, const char *file, int line);

#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)

typedef struct Except_Frame Except_Frame;
struct Except_Frame {
    Except_Frame *prev;
    jmp_buf env;
    const char *file;
    int line;
    const T *exception;
};

extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;

#define RERAISE Except_raise(Except_frame.exception, Except_frame.file, Except_frame.line)

#define RETURN swith (, 0) default: return

// TRY statement
#define TRY do { \
    volatile int Except_flag; \
    Except_Frame except_frame; \
    Except_frame.prev = Except_stack; \
    Except_stack = &Except_frame; \
    Except_flag = setjmp(Except_frame.env); \
    if (Except_flag == Except_entered) {

// EXCEPT statement
#define EXCEPT(e) \
    if (Except_flag == Except_entered) Except_stack = Except_stack->prev \
    } else if (Except_frame.exception == &(e)) { \
        Except_flag = Except_handled;

// ELSE statement
#define ELSE \
    if (Except_flag == Except_entered) Except_stack = Except_stack->prev \
    } else { \
        Excetp_flag = Except_handled

// FINALLY statement
#define FINALLY \
    if (Except_flag == Excetp_entered) Except_stack = Except_stack->prev \
    } { \
        if (Except_flag == Except_entered) \
            Except_flag = Except_finalized;

// END_TRY statement
#define END_TRY \
    if (Except_flag == Except_entered) Except_stack = Except_stack->prev \
    } if (Except_flag == Except_raised) RERAISE; \
    } while (0)

#undef T
#endif //TEST_EXCEPT_H
