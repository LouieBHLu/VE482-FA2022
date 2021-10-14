#ifndef UTIL_H
#define UTIL_H

/* Macro */
#define PARENT 1
#define PARENT_EXIT 2
#define CHILD 3

#define RUNNNING 1
#define FINISH 2

#define MAX_JOB 100             // Max of Job running
#define MAX_PATH 100            // Max path length
#define MAX_CMD_LENGTH 1024     // Max command length
#define MAX_FILE 100            // Max file name length
#define MAX_PIPE 400            // Max pipe number

#define REPLACE_IDENTITY 17     // Device Control 4

/* Global Variable */
struct sigaction old_act, act;

char* line;                     // Input line
char* comLine;                  // Recombined Input line 
char* FileIn;                   // Input file
char* FileOut;                  // Output file
char* backJob[MAX_JOB];         // Array of background command
char* lastDir;                  // last directory
char* curDir;                   // current directory
const char* homeDir;            // $HOME directory
    
int backJobCnt;                 // Number of Job working in the background; start from 1
int isBack;                     // if background of not
int FileStdin;                  // Stdin file
int FileStdout;                 // Stdout file
int status;                     // Status of current process
int pids[MAX_PIPE];             // Array of pids
int backJobStatus[MAX_JOB * 2]; // Array of status of background job

/* Functions */
void printDebug(char *m);
void printPrompt(char *m);
void printError(char *m);
void init();
void freefile();
void _ls(const char *dir,int op_a);
void CTRLCHandle();
void errorClose(char *m, char ** args, const int argCnt);
void replaceMark(const char mark, unsigned int i, char* marks, int* markNum);

#endif
