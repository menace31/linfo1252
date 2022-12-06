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
    test_and_test_and_set(&mutex);
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
  if(pthread_mutex_init(&mutex, NULL) != 0)
    return EXIT_FAILURE;

  // Création des threads
  for(int i=0;i<nProd;i++)
  {
    if(pthread_create(&(thread[i]),NULL,fun,NULL)!=0)
      return EXIT_FAILURE;
  }

  // Rejoignement des threads
  for (int i = 0; i < N; i++)
  {
    if(pthread_join(thread[i], NULL) != 0)
      return EXIT_FAILURE;
  }

  // Destruction du mutex
  if (pthread_mutex_destroy(&(mutex)) != 0)
    return EXIT_FAILURE;
  sem_destroy(&empty);

  return(EXIT_SUCCESS);
}
