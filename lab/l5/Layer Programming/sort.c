#include "sort.h"

int int_inc(void* x, void* y){
    return *((int *)(((node *) x)->data)) - *((int *)(((node *) y)->data));
}

int int_dec(void* x, void* y){
    return *((int *)(((node *) y)->data)) - *((int *)(((node *) x)->data));
}

int double_inc(void* x, void* y){
    return *((double *)(((node *) x)->data)) - *((double *)(((node *) y)->data));
}

int double_dec(void* x, void* y){
    return *((double *)(((node *) y)->data)) - *((double *)(((node *) x)->data));
}

int char_inc(void* x, void* y){
    return strcmp((char *)(((node *) x)->data), (char *)(((node *) y)->data));
}

int char_dec(void* x, void* y){
    return strcmp((char *)(((node *) y)->data), (char *)(((node *) x)->data));
}

int random_sort(void* x, void* y){
    int rnd = rand() % 2;
    if(rnd == 1) return rnd;
    else return -1;
}

// Referring: https://www.geeksforgeeks.org/function-pointer-in-c/
int (*compare[3][3]) (void* x, void* y) = {
    {int_inc, int_dec, random_sort},
    {char_inc, char_dec, random_sort},
    {double_inc, double_dec, random_sort}
};


void sortNodes(llist* l, type* t){
    if(l->size == 0) return;
    
    node* nodes[l->size];
    int i = 0;
    node* tmp = l->head;
    while(tmp != NULL){
        nodes[i] = tmp;
        i++;
        tmp = tmp->next;
    }

    tmp = NULL;
    for(int i = 0; i < l->size - 1; i++){
        for(int j = 0; j < l->size - 1 - i; j++){
            if(compare[t->dataType][t->sortingType](nodes[j+1], nodes[j]) <= 0){
                tmp = nodes[j];
                nodes[j] = nodes[j+1];
                nodes[j+1] = tmp;
            }
        }
    }

    l->head = nodes[0];
    for(int i = 0; i < l->size - 1; i++) nodes[i]->next = nodes[i+1];
    nodes[l->size - 1]->next = NULL;
}