#include "util.h"
#include "main.h"

void printDebug(char *m){
    #ifdef DEBUG
        printf("Debug: %s", m);
        fflush(stdout);
    #endif
    char a = m[0];
    a++;
}

void printPrompt(char *m){
    printf("%s", m);
    fflush(stdout);
}

void printError(char *m){
    printf("%s", m);
    fflush(stdout);
}

void init(){    
    isBack = 0;
    FileStdin = dup(0);
    FileStdout = dup(1);
    FileIn = (char *)malloc(sizeof(char) * MAX_FILE);
    FileOut = (char *)malloc(sizeof(char) * MAX_FILE);
    line = (char *)malloc(sizeof(char) * MAX_CMD_LENGTH);
    comLine = (char *)malloc(sizeof(char) * MAX_CMD_LENGTH);
    memset(line,0,MAX_CMD_LENGTH);
    memset(comLine,0,MAX_CMD_LENGTH);
}

void freefile(){
    dup2(FileStdin, 0);
    dup2(FileStdout, 1);
    free(FileIn);
    free(FileOut);
}

void CTRLCHandle(){
    sigaction(SIGINT, &old_act, NULL);
    printDebug("Ctrl-C\n");

    if(status == PARENT){
        printPrompt("\n");
        status = PARENT_EXIT;
        printDebug("Status parent normal\n");
    }
    else if(status == CHILD){
        printDebug("Status child normal\n");
        exit(0);
    }
}

void errorClose(char *m, char ** args, const int argCnt){
    printf("%s\n", m);
    for(int i = 0; i < argCnt; i++){
        if (args[i] != NULL) free(args[i]);
    }
    free(args);
    freefile();
    for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
    if(lastDir != NULL) free(lastDir);
    exit(0);
}

void replaceMark(const char mark, unsigned int i, char* marks, int* markNum){
    if(comLine[i] == mark){
        comLine[i] = REPLACE_IDENTITY;
        marks[(*markNum)++] = mark;
    }
}
