#include <error.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

#define NSECCRIT 6400
int mutex;

void *fun(void *arg)
{
  int nPassages = *((int *)arg);
  for(int j=0;j<nPassages;j++)
  {
    test_and_set(&mutex);
    for (int i=0; i<10000; i++);
    unlock(&mutex);
  }
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
  int first = NSECCRIT / N;               // nombre d'opérations par thread
  int last = first + NSECCRIT;
  pthread_t thread[N];

  // Création des threads
  for(int i=0;i<N;i++)
  {
    if (i != N - 1){
      if(pthread_create(&(thread[i]),NULL,&fun,(void *)&first)!=0)
      return EXIT_FAILURE;
    }
    else{
      if(pthread_create(&(thread[i]),NULL,&fun,(void *)&last)!=0)
      return EXIT_FAILURE;
    }
  }

  // Rejoignement des threads
  for (int i = 0; i < N; i++)
  {
    if(pthread_join(thread[i], NULL) != 0)
      return EXIT_FAILURE;
  }

  return(EXIT_SUCCESS);
}
