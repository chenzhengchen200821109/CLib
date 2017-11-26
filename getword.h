//
// Created by chenzheng on 02/11/2017.
//

#ifndef CLIB_GETWORD_H
#define CLIB_GETWORD_H

#include <stdio.h>

extern int getword(FILE *fp, char *buf, int size, int first(int c), int rest(int c));

#endif //CLIB_GETWORD_H
