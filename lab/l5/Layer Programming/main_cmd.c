#include "ui.h"

int main(int argc, char *argv[]){
    srand(time(NULL));  

    if(argc != 3){
        printf("Error: wrong argument number!\n");
        return -1;
    }
    type t;
    readType(&t, argv);
    cmd(&t, argv);

    return 0;
}
