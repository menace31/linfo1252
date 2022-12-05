#include <error.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Initialisation
#define N 8 // places dans le buffer
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int *buffer;
int *presence;

// Producteur
void *producer(void *argv)
{
  int item;
  int changed = 0;
  for(int j=0;j<8192;j++)
  {
    for (int i=0; i<10000; i++); // simulation traitement
    item=rand();
    sem_wait(&empty); // attente d’une place libre
    pthread_mutex_lock(&mutex);
    // section critique
    for(int i;i<N;i++)
    {
      if(presence[i]==0 && changed==0)
      {
        buffer[i] = item;
        presence[i] = 1;
        changed = 1;
      }
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a une place remplie en plus
  }
  return (NULL);
}

// Consommateur
void *consumer(void *argv)
{
  int item;
  int changed = 0;
  for(int j=0;j<8192;j++)
  {
    sem_wait(&full); // attente d’une place remplie
    pthread_mutex_lock(&mutex);
    // section critique
    for(int i;i<N;i++)
    {
      if(presence[i]==1 && changed==0)
      {
        presence[i] = 0;
        changed = 1;
      }
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty); // il y a une place libre en plus
    for (int i=0; i<10000; i++); // simulation traitement
  }
  return (NULL);
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
  presence = calloc(N,sizeof(int));

  // Création du mutex + semaphores
  int err;
  err = pthread_mutex_init(&mutex, NULL);
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_init");
  err = sem_init(&empty, 0 , N); // buffer vide
  if(err!=0)
    error(EXIT_FAILURE,err,"sem_init");
  sem_init(&full, 0 , 0); // buffer vide
  if(err!=0)
    error(EXIT_FAILURE,err,"sem_init");

  // Création des threads
  for(int i=0;i<nProd;i++)
  {
    err=pthread_create(&(prod[i]),NULL,producer,NULL);
    if(err!=0)
      error(EXIT_FAILURE, err,"pthread_create");
  }
  for(int i=0;i<nCons;i++)
  {
    err=pthread_create(&(cons[i]),NULL,consumer,NULL);
    if(err!=0)
      error(EXIT_FAILURE, err,"pthread_create");
  }

  // Rejoignement des threads
  for (int i = 0; i < nProd; i++)
  {
    err = pthread_join(prod[i], NULL);
    if (err != 0)
      error(EXIT_FAILURE, err, "pthread_join");
  }
  for (int i = 0; i < nCons; i++)
  {
    err = pthread_join(cons[i], NULL);
    if (err != 0)
      error(EXIT_FAILURE, err, "pthread_join");
  }

  // Destruction du mutex + semaphores
  err = pthread_mutex_destroy(&(mutex));
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_destroy");
  sem_destroy(&empty);
  if(err!=0)
    error(EXIT_FAILURE, err,"sem_destroy");
  sem_destroy(&full);
  if(err!=0)
    error(EXIT_FAILURE, err,"sem_destroy");

  free(buffer);
  free(presence);
  return(EXIT_SUCCESS);
}
