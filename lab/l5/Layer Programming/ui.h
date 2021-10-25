#ifndef UI_H
#define UI_H

#include "list.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_KEY 1024
#define MAX_STRING 1024

void readType(type* t, char* argv[]);

void insertNode(llist* l, char* k, void* d);

void writeFile(llist*l, type* t);

void cmd(type* t, char* argv[]);

void menu();

#endif