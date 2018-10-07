//
// Created by chenzheng on 21/01/2017.
//

#ifndef TEST_LINKLIST_H
#define TEST_LINKLIST_H
#include <stdlib.h>

//typedef struct ListElem_ {
//void *data;
//    struct ListElem_ *next;
//} ListElem;

/* Define a structure for linked lists */
/* typedef struct List_ {
    int size;
    ListElem *head;
} List;

extern void list_init(List *list);
extern void list_destroy(List *list);
extern int list_add(List *list, ListElem *element, const void *data);
extern int list_delete(List *list, ListElem *element, void **data);
extern int list_size(List *list); */

struct node_int {
    int data;
    struct node_int *next;
};

/* The length() function takes a linked list and computes the number of elements in the list */
int length_int(struct node_int *head);
void push_int(struct node_int **headRef, int data);
void append_int(struct node_int **headRef, int data);
int pop_int(struct node_int **headRef);
void print_int(struct node_int *head);

#endif //TEST_LINKLIST_H
