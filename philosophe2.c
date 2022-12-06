#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "verrou.h"

pthread_mutex_t* baguette;

int N;

void* mange() {
    printf("Philosophe mange\n");
    NULL;
}
void* philosophe ( void* arg )
{
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % N;
    for(int i = 0; i<10000; i++) {
        // philosophe pense
        if (left < right)
        {
            test_and_test_and_set(&baguette[left]);
            test_and_test_and_set(&baguette[right]);
        }
        else
        {
            test_and_test_and_set(&baguette[right]);
            test_and_test_and_set(&baguette[left]);
        }
        mange();
        unlock(&baguette[left]);
        unlock(&baguette[right]);
    }
}

int main (int argc, char *argv[]){
    if(argc < 2){
        return EXIT_FAILURE;
    }
    N = atoi(argv[1]);
    int id[N];
    pthread_t phil[N];
    baguette = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * N);

    //Creat mutex
    for(int i = 0; i < N; ++i){
        id[i]=i;
        if(pthread_mutex_init(&baguette[i],NULL) != 0){
            return EXIT_FAILURE;
        }
    }
    //creat thread
    for (int i = 0; i < N; ++i) {
        if(pthread_create(&phil[i], NULL, philosophe, (void *)&(id[i]))!= 0){
            return EXIT_FAILURE;
        }

    }
    //join thread
    for (int i = 0; i < N; ++i) {
        if(pthread_join(phil[i], NULL)!= 0){
            return EXIT_FAILURE;
        }
    }
    //destroy mutex
    for(int i = 0; i< N; ++i){
        if(pthread_mutex_destroy(&(baguette[i]))!=0){
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
