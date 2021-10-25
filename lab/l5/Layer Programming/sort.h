#ifndef SORT_H
#define SORT_H

#include "list.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


#define INT_TYPE 0
#define CHAR_TYPE 1
#define DOUBLE_TYPE 2
#define RAND_SORT 2
#define INC_SORT 0
#define DEC_SORT 1
#define NULL_TYPE -1

typedef struct type{
    int sortingType;
    int dataType;
} type;

void sortNodes(llist* l, type* t);

#endif