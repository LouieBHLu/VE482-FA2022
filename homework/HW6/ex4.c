// Assume that we're using a computer with physical memory of 8 GB, and some programs have already taken 2GB memory.
#include <stdio.h>
#include <stdlib.h>

int *d1; // 1,073,741,824 * 4 = 4GB
int *d2; // 1,073,741,824 * 4 = 4GB
int done1;
int done2;

void initBit(int *d1, int *d2){
  d1 = (int *)malloc(sizeof(int)*1073741824);
  printf("d1 init ok\n");
  // d2 execeeds physical memory, thrashing happens
  d2 = (int *)malloc(sizeof(int)*1073741824);
  printf("d2 init ok\n");
  for(int i = 0;i < 1073741824; i++) (d1)[i]=i;
  printf("d1 calc ok\n");
  // d2 execeeds physical memory, thrashing happens
  for(int i = 0;i < 1073741824; i++) (d2)[i]=i;
  printf("d2 calc ok\n");
}

int main(){
  done1 = done2 = 0;
  initBit(d1, d2);
  free(d1);
  free(d2);
  return 0;
}