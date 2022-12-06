#include <error.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

#define NSECCRIT 6400
pthread_mutex_t mutex;
int nPassages;

void *fun(void *argv)
{
  for(int j=0;j<nPassages;j++)
  {
    test_and_test(&mutex);
    for (int i=0; i<10000; i++);
    unlock(&mutex);
  }
  return (NULL);
}

//------------------------------------------------------------------

int main(int argc, char *argv[])
{
  if (argc < 2)
    {
        fprintf(stderr, "ERROR\tLe nombre d'arguments n'est pas valide\n");
        return EXIT_FAILURE;
    }
  int N = atoi(argv[1]);
  pthread_t thread;
  int err;

  // Création du mutex
  err = pthread_mutex_init(&mutex, NULL);
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_init");

  // Création des threads
  for(int i=0;i<nProd;i++)
  {
    err=pthread_create(&(thread[i]),NULL,fun,NULL);
    if(err!=0)
      error(EXIT_FAILURE, err,"pthread_create");
  }

  // Rejoignement des threads
  for (int i = 0; i < N; i++)
  {
    err = pthread_join(thread[i], NULL);
    if (err != 0)
      error(EXIT_FAILURE, err, "pthread_join");
  }

  // Destruction du mutex
  err = pthread_mutex_destroy(&(mutex));
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_destroy");
  sem_destroy(&empty);

  return(EXIT_SUCCESS);
}
