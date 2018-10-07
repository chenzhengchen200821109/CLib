//
// Created by chenzheng on 07/10/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "fmt.h"
#include "thread.h"
#include "chan.h"

struct args
{
    Chan_T c;
    int n, last;
};

int main(int argc, char* argv[])
{
    struct args args;

    Thread_init(1, NULL);
    args.c = Chan_new();
    Thread_new(source, &args, sizeof(args), NULL);
    args.n = argc > 2 ? atoi(argv[2]) : 5;
    args.last = argc > 1 ? atoi(argv[1]) : 1000;
    Thread_new(sink, &args, sizeof(args), NULL);
    Thread_exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}