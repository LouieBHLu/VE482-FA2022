#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000000
int count = 0;
sem_t sem;

void *thread_count(void *x){
    int tmp;
    for (int i = 0; i < N; i++){
        sem_wait(&sem);
        tmp = count;
        tmp++;
        count = tmp;
        sem_post(&sem);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t *t = malloc(sizeof(pthread_t));
    sem_init(&sem, 0, 1);

    for (size_t i = 0; i < 2; i++){
        if(pthread_create(t + i, NULL, thread_count, NULL)){
            fprintf(stderr,"ERROR creating thread %zu\n", i);
            exit(1);
        }
    }

    for (size_t i = 0; i < 2; i++){
        if(pthread_join(*(t + i), NULL)){
            fprintf(stderr,"ERROR joining thread\n");
            exit(1);
        }
    }

    if(count < 2 * N) printf("Count is %d, but should be %d\n", count, 2 * N);
    else printf("Count is [%d]\n", count);

    pthread_exit(NULL);
    free(t);
    sem_destroy(&sem);
    
    return 0;
}
