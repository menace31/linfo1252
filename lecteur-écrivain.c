#include <error.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Initialisation
#define nReadTot 2560 // nbre lectures
#define nWriteTot 640 // nbre lectures
pthread_mutex_t mutex;
sem_t db; // accès à la db
int readcount=0; // nombre de readers

// Ecrivain
void *writer(void *argv)
{
  int nWriteFrac = *((int *)argv);
  for(int j=0;j<nWriteFrac;j++)
  {
    sem_wait(&db);
    // section critique,
    for (int i=0; i<10000; i++);
    sem_post(&db);
  }
}

// Lecteur
void *reader(void *argv)
{
  int nReadFrac = *((int *)argv);
  for(int j=0;j<nReadFrac;j++)
  {
    pthread_mutex_lock(&mutex);
    // section critique
    readcount++;
    if (readcount==1)
    { // arrivée du premier reader
      sem_wait(&db);
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
  int nThreadE = atoi(argv[1]);
  int nThreadL = atoi(argv[2]);
  pthread_t ecrivain[nThreadE];
  pthread_t lecteur[nThreadL];

  // Création du mutex + semaphore
  if(pthread_mutex_init(&mutex, NULL) != 0)
    return EXIT_FAILURE;
  if(sem_init(&db, 0, 1) != 0)
    return EXIT_FAILURE;

  // Création des threads
  int nLoop1E = (nWriteTot / nThreadE) + (nWriteTot % nThreadE); // 1er thread
  int nLoop1L = (nReadTot / nThreadL) + (nReadTot % nThreadL);
  if(pthread_create(&(ecrivain[0]),NULL,writer,(void *)&nLoop1E) != 0)
    return EXIT_FAILURE;
  if(pthread_create(&(lecteur[0]),NULL,reader,(void *)&nLoop1L) != 0)
    return EXIT_FAILURE;

  int nLoopsE = nWriteTot / nThreadE; // threads suivants
  int nLoopsL = nReadTot / nThreadL;
  for(int i=1;i<nThreadE;i++)
  {
    if(pthread_create(&(ecrivain[i]),NULL,writer,(void *)&nLoopsE) != 0)
      return EXIT_FAILURE;
  }
  for(int i=1;i<nThreadL;i++)
  {
    if(pthread_create(&(lecteur[i]),NULL,reader,(void *)&nLoopsL) != 0)
      return EXIT_FAILURE;
  }

  // Rejoignement des threads
  for (int i = 0; i < nThreadE; i++)
  {
    if (pthread_join(ecrivain[i], NULL) != 0)
      return EXIT_FAILURE;
  }
  for (int i = 0; i < nThreadL; i++)
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
