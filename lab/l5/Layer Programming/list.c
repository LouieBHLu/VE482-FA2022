#include "list.h"

void list(llist* l){
    l->head = NULL;
    l->size = 0;
}

void addNode(llist* l, node* n){
    if(l->head == NULL) l->head = n;
    else{
        node* tmp = l->head;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = n;
    }
    l->size++;
}

void* listSearch(llist* l, const char* k){
    if(l->size == 0) return NULL;
    node* tmp = l->head;
    while(tmp != NULL){
        if(!strcmp(k, tmp->key)) return tmp->data;
        else tmp = tmp->next;
    }
    return NULL;
}

void listDes(llist* l){
    if(l->size == 0) return;

    node* tmp = l->head;
    while(tmp->next != NULL){
        node* lastNode = tmp;
        free(tmp->key);
        free(tmp->data);
        tmp = tmp->next;
        free(lastNode);    
    }
}
