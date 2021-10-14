#include "main.h"

int main(){
    act.sa_handler = &CTRLCHandle; // Can also be a signal
    backJobCnt = 0;
    lastDir = NULL;
    for (size_t i = 0; i < MAX_JOB; i++) backJob[i] = NULL;

    homeDir = getenv("HOME");
    if(homeDir == NULL) homeDir = getpwuid(getuid())->pw_dir;

    char curCmd[MAX_PATH];
    curDir = (char *)malloc(sizeof(char) * MAX_PATH);
    memset(curDir, 0, MAX_PATH);

    if(getcwd(curCmd, sizeof(curCmd)) == NULL) strcpy(curDir,homeDir); // path too small
    else strcpy(curDir, curCmd);

    // Main loop
    while(1){
        // Set signal
        sigaction(SIGINT, &act, &old_act);
        status = PARENT;
        printPrompt("mumsh $ ");
        // Initialization
        init();

        /** Read
         * @line: One original input line
         * @comLine: Combine several input lines
         */
        int isInputEnd = 1, isFirstLine = 1;
        int fgetsCTRLC = 0;
        int singleQuotationFinished = 1;
        int doubleQuotationFinished = 1;

        while(!isInputEnd || isFirstLine){
            isInputEnd = 1;
            if(isFirstLine) isFirstLine = 0;
            else printPrompt("> ");
            // fgets to deal with CTRL+D
            if(fgets(line, MAX_CMD_LENGTH, stdin) == NULL){
                printDebug("fgets quit.\n");
                if(errno == EINTR){
                    // If Ctrl+C
                    printDebug("Interrupted system call.\n");
                    errno = 0; 
                    fgetsCTRLC = 1;
                    break;
                }
                free(line);
                free(comLine);
                freefile();
                if(lastDir != NULL) free(lastDir);
                free(curDir);
                // free local var
                for (int i = 0; i < backJobCnt; i++) free(backJob[i]);
                printPrompt("exit\n");
                exit(0);
            }
            // check one-sided quotation mark
            for (unsigned int i = 0; i < strlen(line); i++){
                if(line[i] == '\''){
                    if(singleQuotationFinished && doubleQuotationFinished) singleQuotationFinished = 0;
                    else if(!singleQuotationFinished) singleQuotationFinished = 1;
                }
                else if(line[i] == '\"'){
                    if(singleQuotationFinished && doubleQuotationFinished) doubleQuotationFinished = 0;
                    else if(!doubleQuotationFinished && ((i > 0 && line[i-1] != '\\') || i == 0)) doubleQuotationFinished = 1;
                }
            }
            if(!singleQuotationFinished || !doubleQuotationFinished){
                strcat(comLine, line);
                isInputEnd = 0;
                continue;
            }
            // check if the input is over
            if(strlen(line) > 1){
                for (size_t i = strlen(line) - 2; i >= 0; i--){
                    if(line[i] == '>' || line[i] == '<' || line[i] == '|'){
                        isInputEnd = 0;
                        break;
                    }
                    else if (line[i] == ' ') continue;
                    else break;
                }
            }
            // If input is not over, cat and loop
            if(!isInputEnd){
                line[strlen(line) - 1] = ' ';
                strcat(comLine, line);
                // check syntax error
                for (size_t t = strlen(comLine)-1; t > 0; t--){
                    int branch = 0;
                    if(comLine[t]=='>' || comLine[t]=='<' || comLine[t]=='|'){
                        if(t <= 0) break;
                        branch = 1;
                        char tempChar = comLine[t];
                        t--;
                        if(t <= 0) break;
                        // if(--t <= 0) break;
                        while(t > 0 && (comLine[t] == ' ' || comLine[t] == '\n')) t--;
                        if(comLine[t] == '>' || comLine[t] == '<'){
                            char errorM[MAX_CMD_LENGTH];
                            sprintf(errorM, "syntax error near unexpected token `%c\'\n",tempChar);
                            printError(errorM);
                            fgetsCTRLC = 1;
                        }
                    }
                    if(branch) break;
                }
                if(fgetsCTRLC) break;
                continue;
            }
            // If input is over, cat and finish
            strcat(comLine, line);
            isInputEnd = 1;
        }
        
        free(line);
        if(fgetsCTRLC == 1 || status == PARENT_EXIT){
            free(comLine);
            freefile();
            continue;
        }
        fflush(stdin);

        /** Check if the inpupt is an empty line
         */
        int isEmpty = 1;
        for (size_t i = 0; i < strlen(comLine); i++){
            if(comLine[i] != 10 && comLine[i] != 32 && comLine[i] != 0) isEmpty = 0;
        }
        if(isEmpty){
            printDebug("Empty line.\n");
            free(comLine);
            freefile();
            continue;
        }

        /** Parse background "&" character
         */
        if(comLine[strlen(comLine) - 2] == '&'){
            isBack = 1;
            backJob[backJobCnt] = (char*)malloc(sizeof(char) * MAX_CMD_LENGTH);
            memset(backJob[backJobCnt],0,MAX_CMD_LENGTH);
            strcpy(backJob[backJobCnt], comLine);
            backJob[backJobCnt][strlen(backJob[backJobCnt]) - 1] = '\0';
        }

        /** Parse quotation
         */
        singleQuotationFinished = 1;
        doubleQuotationFinished = 1;
        unsigned int delNum = 0;
        unsigned int delList[MAX_CMD_LENGTH];
        int markNum = 0;
        char marks[MAX_CMD_LENGTH];
        memset(marks, 0, MAX_CMD_LENGTH);

        for (unsigned int i = 0; i < strlen(comLine) - 1; i++){
            if(!singleQuotationFinished || !doubleQuotationFinished){
                if(comLine[i] == '>'){
                    comLine[i] = REPLACE_IDENTITY;
                    marks[markNum++]='>';
                }
                if(comLine[i] == '<'){
                    comLine[i] = REPLACE_IDENTITY;
                    marks[markNum++]='<';
                }
                if(comLine[i] == '|'){
                    comLine[i] = REPLACE_IDENTITY;
                    marks[markNum++]='|';
                }
                if(comLine[i] == ' '){
                    comLine[i] = REPLACE_IDENTITY;
                    marks[markNum++]=' ';
                }
                if(comLine[i] == '\n'){
                    comLine[i] = REPLACE_IDENTITY;
                    marks[markNum++]='\n';
                }
            }
            if(comLine[i] == '\''){
                if(singleQuotationFinished && doubleQuotationFinished){
                    delList[delNum++] = i;
                    singleQuotationFinished = 0;
                }
                else if(singleQuotationFinished){
                    delList[delNum++] = i;
                    singleQuotationFinished = 1;
                } 
            }
            else if(comLine[i] == '\"'){
                if(singleQuotationFinished && doubleQuotationFinished){
                    delList[delNum++] = i;
                    doubleQuotationFinished = 0;
                }
                else if(!doubleQuotationFinished && ((comLine[i-1] != '\\' && i>0) || i == 0)){
                    delList[delNum++] = i;  
                    doubleQuotationFinished = 1;
                } 
                else if (!doubleQuotationFinished && comLine[i-1] == '\\' && i > 0) delList[delNum++] = i;
            }
        }

        char* tempLine = (char*)malloc(sizeof(char) * MAX_CMD_LENGTH);
        memset(tempLine, 0, MAX_CMD_LENGTH);
        for (size_t i = 0, j = 0, k = 0; i < strlen(comLine); i++){
            if(j < delNum && i == delList[j]) j++;
            else if(j >= delNum || (j < delNum && i != delList[j])) tempLine[k++] = comLine[i];
        }
        memset(comLine, 0, MAX_CMD_LENGTH);
        strcpy(comLine, tempLine);    
        free(tempLine);

        /** Parse redirection
         * @redirectLine: parse the whole line into tokens separated with >, <, >>
         */
        char *redirectLine = (char *)malloc(sizeof(char) * MAX_CMD_LENGTH * 2);
        memset(redirectLine, 0, MAX_CMD_LENGTH * 2);
        
        for(size_t i = 0, j = 0; i < strlen(comLine);){
            if(comLine[i]!='<' && comLine[i]!='>' && comLine[i]!='|') redirectLine[j++]=comLine[i++];
            else{
                redirectLine[j++] = ' ';
                redirectLine[j++] = comLine[i++];
                if(i < strlen(comLine) && comLine[i] == '>') redirectLine[j++] = comLine[i++];
                else if(i < strlen(comLine) && comLine[i] != ' ') redirectLine[j++] = ' ';
                else continue;
            }
        }

        free(comLine);
        char *tmpLine = (char *)malloc(sizeof(char) * MAX_CMD_LENGTH * 2);
        memset(tmpLine, 0, MAX_CMD_LENGTH * 2);
        strcpy(tmpLine, redirectLine);

        /** Parse arguments from the line
         * @args: arguments in the input line
         * @argCnt: number of arguments
         * @outRed: out redirection
         * @inRed: in redirection (cover)
         * @inCov: in redirection (append)
         * @ioError: I/O error
         */
        int outRed = 0;
        int inRed = 0;
        int outCov = 0;
        int ioError = 0;

        char **args = (char **)malloc(sizeof(char *) * MAX_CMD_LENGTH);
        for (int i = 0; i < MAX_CMD_LENGTH; i++) args[i] = NULL;
        int argCnt = 0;
        const char delim[4] = " \n\t";
        char *token;
        token = strtok(redirectLine, delim);

        while(token != NULL){
            if(token[0] == '<'){
                if(inRed == 1){
                    if(!ioError) printError("error: duplicated input redirection\n");
                    ioError = 1;
                }

                inRed = 1;
                if(strlen(token) == 1){
                    token = strtok(NULL, delim);
                    memset(FileIn, 0, MAX_FILE);
                    strcpy(FileIn, token);
                    token = strtok(NULL, delim);
                    continue;
                }
                else{
                    memset(FileIn, 0, MAX_FILE);
                    strcpy(FileIn, token);
                    // Remove the '<'
                    memmove(FileIn, FileIn + 1, strlen(FileIn));
                    token = strtok(NULL, delim);
                    continue;
                }
            }
            else if(token[0] == '>'){
                if(outRed == 1 || outCov == 1){
                    if(!ioError) printError("error: duplicated output redirection\n");
                    ioError = 1;
                }

                if(strlen(token) > 1 && token[1] == '>'){
                    if(strlen(token) > 2 && token[2] == '>'){
                        if(!ioError) printError("syntax error near unexpected token `>\'\n");
                        ioError = 1;
                    }
                    outCov = 1;
                    if(strlen(token) == 2){
                        token = strtok(NULL, delim);
                        memset(FileOut, 0, MAX_FILE);
                        strcpy(FileOut, token);
                        token = strtok(NULL, delim);
                        continue;
                    }
                    else{
                        memset(FileOut, 0, MAX_FILE);
                        strcpy(FileOut, token);
                        // Remove the '>>'
                        memmove(FileOut, FileOut + 2, strlen(FileOut));
                        token = strtok(NULL, delim);
                        continue;
                    }
                }
                else{
                    outRed = 1;
                    if(strlen(token) == 1){
                        token = strtok(NULL, delim);
                        memset(FileOut, 0, MAX_FILE);
                        strcpy(FileOut, token);
                        token = strtok(NULL, delim);
                        continue;
                    }
                    else{
                        memset(FileOut, 0, MAX_FILE);
                        strcpy(FileOut, token);
                        // Remove the '>'
                        memmove(FileOut, FileOut + 1, strlen(FileOut));
                        token = strtok(NULL, delim);
                        continue;
                    }
                }
            }

            args[argCnt] = (char *)malloc(sizeof(char)*(strlen(token) + 1));
            memset(args[argCnt], 0, strlen(token) + 1);
            strcpy(args[argCnt], token);
            argCnt++;
            token = strtok(NULL, delim);
        }

        free(redirectLine);

        /** Handle I/O error
         * @isDup: Duplicated error
         */
        char **tmpArgs = (char **)malloc(sizeof(char *) * MAX_CMD_LENGTH);
        for(int i = 0; i < MAX_CMD_LENGTH; i++) tmpArgs[i] = NULL;
        int tmpArgCnt = 0;
        char *tmpToken;
        tmpToken = strtok(tmpLine, delim);
        while(tmpToken != NULL){
            tmpArgs[tmpArgCnt] = (char *)malloc(sizeof(char) * strlen(tmpToken) + 1);
            memset(tmpArgs[tmpArgCnt], 0, strlen(tmpToken) + 1);
            strcpy(tmpArgs[tmpArgCnt], tmpToken);
            tmpArgCnt++;
            tmpToken = strtok(NULL, delim);
        }

        int isDup = 0;
        if(!ioError){
            for(int i = 0; i < tmpArgCnt; i++){
                if(tmpArgs[i][0] == '|') isDup = 1;
                if(tmpArgs[i][0] == '<' && isDup){
                    printError("error: duplicated input redirection\n");
                    ioError = 1;
                    break;
                }
            }
        }

        isDup = 0;
        for(int i = 0; i < tmpArgCnt; i++){
            if(ioError) break;
            if(tmpArgs[i][0] == '|' && i < tmpArgCnt-1 && tmpArgs[i+1][0] == '|'){
                printError("error: missing program\n");
                ioError = 1;
                break;
            }
            if(i==0 && tmpArgs[i][0] == '|'){
                printError("error: missing program\n");
                ioError = 1;
                break;
            }
            if(tmpArgs[i][0] == '>' && i < tmpArgCnt - 1 && tmpArgs[i+1][0] == '<'){
                printError("syntax error near unexpected token `<\'\n");
                ioError = 1;
                break;
            }
            if(tmpArgs[i][0] == '>' && i < tmpArgCnt - 1 && tmpArgs[i+1][0] == '|'){
                printError("syntax error near unexpected token `|\'\n");
                ioError = 1;
                break;
            }
            if(tmpArgs[i][0]== '>' && i < tmpArgCnt-1 && tmpArgs[i+1][0] == '>'){
                printError("syntax error near unexpected token `>\'\n");
                ioError = 1;
                break;
            }
            if(tmpArgs[i][0]=='>') isDup=1;
            if(tmpArgs[i][0]=='|' && isDup){
                printError("error: duplicated output redirection\n");
                ioError = 1;
                break;
            }
        }
        for(int i = 0; i < tmpArgCnt; i++) if(tmpArgs[i] != NULL) free(tmpArgs[i]);
        free(tmpLine);
        free(tmpArgs);
        // Exit if there is an I\O error
        if(ioError){
            for(int i = 0; i < argCnt; i++){
                if (args[i] != NULL) free(args[i]);
            }
            free(args);
            freefile();
            continue;
        }

        /** Parse pipes
         * @pipeNum: Total number of pipes
         * @cmds: Array to store each command
         * @cmdNum: Total number of commands
         */
        int pipeNum = 0;
        int cmds[MAX_PIPE];
        cmds[0] = 0;
        int cmdCnt = 1;

        for (int i = 0; i < argCnt; i++){
            if(!strcmp(args[i],"|")){
                pipeNum++;
                free(args[i]);
                args[i] = NULL;
                cmds[cmdCnt] = ++i;
                cmdCnt++;
            }
            else if(!strcmp(args[i],"&")){
                free(args[i]);
                args[i] = NULL;
            }
        }

        int cmdNum = pipeNum + 1;
        cmds[cmdNum] = argCnt + 1;

        int markIndex = 0;
        for(int i = 0; i < argCnt; i++){
            if(args[i] == NULL) continue;
            for(size_t j = 0; j < strlen(args[i]); j++){
                if(args[i][j] == REPLACE_IDENTITY && markIndex < markNum) args[i][j] = marks[markIndex++];
            }
        }

        /** Create pipes
         * @pipeFile: Array of files in pipes
         */
        int pipeFile[2 + pipeNum * 2];
        for (int i = 0; i < pipeNum; i++){
            if(pipe(pipeFile + 2 * i) == -1){
                printDebug("Pipe Failure.\n");
                for(int i = 0; i < argCnt; i++){
                    if (args[i] != NULL) free(args[i]);
                }
                free(args);
                freefile();
                for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
                if(lastDir != NULL) free(lastDir);
                exit(0);
            }
        }

        /** Execute
         * @childStatus
         * @inFd: FD for in
         * @outFd: FD for out
         */
        int childStatus;
        int inFd, outFd;

        for (int i = 0; i < cmdNum; i++){
            int cmd = cmds[i];
            int offsetArg = cmds[i+1] - cmd - 1;
            // Run exit
            if(!strcmp(args[cmd],"exit")){
                printPrompt("exit\n");
                if(lastDir != NULL) free(lastDir);
                for (int j = 0; j < argCnt; j++){
                    if(args[j] != NULL) free(args[j]);
                }
                free(args);
                freefile();

                for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
                exit(0);
            }
            // Run cd
            if(!strcmp(args[cmd],"cd")){
                fflush(NULL);
                if((offsetArg == 2 && !strcmp(args[cmd+1], "~")) || offsetArg == 1){
                    chdir(homeDir);
                    if(lastDir == NULL) lastDir = (char *)malloc(MAX_PATH * sizeof(char));
                    memset(lastDir, 0, MAX_PATH);
                    strcpy(lastDir, curDir);
                    strcpy(curDir, homeDir);
                }
                else{
                    if(!strcmp(args[cmd + 1], "-")){
                        if(lastDir == NULL) printPrompt("No last dir!\n");
                        else{
                            chdir(lastDir);
                            printPrompt(lastDir);
                            printPrompt("\n");
                        }
                    }
                    else{
                        if(chdir(args[cmd+1]) == -1){
                            printPrompt(args[cmd+1]);
                            printPrompt(": No such file or directory\n");
                        }
                        else{
                            if(lastDir == NULL) lastDir = (char *)malloc(MAX_PATH * sizeof(char));
                            memset(lastDir, 0, MAX_PATH);
                            strcpy(lastDir, curDir);
                            char pwd[MAX_PATH];
                            getcwd(pwd, sizeof(pwd));
                            strcpy(curDir,pwd);
                        }
                    }
                }
                continue;
            }
            if(!strcmp(args[cmd],"jobs")){
                for(int j = 0; j < backJobCnt; j++){
                    char tmpM[MAX_CMD_LENGTH];
                    if(waitpid(backJobStatus[2 * j], NULL, WNOHANG) == 0) sprintf(tmpM, "[%d] running %s\n", j+1, backJob[j]);
                    else sprintf(tmpM, "[%d] done %s\n", j+1, backJob[j]);
                    printPrompt(tmpM);
                }
                continue;
            }

            // Fork, referring to https://www.jianshu.com/p/9b8281fe75c5
            pid_t pid = fork();
            pids[i] = pid;

            if(pid == 0){
                // Child Process
                sigaction(SIGINT, &act, &old_act);
                status = CHILD;
                // If not the first cmd
                if(i != 0){
                    if(pipeNum > 0 && dup2(pipeFile[2 * i - 2], 0) == -1){
                        printDebug("dup2 stdin failure.\n");
                        for(int i = 0; i < argCnt; i++){
                            if (args[i] != NULL) free(args[i]);
                        }
                        free(args);
                        freefile();
                        for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
                        if(lastDir != NULL) free(lastDir);
                        exit(0);
                    }
                }
                // If not the last cmd
                if(i + 1 < cmdNum){
                    if(pipeNum > 0 && dup2(pipeFile[2 * i + 1], 1) <= 0){
                        printDebug("dup2 stdout failure.\n");
                            for(int i = 0; i < argCnt; i++){
                                if (args[i] != NULL) free(args[i]);
                            }
                            free(args);
                            freefile();
                            for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
                            if(lastDir != NULL) free(lastDir);
                            exit(0);
                    }
                }
                // Redirection
                if(inRed && i == 0){
                    inFd = open(FileIn, O_RDONLY);
                    if(inFd <= 0){
                        if(errno == ENOENT){
                            printError(FileIn);
                            printError(": No such file or directory\n");
                            exit(0);
                        }
                    }
                    // Copy stdin to inFd
                    dup2(inFd, 0);
                    close(inFd);
                }

                if(outRed && i + 1 == cmdNum){
                    outFd = open(FileOut, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                    if(outFd <= 0){
                        if(errno == EROFS || errno == EPERM){
                            printError(FileOut);
                            printError(": Permission denied\n");
                            exit(0);
                        }
                    }
                    // Copy stdout to outFd
                    dup2(outFd, 1);
                    close(outFd);
                }
                else if(outCov && i + 1 == cmdNum){
                    outFd = open(FileOut, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
                    if(outFd <= 0){
                        if(errno == EROFS || errno == EPERM){
                            printError(FileOut);
                            printError(": Permission denied\n");
                            exit(0);
                        }
                    }
                    // Copy stdout to outFd
                    dup2(outFd, 1);
                    close(outFd);
                }
                // Close pipeFile
                for (int i = 0; i < pipeNum * 2; i++) close(pipeFile[i]);
                
                // Run pwd
                if(!strcmp(args[cmd], "pwd")){
                    char pwd[MAX_PATH];
                    if(getcwd(pwd, sizeof(pwd)) != NULL){
                        printPrompt(pwd);
                        printPrompt("\n");
                    }
                    else printDebug("pwd error.\n");
                    exit(0);
                }
                
                // Run bash command under /usr/bin
                if(execvp(args[cmd], args+cmd) < 0){
                    printError(args[cmd]);
                    printError(": command not found\n");
                }
                // If execvp fails, exit; if execvp succeeds, omit
                exit(0);
            }
            else if(pid > 0 && isBack == 1 && i == 0){
                backJobStatus[backJobCnt * 2] = pid;
                backJobStatus[backJobCnt * 2 + 1] = RUNNNING;
                backJobCnt++;
                char tmpM[MAX_CMD_LENGTH];
                sprintf(tmpM, "[%d] %s\n", backJobCnt, backJob[backJobCnt - 1]);
                printPrompt(tmpM);
            }
            else if(pid < 0){
                // Fork error and exit
                printDebug("Error: fork failed.\n");
                for(int i = 0; i < argCnt; i++){
                    if (args[i] != NULL) free(args[i]);
                }
                free(args);
                freefile();
                for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
                if(lastDir != NULL) free(lastDir);
                exit(0);
            }
            else{} // Parent and do nothing 
        }

        // Close pipes(parent)
        for (int i = 0; i < 2 * pipeNum; i++) close(pipeFile[i]);
        
        // if not run in the background
        if(!isBack) for(int i = 0; i < pipeNum + 1; i++) waitpid(pids[i], NULL, WUNTRACED);
        else if(isBack) waitpid(backJobStatus[2 * (backJobCnt - 1)], &childStatus, WNOHANG);
        
        // Clean args
        for(int i = 0; i < argCnt; i++){
            if (args[i] != NULL) free(args[i]);
        }
        free(args);
        freefile();
    }

    for (int j = 0; j < backJobCnt; j++) free(backJob[j]);
    if(lastDir != NULL) free(lastDir);
    return 0;
}
