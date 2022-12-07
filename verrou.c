#include <stdio.h>
#include <stdlib.h>
#include "verrou.h"

int test_and_set(int *lock){
    int i = 1;
    asm("xchgl %1, %0"
        : "+m"(*lock), "+r"(i));
    return i;
}

void test_and_test_and_set(int *lock){
    while (test_and_set(lock)){
        while (*lock){NULL;}
    }
}

void lock(int *lock){
    while (test_and_set(lock)){NULL;}
}

void unlock(int *lock){
    asm("movl $0, %%eax\n"
        "xchgl %%eax, %0"
        : "+m"(*lock)
        :
        : "%eax");
}

void wait(semaphore_t *mut){
    while (1)
    {
        test_and_test_and_set(mut->mutex);
        if (mut->nombre <= 0)
        {
            unlock(mut->mutex);
        }
        else{
            mut->nombre-=1;
            unlock(mut->mutex);
            break;
        }
    }
}

void post(semaphore_t *sem)
{
    test_and_test_and_set(sem->mutex);
    sem->nombre+=1;
    unlock(sem->mutex);
}
