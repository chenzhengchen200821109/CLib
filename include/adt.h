//
// Created by chenzheng on 30/12/2016.
//

#ifndef _ADT_H_
#define _ADT_H_

typedef struct {
    int *elems;
    int logicalen; /* logicalen is used for detect whether alloclength has been used up */
    int alloclength;
} Stack;

//
void StackNew(Stack *s);
void StackDispose(Stack *s);
void StackPush(Stack *s, int value);
int StackPop(Stack *s);

// a more generics version of stack structure
typedef struct {

    void *elems;
    int elemSize;
    int logicalength;
    int alloclength;

} Stack_g;

void StackNew_g(Stack_g *s, int elemSize);
void StackDispose_g(Stack_g *s);
void StackPush_g(Stack_g *s, void *elemAddr);
void StackPop_g(Stack_g *s, void *elemAddr);

#endif // _ADT_H_