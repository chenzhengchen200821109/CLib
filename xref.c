//
// Created by chenzheng on 08/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "table.h"
#include "atom.h"
#include "set.h"
#include "mem.h"
#include "getword.h"

int first(int c);
int rest(int c);
void xref(const char *, FILE *, Table_T);
int compare(const void *, const void *);
void print(Table_T);
int cmpint(const void *, const void *);

int linenum;

int main(int argc, char *argv[])
{
    int i;
    Table_T identifiers = Table_new(0, NULL, NULL);

    for (i = 0; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can't open '%s' (%s)\n", argv[0], argv[i], strerror(errno));
        } else {
            xref(argv[i], fp, identifiers);
            fclose(fp);
        }
    }
    if (argc == 1) xref(NULL, stdin, identifiers);
    {
        int i;
        void **arrray = Table_toArray(identifiers, NULL);
        qsort(arrray, Table_length(identifiers), 2 * sizeof(*arrray), compare);
        for (i = 0; arrray[i]; i += 2) {
            printf("%s", (char *)arrray[i]);
            print(arrray[i]);
        }
        FREE(arrray);
    }
}

int compare(const void *x, const void *y)
{
    return strcmp(*(char **)x, *(char **)y);
}

void print(Table_T files) {
    int i;
    void **array = Table_toArray(files, NULL);

    qsort(array, Table_length(files), 2 * sizeof(*array), compare);
    for (i = 0; array[i]; i += 2) {
        if (*(char *)array[i] != '\0')
            printf("\t%s:", (char *)array[i]);
        {
            int j;
            void **lines = Set_toArray(array[i+1], NULL);
            qsort(lines, Set_length(array[i+1]), sizeof(*lines), cmpint);
            for (j = 0; lines[j]; j++)
                printf(" %d", *(int *)lines[j]);
            FREE(lines);
        }
    }
}

int cmpint(const void *x, const void *y)
{
    if (**(int **)x < **(int **)y)
        return -1;
    else if (**(int **)x > **(int **)y)
        return +1;
    else
        return 0;
}