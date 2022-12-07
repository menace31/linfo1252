#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "verrou.h"

int N;
int* baguette;

void* mange() {
    NULL;
}
void* philosophe ( void* arg )
{
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % N;
    for(int i = 0; i<1000000; i++) {
        // philosophe pense
        if (left < right)
        {
            test_and_test_and_set(left +baguette);
            test_and_test_and_set(right +baguette);
        }
        else
        {
            test_and_test_and_set(right +baguette);
            test_and_test_and_set(left +baguette);
        }
        mange();
        unlock(left +baguette);
        unlock(right +baguette);
    }
}

int main (int argc, char *argv[]){
    if(argc < 2){
        return EXIT_FAILURE;
    }
    N = atoi(argv[1]);
    int id[N];
    pthread_t phil[N];
    baguette = malloc(sizeof(int) * N);

    //Creat mutex
    for(int i = 0; i < N; ++i){
        id[i]=i;
    }
    //creat thread
    for (int i = 0; i < N; ++i) {
        if(pthread_create(&phil[i], NULL, &philosophe, (void *)&(id[i]))!= 0){
            return EXIT_FAILURE;
        }

    }
    //join thread
    for (int i = 0; i < N; ++i) {
        if(pthread_join(phil[i], NULL)!= 0){
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
