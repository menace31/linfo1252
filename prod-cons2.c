#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

// Initialisation
#define N 8 // places dans le buffer
#define N_ELEMENTS 8192 //
semaphore_t empty;
semaphore_t full;
int mutex;
int fullMutex;
int emptyMutex;
int *buffer;
int filling=0;

// Producteur
void *producer(void *argv)
{
  int nprodFrac = *((int *)argv);
  int item;
  for(int j=0;j<nprodFrac;j++)
  {
    for (int i=0; i<10000; i++); // simulation traitement
    item=rand();
    wait(&empty);
    test_and_test_and_set(&mutex);
    /////// SECTION CRITIQUE ///////
    buffer[filling] = item;
    filling++;
    ////////////////////////
    unlock(&mutex);
    post(&full);
  }
}

// Consommateur
void *consumer(void *argv)
{
  int nconsFrac = *((int *)argv);
  for(int j=0;j<nconsFrac;j++)
  {
    wait(&full);
    test_and_test_and_set(&mutex);
    //////// SECTION CRITIQUE ////////
    buffer[filling-1]=0;
    filling--;
    ////////////////////////
    unlock(&mutex);
    post(&empty);
    for (int i=0; i<10000; i++); // simulation traitement
  }
}

//------------------------------------------------------------------

int main(int argc, char *argv[])
{
  if (argc < 3)
    {
        fprintf(stderr, "ERROR\tLe nombre d'arguments n'est pas valide\n");
        return EXIT_FAILURE;
    }
  int nProd = atoi(argv[1]);
  int nCons = atoi(argv[2]);
  pthread_t prod[nProd];
  pthread_t cons[nCons];
  buffer = malloc(sizeof(int) * N);

  // Création du mutex + semaphores
  mutex = 0;
  empty.nombre = N;
  empty.mutex = &emptyMutex;
  full.nombre = 0;
  full.mutex = &fullMutex;

  // Création des threads
  int nLoop1Prod = (N_ELEMENTS / nProd) + (N_ELEMENTS % nProd); // 1er threads
  int nLoop1Cons = (N_ELEMENTS / nCons) + (N_ELEMENTS % nCons);
  if(pthread_create(&(prod[0]),NULL,producer,(void *)&nLoop1Prod) != 0)
    return EXIT_FAILURE;
  if(pthread_create(&(cons[0]),NULL,consumer,(void *)&nLoop1Cons) != 0)
    return EXIT_FAILURE;
  int nLoopsProd = N_ELEMENTS / nProd; // threads suivants
  int nLoopsCons = N_ELEMENTS / nCons;
  for(int i=1;i<nProd;i++)
  {
    if(pthread_create(&(prod[i]),NULL,producer,(void *)&nLoopsProd) != 0)
      return EXIT_FAILURE;
  }
  for(int i=1;i<nCons;i++)
  {
    if(pthread_create(&(cons[i]),NULL,consumer,(void *)&nLoop1Cons) != 0)
      return EXIT_FAILURE;
  }

  // Rejoignement des threads
  for (int i = 0; i < nProd; i++)
  {
    if(pthread_join(prod[i], NULL) != 0)
      return EXIT_FAILURE;
  }
  for (int i = 0; i < nCons; i++)
  {
    if(pthread_join(cons[i], NULL) != 0)
      return EXIT_FAILURE;
  }

  free(buffer);

  return(EXIT_SUCCESS);
}
