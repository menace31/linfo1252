#ifndef _VERROU_H
#define _VERROU_H

typedef struct semaphore_t
{
    int nombre;
    int *mutex;
} semaphore_t;

void lock(int *lock);
void unlock(int *lock);

int test_and_set(int *lock);
void test_and_test(int *lock);

void wait(semaphore_t *sem);
void post(semaphore_t *sem);

#endif