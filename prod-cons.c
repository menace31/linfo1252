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
  if (pthread_mutex_init(&mutex, NULL) != 0) // buffer rempli
    return EXIT_FAILURE;
  if(sem_init(&empty, 0 , N) != 0)
    return EXIT_FAILURE;
  if(sem_init(&full, 0 , 0) != 0) // buffer vide
    return EXIT_FAILURE;

  // Création des threads
  for(int i=0;i<nProd;i++)
  {
    if(pthread_create(&(prod[i]),NULL,producer,NULL) != 0)
      return EXIT_FAILURE;
  }
  for(int i=0;i<nCons;i++)
  {
    if(pthread_create(&(cons[i]),NULL,consumer,NULL) != 0)
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

  // Destruction du mutex + semaphores
  if (pthread_mutex_destroy(&(mutex)) != 0)
    return EXIT_FAILURE;
  if(sem_destroy(&empty) != 0)
    return EXIT_FAILURE;
  if(sem_destroy(&full) !=0)
    return EXIT_FAILURE; 

  free(buffer);
  free(presence);

  return(EXIT_SUCCESS);
}
