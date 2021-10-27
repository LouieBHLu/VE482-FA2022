#include "lab5_dlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node_t{
    char *k;
    dlistValue v;
    struct node *next;
} node;

typedef struct dlist_t{
    node *head;
    dlistValueType type;
    int size;
} list;

dlist createDlist(dlistValueType type){
    if(type == DLIST_UNKOWN) return NULL;

    dlist l = (list *)malloc(sizeof(list));
    ((list *)l)->head = NULL;
    ((list *)l)->type = type;
    ((list *)l)->size = 0;
    return l;
}

int dlistIsEmpty(dlist_const this){
    if(((list *)this)->size == 0) return 1;
    return 0;
}

void dlistAppend(dlist this, const char* key, dlistValue value){
    node *n = (node *)malloc(sizeof(node));
    n->v = value;
    n->k = key;
    n->next = NULL;

    if(dlistIsEmpty(this)) ((list *)this)->head = n;
    else{
        node *tmp = ((list *)this)->head;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = n;        
    }   
    ((list *)this)->size++;
}

static int random_sort(void* x, void* y){
    int result = strcmp(((node *) y)->v.strValue, ((node *) x)->v.strValue);
    int rnd = result * rand() % 2;
    if(rnd == 1) return rnd;
    else return -1;
}

int (*compare[3][3]) (const void *, const void *) = {
    {random_sort, random_sort, random_sort},
    {random_sort, random_sort, random_sort},
    {random_sort, random_sort, random_sort}
};

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method){
    if(((list *)this)->size == 0) return;

    node **nodes = (node **)malloc(sizeof(node *) * (unsigned long)((list *)this)->size);
    for(int i = 0; i < ((list *)this)->size - 1; i++) nodes[i] = NULL; 

    int i = 0;
    node *tmp = ((list *)this)->head;
    while(tmp != NULL){
        nodes[i] = tmp;
        tmp = tmp->next;
        i++;
    }

    tmp = NULL;
    for(int i = 0; i < ((list *)this)->size - 1; i++){
        for(int j = 0; j < ((list *)this)->size - 1 - i; j++){
            if(compare[((list *)this)->type][((list *)this)->type](nodes[j+1], nodes[j]) <= 0){
                tmp = nodes[j];
                nodes[j] = nodes[j+1];
                nodes[j+1] = tmp;
            }
        }
    }

    ((list *)this)->head = nodes[0];
    for(int i = 0; i < ((list *)this)->size - 1; i++) nodes[i]->next = nodes[i+1];
    nodes[((list *)this)->size - 1]->next = NULL;
}

void dlistPrint(dlist_const this){
    if(((list *)this)->size == 0) return;

    node *tmp;
    while(tmp->next != NULL){
        if(((list *)this)->type == DLIST_INT) printf("%s=%d\n", tmp->k, tmp->v);
        else if(((list *)this)->type == DLIST_STR) printf("%s=%s\n", tmp->k, tmp->v);
        else printf("%s=%lf\n", tmp->k, tmp->v);
        tmp = tmp->next;
    }
}

void dlistFree(dlist this){
    if(((list *)this)->size == 0) return;

    node *tmp = ((list *)this)->head;
    while(tmp->next != NULL){
        node *curnode = tmp;
        tmp = tmp->next;
        free(curnode);
    }
}







