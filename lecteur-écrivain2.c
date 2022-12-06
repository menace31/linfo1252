#include <error.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

// Initialisation
#define nRead 2560 // nbre lectures
#define nWrite 640 // nbre lectures
pthread_mutex_t mutex;
sem_t db; // accès à la db
int readcount=0; // nombre de readers

// Ecrivain
void *writer(void *argv)
{
  while(true)
  {
    wait(&db);
    // section critique, un seul writer à la fois
    for (int i=0; i<10000; i++);
    post(&db);
  }
}

// Lecteur
void *reader(void *argv)
{
  while(true)
  {
    test_and_test_and_set(&mutex);
    // section critique
    readcount++;
    if (readcount==1)
    { // arrivée du premier reader
      wait(&db);
    }
    pthread_mutex_unlock(&mutex);
    for (int i=0; i<10000; i++);
    pthread_mutex_lock(&mutex);
    // section critique
    readcount--;
    if(readcount==0)
    { // départ du dernier reader
      sem_post(&db);
    }
    pthread_mutex_unlock(&mutex);
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
  int nE = atoi(argv[1]);
  int nL = atoi(argv[2]);
  pthread_t ecrivain[nE];
  pthread_t lecteur[nL];

  // Création du mutex + semaphore
  if(pthread_mutex_init(&mutex, NULL) != 0)
    return EXIT_FAILURE;
  if(sem_init(&db, 0, 1) != 0)
    return EXIT_FAILURE;

  // Création des threads
  for(int i=0;i<nE;i++)
  {
    if(pthread_create(&(ecrivain[i]),NULL,writer,NULL) != 0)
      return EXIT_FAILURE;
  }
  for(int i=0;i<nL;i++)
  {
    if(pthread_create(&(lecteur[i]),NULL,reader,NULL) != 0)
      return EXIT_FAILURE;
  }

  // Rejoignement des threads
  for (int i = 0; i < nE; i++)
  {
    if (pthread_join(ecrivain[i], NULL) != 0)
      return EXIT_FAILURE;
  }
  for (int i = 0; i < nL; i++)
  {
    if (pthread_join(lecteur[i], NULL) != 0)
      return EXIT_FAILURE;
  }

  // Destruction du mutex + semaphores
  if (pthread_mutex_destroy(&(mutex)) != 0)
    return EXIT_FAILURE;
  if(sem_destroy(&db) != 0)
    return EXIT_FAILURE;

  return(EXIT_SUCCESS);
}
