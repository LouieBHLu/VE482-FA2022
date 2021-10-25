#include "ui.h"

void readType(type* t, char* argv[]){
    t->dataType = NULL_TYPE;
    t->sortingType = NULL_TYPE;
    char* tmpStr = malloc(sizeof(char) * MAX_STRING);
    memset(tmpStr, 0, MAX_STRING);
    strcpy(tmpStr, argv[1]);
    char* token = strtok(tmpStr, "_.");
    token = strtok(NULL, "_.");

    if(!strcmp(token, "int")) t->dataType = INT_TYPE;
    else if(!strcmp(token, "double")) t->dataType = DOUBLE_TYPE;
    else if(!strcmp(token, "char")) t->dataType = CHAR_TYPE;

    if(!strcmp(argv[2], "rand")) t->sortingType = RAND_SORT;
    else if(!strcmp(argv[2], "inc")) t->sortingType = INC_SORT;
    else if(!strcmp(argv[2], "dec")) t->sortingType = DEC_SORT;

    free(tmpStr);
}

void insertNode(llist* l, char* k, void* d){
    node* newNode = (node *)malloc(sizeof(node));
    newNode->key = k;
    newNode->data = d;
    newNode->next = NULL;
    addNode(l, newNode);
}

void writeFile(llist* l, type* t){
    char* outFile = (char *)malloc(sizeof(char) * MAX_STRING);
    memset(outFile, 0, MAX_STRING);
    strcat(outFile, "tmp/");

    if(t->sortingType == INC_SORT) strcat(outFile, "inc_");
    else if(t->sortingType == DEC_SORT) strcat(outFile, "dec_");
    else if(t->sortingType == RAND_SORT) strcat(outFile, "rand_");

    if(t->dataType == INT_TYPE) strcat(outFile, "int.txt");
    else if(t->dataType == CHAR_TYPE) strcat(outFile, "char.txt");
    else if(t->dataType == DOUBLE_TYPE) strcat(outFile, "double.txt");

    printf("writing %s\n", outFile);
    FILE* f = fopen(outFile, "w");

    if(l->head == NULL) return;
    node* tmp = l->head;
    while(tmp != NULL){
        if(t->dataType == INT_TYPE) fprintf(f, "%s=%d\n", tmp->key, *((int *) tmp->data));
        else if(t->dataType == DOUBLE_TYPE) fprintf(f, "%s=%lf\n", tmp->key, *((double *) tmp->data));
        else fprintf(f, "%s=%s\n", tmp->key, (char*)tmp->data);

        tmp = tmp->next;
    }

    fclose(f);
    free(outFile);
}

void cmd(type* t, char* argv[]){
    FILE *f = fopen(argv[1], "r");
    char* tmpKey;
    char* tmpData;
    char* line = (char*)malloc(sizeof(char)*MAX_STRING);
    memset(line, 0, MAX_STRING);

    llist l;
    list(&l);
    printf("reading %s\n",argv[1]);

    while(fgets(line, MAX_STRING, f) != NULL){
        tmpKey = strtok(line, "=");
        tmpData = strtok(NULL, "\n");

        // Create a void pointer to be casted by any data type
        void* d;
        if(t->dataType == INT_TYPE){
            d = (void *)malloc(sizeof(int));
            *(int *) d = atoi(tmpData);
        }
        else if(t->dataType == CHAR_TYPE){
            d = (void *)malloc(sizeof(char) * MAX_STRING);
            memset(d, 0, MAX_STRING);
            strcpy((char *) d, tmpData);
        }
        else if(t->dataType == DOUBLE_TYPE){
            d = (void *)malloc(sizeof(double));
            *(double *) d = atof(tmpData);
        }
        else{
            d = malloc(sizeof(char) * MAX_STRING);
            memset(d, 0, MAX_STRING);
            strcpy(d, tmpData);
        }
        
        char* k = (char*)malloc(sizeof(char) * MAX_STRING);
        memset(k, 0, MAX_KEY);
        strcpy(k, tmpKey);
        // printf("Finish one line, and insert node\n");
        
        insertNode(&l, k, d);
        memset(line, 0, MAX_STRING);
    }
    free(line);
    printf("sorting elements\n");
    sortNodes(&l, t);
    writeFile(&l, t);
    listDes(&l);
    fclose(f);
}

void menu(){
    printf("Welcome to VE482 Lab 5 Layer Programming!");
    int exit = 0;
    
    while(!exit){
        printf(
            "usage: <input file path> <sorting type>\n"
            "input file format: <sorting type>_<datatype.txt>\n"
            "sorting types supported: rand, inc, dec\n"
            "data types supported: int, char, double\n"
            "\nPlease input or type \"exit\" to quit the program.\n\n"
        );
        
        type t;
        char *arguments[3];
        for (size_t i = 0; i < 3; i++) arguments[i] = (char *)malloc(sizeof(char) * MAX_STRING);

        char line[MAX_STRING];
        if(fgets(line, MAX_STRING, stdin) == NULL) printf("Error: Input can not be empty!\n");

        char *token;
        token = strtok(line, " \n");
        if(token == NULL) printf("Error: Wrong length!\n");
        strcpy(arguments[1], token);
        if(!strcmp(arguments[1], "exit")){
            exit = 1;
            for (size_t i = 0; i < 3; i++) free(arguments[i]);
            continue;
        } 

        token = strtok(NULL, " \n");
        if(token == NULL) printf("Error: Wrong Input!\n");
        strcpy(arguments[2], token);
        readType(&t, arguments);
        cmd(&t, arguments);
        printf("Operation Succeeded!\n"
                "------------------\n\n");
        for (size_t i = 0; i < 3; i++) free(arguments[i]);
    }
}
