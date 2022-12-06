#include <error.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

int N;
pthread_mutex_t *baguette;

void *philosophe(void *arg)
{
    int *id = (int *)arg;
    int left = *id;
    int right = (left + 1) % N;
    int err;
    for (int i = 0; i < 100000; i++)
    {
        // philosophe pense
        if (left < right)
        {
            err = test_and_test(baguette + left);
            if (err != 0)
            {
                error(EXIT_FAILURE, err, "pthread_mutex_lock");
            }
            err = test_and_test(baguette + right);
            if (err != 0)
            {
                error(EXIT_FAILURE, err, "pthread_mutex_lock");
            }
        }
        else
        {
            err = test_and_test(baguette + right);
            if (err != 0)
            {
                error(EXIT_FAILURE, err, "pthread_mutex_lock");
            }
            err = test_and_test(baguette + left);
            if (err != 0)
            {
                error(EXIT_FAILURE, err, "pthread_mutex_lock");
            }
        }
        //mange
        err = unlock(&baguette[left]);
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_mutex_unlock");
        err = unlock(&baguette[right]);
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_mutex_unlock");
    }
    return (NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "ERROR\tLe nombre d'arguments n'est pas valide\n");
        return EXIT_FAILURE;
    }
    N = atoi(argv[1]);
    pthread_t phil[N];
    int err;
    int num[N];
    baguette = malloc(sizeof(pthread_mutex_t) * N);
    //--- Création des mutex ----------------------------
    for (int i = 0; i < N; i++)
    {
        err = pthread_mutex_init(&(baguette[i]), NULL);
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_mutex_init");
        num[i] = i;
    }
    //--- Création et lancement des threads -------------
    for (int i = 0; i < N; i++)
    {
        err = pthread_create(&(phil[i]), NULL, &philosophe, (void *)&(num[i]));
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_create");
    }
    //--- Rejoignement des threads -----------------------
    for (int i = 0; i < N; i++)
    {
        err = pthread_join(phil[i], NULL);
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_join");
    }
    //--- Destruction des mutex --------------------------
    for (int i = 0; i < N; i++)
    {
        err = pthread_mutex_destroy(&(baguette[i]));
        if (err != 0)
            error(EXIT_FAILURE, err, "pthread_mutex_destroy");
    }

    free(baguette);
    return EXIT_SUCCESS;
}
