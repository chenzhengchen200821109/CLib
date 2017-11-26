//
// Created by chenzheng on 09/11/2017.
//

#include <stdio.h>
#include "table.h"

int main()
{
    int i;
    Table_T table;
    int value1 = 1;
    int value2 = 2;
    int value3 = 3;

    table = Table_new(0, NULL, NULL);
    Table_put(table, "key1", &value1);
    Table_put(table, "key2", &value2);
    Table_put(table, "key2", &value3);
    void **array = Table_toArray(table, NULL);
    for (i = 0; array[i]; i += 2)
        printf("%d\t%s\n", *(int *)array[i+1], (char *)array[i]);

    return 0;
}

