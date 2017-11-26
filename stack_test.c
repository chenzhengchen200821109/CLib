#include <stdio.h>
#include "stack.h"

int main() {

    int elem1 = 1;
    int elem2 = 2;
    int elem3 = 3;
    int elem4 = 4;
    int *presult;

    /* stack implementation test */
    Stack_T names = Stack_new(4);
    Stack_push(names, &elem1);
    Stack_push(names, &elem2);
    Stack_push(names, &elem3);
    Stack_push(names, &elem4);
    presult = Stack_pop(names);
    printf("%d is popping out\n", *presult);

    return 0;
}