#ifndef EX2_H
#define EX2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAX_KEY 1024
#define MAX_STRING 1024

#define INT_TYPE 0
#define CHAR_TYPE 1
#define DOUBLE_TYPE 2
#define RAND_SORT 2
#define INC_SORT 0
#define DEC_SORT 1
#define NULL_TYPE -1


typedef struct node{
    char* key;
    void* data;
    struct node* next;
} node;

typedef struct list{
    node* head;
    int size;
} llist;

typedef struct type{
    int sortingType;
    int dataType;
} type;

void list(llist* l);

void addNode(llist* l, node* n);

void* listSearch(llist* l, const char* k);

void listDes(llist* l);

void readType(type* t, const char* argv[]);

void readFile(type* t, const char* argv[]);

void insertNode(llist* l, char* k, void* d);

void listDes(llist* l);

void sortNodes(llist* l, type* t);

void writeFile(llist*l, type* t);

#endif
