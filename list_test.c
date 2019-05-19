//
// Created by chenzheng on 31/10/2017.
//

/*
 * illustrations for using list interface
 */
#include <stdio.h>
#include "list.h"

void printList(void **x, void *cl)
{
    char **str = (char **)x;
    FILE *fp = cl;

    fprintf(fp, "%s\n", *str);
}

int main()
{
    List_T p1, p2, p3;

    p1 = List_list_v2("abc", "def", NULL);
    List_map(p1, printList, stdout);
    printf("--- End of List_list_v2() ---\n");
    p2 = List_copy_v2(p1);
    List_map(p2, printList, stdout);
    printf("--- End of List_copy_v2() ----\n");
    p3 = List_append_v2(p1, p2);
    List_map(p3, printList, stdout);
    printf("--- End of List_append_v2() ----\n");

    return 0;
}
