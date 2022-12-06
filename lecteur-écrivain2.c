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
    test_and_test(&mutex);
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
  int err;
  err = pthread_mutex_init(&mutex, NULL);
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_init");
  err = sem_init(&db, 0, 1); // buffer vide
  if(err!=0)
    error(EXIT_FAILURE,err,"sem_init");

  // Création des threads
  for(int i=0;i<nE;i++)
  {
    err=pthread_create(&(ecrivain[i]),NULL,writer,NULL);
    if(err!=0)
      error(EXIT_FAILURE, err,"pthread_create");
  }
  for(int i=0;i<nL;i++)
  {
    err=pthread_create(&(lecteur[i]),NULL,reader,NULL);
    if(err!=0)
      error(EXIT_FAILURE, err,"pthread_create");
  }

  // Rejoignement des threads
  for (int i = 0; i < nE; i++)
  {
    err = pthread_join(ecrivain[i], NULL);
    if (err != 0)
      error(EXIT_FAILURE, err, "pthread_join");
  }
  for (int i = 0; i < nL; i++)
  {
    err = pthread_join(lecteur[i], NULL);
    if (err != 0)
      error(EXIT_FAILURE, err, "pthread_join");
  }

  // Destruction du mutex + semaphores
  err = pthread_mutex_destroy(&(mutex));
  if (err != 0)
    error(EXIT_FAILURE, err, "pthread_mutex_destroy");
  sem_destroy(&db);
  if(err!=0)
    error(EXIT_FAILURE, err,"sem_destroy");

  return(EXIT_SUCCESS);
}
