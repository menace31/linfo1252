#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "verrou.h"

// Initialisation
#define nReadTot 2560 // nbre lectures
#define nWriteTot 640 // nbre lectures
int mutex;
int dbMutex;
semaphore_t db; // accès à la db
int readcount=0; // nombre de readers
int ecritures=0;
int lectures=0;

// Ecrivain
void *writer(void *argv)
{
  int nWriteFrac = *((int *)argv);
  for(int j=0;j<nWriteFrac;j++)
  {
    wait(&db);
    ////// SECTION CRITIQUE ////////
    for (int i=0; i<10000; i++);
    ///////////////////////////////
    post(&db);
  }
}

// Lecteur
void *reader(void *argv)
{
  int nReadFrac = *((int *)argv);
  for(int j=0;j<nReadFrac;j++)
  {
    test_and_test_and_set(&mutex);
    // section critique
    readcount++;
    if (readcount==1)
    { // arrivée du premier reader
      wait(&db);
    }
    unlock(&mutex);
    ////// SECTION CRITIQUE ////////
    for (int i=0; i<10000; i++);
    ///////////////////////////////
    test_and_test_and_set(&mutex);
    // section critique
    readcount--;
    if(readcount==0)
    { // départ du dernier reader
      post(&db);
    }
    unlock(&mutex);
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
  mutex = 0;
  db.nombre = 1;
  db.mutex = &dbMutex;

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

  return(EXIT_SUCCESS);
}
