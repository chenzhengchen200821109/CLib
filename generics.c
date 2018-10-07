//
// Created by chenzheng on 29/12/2016.
// a generics version of swap function
//
#include <string.h>
#include <stdlib.h>

static int qsort_gg(void *, int, int, int, int (*cmpFunc)(void *, void *));
static void qsort_ggg(void *, int, int, int, int (*cmpFunc)(void *, void *));
static int partition(void *, int, int, int, int (*cmpFunc)(void *, void *));
static int partition_g(void *, int, int, int, int (*cmpFunc)(void *, void *));

//
void swap(void *vp1, void *vp2, int size) {
    char buffer[size];
    memcpy(buffer, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, buffer, size);
}

//
void *lsearch(void *key, void *base, int n, int elemSize) {
    for (int i = 0; i < n; i++) {
        void *elemAddr = (char *)base + i * elemSize;
        if (memcmp(elemAddr, key, elemSize) == 0)
            return elemAddr;
    }
    return NULL;
}

//
void *lsearch_g(void *key, void *base, int n, int elemSize, int (*cmpFunc)(void *vp1, void *vp2)) {
    for (int i = 0; i < n; i++) {
        void *elemAddr = (char *)base + i * elemSize;
        if (cmpFunc(key, elemAddr) == 0)
            return elemAddr;
    }
    return NULL;
}

//
void rotate(void *front, void *middle, void *end) {
    int frontSize = (char *)middle - (char *)front;
    int backSize = (char *)end - (char *)middle;

    char buffer[frontSize]; /* buffer */

    memcpy(buffer, front, frontSize);
    memmove(front, middle, backSize);
    memcpy((char *)end - frontSize, buffer, frontSize);
}

// This implementation comes from the book of "mastering algorithms with C"
void qsort_g(void *base, int n, int elemSize, int (*cmpFunc)(void *vp1, void *vp2)) {

    // call qsort_g() function.
    //qsort_gg(base, elemSize, 0, n - 1, cmpFunc);
    qsort_ggg(base, elemSize, 0, n - 1, cmpFunc);

}

//
static int qsort_gg(void *base, int elemSize, int left, int right, int (*cmpFunc)(void *vp1, void *vp2)) {

    int j;

    while (left < right) {
        if ((j = partition(base, elemSize, left, right, cmpFunc)) < 0)
            return -1;
        if (qsort_gg(base, elemSize, left, j, cmpFunc) < 0)
            return -1;
        left = j + 1;
    }
    return 0;
}

static void qsort_ggg(void *base, int elemSize, int first, int last, int (*cmpFunc)(void *vp1, void *vp2)) {

    int pos;

    if (first >= last)
        return;
    else {
        pos = partition_g(base, elemSize, first, last, cmpFunc);
        qsort_ggg(base, elemSize, first, pos - 1, cmpFunc);
        qsort_ggg(base, elemSize, pos + 1, last, cmpFunc);
    }
}

//
static int partition(void *base, int elemSize, int left, int right, int (*cmpFunc)(void *vp1, void *vp2)) {

    char *a = base;
    void *pval, *tmp;
    int r[3];

    if ((pval = malloc(elemSize)) == NULL)
        return -1;

    if ((tmp = malloc(elemSize)) == NULL) {
        free(pval);
        return -1;
    }

    r[0] = (rand() % (right - left + 1)) + left;
    r[1] = (rand() % (right - left + 1)) + left;
    r[2] = (rand() % (right - left + 1)) + left;
    memcpy(pval, &a[r[1] * elemSize], elemSize);

    left--;
    right++;

    while (1) {
        do {
            right--;
        } while (cmpFunc(&a[right * elemSize], pval) > 0);

        do {
            left++;
        } while (cmpFunc(&a[left * elemSize], pval) < 0);

        if (left >= right) {
            break;
        }
        else {
            memcpy(tmp, &a[left * elemSize], elemSize);
            memcpy(&a[left * elemSize], &a[right * elemSize], elemSize);
            memcpy(&a[right * elemSize], tmp, elemSize);
        }
    }

    free(pval);
    free(tmp);

    return right;
}

//
static int partition_g(void *base, int elemSize, int first, int last, int (*cmpFunc)(void *vp1, void *vp2)) {

    void *pivotAddr = (char *)base + first * elemSize;
    void *leftAddr;
    void *rightAddr;

    int left = first + 1;
    int right = last;

    while (left <= right) {
        leftAddr = (char *)base + left * elemSize;
        while ((left <= right) && (cmpFunc(leftAddr, pivotAddr) < 0)) {
            left += 1;
            leftAddr = (char *)base + left * elemSize;
        }
        rightAddr = (char *)base + right * elemSize;
        while ((right >= left) && (cmpFunc(rightAddr, pivotAddr) >= 0)) {
            right -= 1;
            rightAddr = (char *)base + right * elemSize;
        }
        if (left < right) {
            swap(leftAddr, rightAddr, elemSize);
        }
        swap(pivotAddr, rightAddr, elemSize);
    }
    return right;
}

