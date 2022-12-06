#include <error.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "test-and-set.h"

#define NSECCRIT 6400

int nPassages;

void *fun(void *argv)
{
  for(int j=0;j<nPassages;j++)
  {
    lock(&mutex);
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

  // Création du verrou ?????

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

  // Destruction du verrou ??????

  return(EXIT_SUCCESS);
}
