//
// Created by chenzheng on 21/01/2017.
//

#include "linklist.h"
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

/* void list_init(List *list) {
    list->size = 0;
    list->head = NULL;
}

int list_add(List *list, ListElem *element, const void *data) {
    ListElem *new_element;
} */

/*
 * print out the value of data field of the list.
 */
void print_int(struct node_int *head) {
    struct node_int *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

/*
 * Given a linked list head pointer, compute and return the number of nodes in the list
 */
int length_int(struct node_int *head) {
    struct node_int *current = head;
    int count = 0;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

/*
 * Takes a list and a data value. Creates a new link with the given data and pushes
 * it onto the front of the list. The list is not passed in by its head pointer. Instead
 * the list is passed in as a "reference" pointer to the head pointer -- this allows us to
 * modify the caller's memory.
 */
void push_int(struct node_int **headRef, int data) {
    struct node_int *newNode = malloc(sizeof(struct node_int));
    newNode->data = data;
    newNode->next = *headRef;
    *headRef = newNode;
}

/*
 * append() function is like push(), except it adds the new node at the tail end of the
 * list instead of the head.
 */
void append_int(struct node_int **headRef, int data) {
    struct node_int *current = *headRef;
    struct node_int *newNode;

    newNode = malloc(sizeof(struct node_int));
    newNode->data = data;
    newNode->next = NULL;

    /* special case for length 0 */
    if (current == NULL) {
        *headRef = newNode;
    }
    else {
        /* locate the last node */
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*
 * pop() function is the opposite of push(), it pops value from the front of the list.
 */
int pop_int(struct node_int **headRef) 
{
    int data;
    struct node_int *current = *headRef;

    /* special case for length 0 */
    if (current == NULL) {
        return -1;
    }
    else {
        data = current->data;
        *headRef = current->next;
        free(current);
        return data;
    }
}
