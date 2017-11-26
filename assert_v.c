//
// Created by chenzheng on 06/03/2017.
//

#include "assert_v.h"

const Except_T Assert_Failed = { "Assertion failed" };

void (assert)(int e) {
    assert(e);
}