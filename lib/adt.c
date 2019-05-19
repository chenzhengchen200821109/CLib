//
// Created by chenzheng on 30/12/2016.
//

#include "adt.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void StackNew(Stack *s) {

    s->logicalen = 0; /* magic number */
    s->alloclength = 4;
    s->elems = (malloc(4 * sizeof(int)));

    assert(s->elems != NULL); /* malloc call failed */

};

void StackDispose(Stack *s) {

    free(s->elems);

};

void StackPush(Stack *s, int value) {

    if (s->logicalen == s->alloclength) {
        s->alloclength *= 2;
        s->elems = realloc(s->elems, s->alloclength * sizeof(int));

        assert(s->elems != NULL); /* realloc call failed */
    }
    s->elems[s->logicalen] = value;
    s->logicalen++;

};

int StackPop(Stack *s) {

    assert(s->logicalen > 0);
    s->logicalen--;
    return s->elems[s->logicalen];

};

//
void StackNew_g(Stack_g *s, int elemSize) {

    s->logicalength = 0;
    s->alloclength = 4;
    s->elemSize = elemSize;
    s->elems = malloc(s->alloclength * s->elemSize);
    assert(s->elems != NULL);

};

void StackDispose_g(Stack_g *s) {

    free(s->elems);

};

void StackPush_g(Stack_g *s, void *elemAddr) {

    if (s->logicalength == s->alloclength) {
        s->alloclength *= 2; /* double the size of the stack structure */
        s->elems = realloc(s->elems, s->alloclength * s->elemSize);
        assert(s->elems != NULL);
    }
    void *target = (char *)s->elems + s->logicalength * s->elemSize;
    memcpy(target, elemAddr, s->elemSize);
    s->logicalength++;
};

// the address of poped element will be written back to the pointer elemAddr.
void StackPop_g(Stack_g *s, void *elemAddr) {

    assert(s->logicalength > 0);

    void *source = (char *)s->elems + (s->logicalength - 1) * s->elemSize;

    memcpy(elemAddr, source, s->elemSize);

    s->logicalength--;
};