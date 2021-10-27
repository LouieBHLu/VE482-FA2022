#include "ex2.h"

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

void insertNode(llist* l, char* k, void* d){
    node* newNode = (node *)malloc(sizeof(node));
    newNode->key = k;
    newNode->data = d;
    newNode->next = NULL;
    addNode(l, newNode);
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

int int_inc(const void* x, const void* y){
    return *((int *)(((node *) x)->data)) - *((int *)(((node *) y)->data));
}

int int_dec(const void* x, const void* y){
    return *((int *)(((node *) y)->data)) - *((int *)(((node *) x)->data));
}

int double_inc(const void* x, const void* y){
    double result = *((double *)(((node *) x)->data)) - *((double *)(((node *) y)->data));
    return (int) result;
}

int double_dec(const void* x, const void* y){
    double result = *((double *)(((node *) y)->data)) - *((double *)(((node *) x)->data));
    return (int) result;
}

int char_inc(const void* x, const void* y){
    return strcmp((char *)(((node *) x)->data), (char *)(((node *) y)->data));
}

int char_dec(const void* x, const void* y){
    return strcmp((char *)(((node *) y)->data), (char *)(((node *) x)->data));
}

int random_sort(const void* x, const void* y){
    assert(x != NULL);
    assert(y != NULL);
    return (((rand()) % 2) * 2 - 1);
}

// Referring: https://www.geeksforgeeks.org/function-pointer-in-c/
int (*cmp[3][3]) (const void* x, const void* y) = {
    {int_inc, int_dec, random_sort},
    {char_inc, char_dec, random_sort},
    {double_inc, double_dec, random_sort}
};

void sortNodes(llist* l, type* t){
    if(l->size == 0) return;
    
    node **nodes = (node **)malloc(sizeof(node *) * (unsigned long)(l->size));
    for(int i = 0; i < l->size - 1; i++) nodes[i] = NULL; 

    int i = 0;
    node* tmp = l->head;
    while(tmp != NULL){
        nodes[i++] = tmp;
        tmp = tmp->next;
    }
    
    tmp = NULL;
    int j;
    for(int i = 1;i < l->size; i++){
        tmp = nodes[i];
        j = i - 1;
        while(j >= 0 && (cmp[t->dataType][t->sortingType](nodes[j], tmp) >= 0)){
            nodes[j + 1] = nodes[j];
            j--;
        }
        nodes[j + 1] = tmp;
    }

    l->head = nodes[0];
    for(int i = 0; i < l->size - 1; i++) nodes[i]->next = nodes[i+1];
    nodes[l->size - 1]->next = NULL;

    free(nodes);
}

void writeFile(const llist* l, type* t){
    // char* outFile = (char *)malloc(sizeof(char) * 100);
    char outFile[100];
    memset(outFile, 0, 100);
    // strcat(outFile, "tmp/");

    if(t->sortingType == INC_SORT) strcat(outFile, "inc_");
    else if(t->sortingType == DEC_SORT) strcat(outFile, "dec_");
    else strcat(outFile, "rand_");

    if(t->dataType == INT_TYPE) strcat(outFile, "int.txt");
    else if(t->dataType == DOUBLE_TYPE) strcat(outFile, "double.txt");
    else strcat(outFile, "char*.txt");

    printf("writing %s\n", outFile);
    FILE* f = fopen(outFile, "w");

    if(l->head == NULL){
        fclose(f);
        return;
    } 
    node* tmp = l->head;
    while(tmp != NULL){
        if(t->dataType == INT_TYPE) fprintf(f, "%s=%d\n", tmp->key, *((int *) tmp->data));
        else if(t->dataType == DOUBLE_TYPE) fprintf(f, "%s=%lf\n", tmp->key, *((double *) tmp->data));
        else fprintf(f, "%s=%s\n", tmp->key, (char*)tmp->data);
        tmp = tmp->next;
    }

    fclose(f);
    // free(outFile);
}

void listDes(llist* l){
    if(l->size == 0) return;

    node* tmp = l->head;
    while(tmp != NULL){
        node* lastNode = tmp;
        free(tmp->key);
        free(tmp->data);
        tmp = tmp->next;
        free(lastNode);    
    }
    free(l);
}

void readType(type* t, const char* argv[]){
    t->dataType = NULL_TYPE;
    t->sortingType = NULL_TYPE;
    char* tmpStr = malloc(sizeof(char) * MAX_STRING);
    memset(tmpStr, 0, MAX_STRING);
    strcpy(tmpStr, argv[1]);
    char* token = strtok(tmpStr, "_.");
    token = strtok(NULL, "_.");

    if(!strcmp(token, "int")) t->dataType = INT_TYPE;
    else if(!strcmp(token, "double")) t->dataType = DOUBLE_TYPE;
    else if(!strcmp(token, "char*")) t->dataType = CHAR_TYPE;

    if(!strcmp(argv[2], "rand")) t->sortingType = RAND_SORT;
    else if(!strcmp(argv[2], "inc")) t->sortingType = INC_SORT;
    else if(!strcmp(argv[2], "dec")) t->sortingType = DEC_SORT;

    free(tmpStr);
}

void readFile(type* t, const char* argv[]){
    FILE *f = fopen(argv[1], "r");
    char* tmpKey;
    char* tmpData;
    char* line = (char *)malloc(sizeof(char) * 2 * MAX_STRING);
    memset(line, 0, 2 * MAX_STRING);
    fprintf(stderr, "%s", line);

    llist l;
    list(&l);
    printf("reading %s\n",argv[1]);

    while(fgets(line, MAX_STRING, f) != NULL){
        tmpKey = strtok(line, "=");
        tmpData = strtok(NULL, "\n");

        // Create a void pointer to be casted by any data type
        void* d = NULL;
        if(t->dataType == INT_TYPE){
            int v = atoi(tmpData);
            d = (void *)malloc(sizeof(int));
            *(int *) d = v;
        }
        else if(t->dataType == DOUBLE_TYPE){
            double v = atof(tmpData);
            d = (void *)malloc(sizeof(double));
            *(double *) d = v;
        }
        else{
            d = (void *)malloc(sizeof(char) * MAX_STRING);
            memset(d, 0, MAX_STRING);
            strcpy((char *) d, tmpData);
        }
        
        char* k = (char*)malloc(sizeof(char) * MAX_STRING);
        memset(k, 0, MAX_KEY);
        strcpy(k, tmpKey);
        insertNode(&l, k, d);
        memset(line, 0, 2 * MAX_STRING);
    }

    free(line);
    printf("sorting elements\n");
    sortNodes(&l, t);
    writeFile(&l, t);
    listDes(&l);
    fclose(f);
}


int main(int argc, char const *argv[]){
    srand((unsigned int) time(NULL));

    if(argc != 3){
        printf("Error: wrong arguments number!\n");
        return 0;
    }
    type t;
    readType(&t, argv);
    // printf("Finish reading type.\n");
    readFile(&t, argv);
    // fprintf(stderr, "asd");
    return 0;
}
