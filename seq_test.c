//
// Created by chenzheng on 12/11/2017.
//

#include "seq.h"
#include <stdio.h>

int main()
{
    char *a = "C";
    char *b = "ML";
    char *c = "C++";
    char *d = "Icon";
    char *e = "AWK";
    void *pElem;
    void **ppElem;
    Seq_T names;

    printf("a = %p\n", a);
    printf("size of (void **) = %ld\n", sizeof(void **));
    printf("size of (void *) = %ld\n", sizeof(void *));
    names = Seq_seq((void *)&a, (void *)&b, (void *)&c, (void *)&d, (void *)&e, NULL);
    pElem = Seq_get(names, 0);
    printf("pElem = %p\n", pElem);
    printf("result = %s\n", *((char **)pElem)); /* type conversion */
    /* --- double pointer --- */
    ppElem = Seq_get(names, 0);
    printf("ppElem = %p\n", ppElem);
    printf("*ppElem = %p\n", *ppElem);
    printf("result = %s\n", (char *)(*ppElem));

    return 0;
}
