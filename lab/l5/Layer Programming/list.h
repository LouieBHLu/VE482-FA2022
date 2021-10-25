#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node{
    char* key;
    void* data;
    struct node* next;
} node;

typedef struct list{
    node* head;
    int size;
} llist;

void list(llist* l);

void addNode(llist* l, node* n);

void* listSearch(llist* l, const char* k);

void listDes(llist* l);

#endif