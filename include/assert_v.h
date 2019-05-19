//
// Created by chenzheng on 06/03/2017.
//

#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "except_v.h"
extern void assert(int e);
#define assert(e) ((void)((e) || (RAISE(Assert_Failed), 0)))
#endif

#endif //TEST_ASSERT_H
