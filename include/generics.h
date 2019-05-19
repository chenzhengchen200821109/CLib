//
// Created by chenzheng on 29/12/2016.
//

#ifndef TEST_SWAP_H
#define TEST_SWAP_H
#endif //TEST_SWAP_H

void swap(void *, void *, int);

void *lsearch(void *, void *, int, int);

void *lsearch_g(void *, void *, int, int, int (*cmpFunc)(void *, void *));

void rotate(void *, void *, void *);

void qsort_g(void *base, int n, int elemSize, int (*cmpFunc)(void *, void *));