#include "sem.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

struct SEM {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  volatile uint32_t n;
};

SEM* sem_init(uint32_t initVal) {
  SEM* sem = malloc(sizeof(struct SEM));
  if (sem == NULL)
    return NULL;

  memset(sem, 0, sizeof(struct SEM));
  if (pthread_mutex_init(&sem->mutex, NULL) != 0) {
    free(sem);
    return NULL;
  }
  if (pthread_cond_init(&sem->cond, NULL) != 0) {
    pthread_mutex_destroy(&sem->mutex);
    free(sem);
    return NULL;
  }

  sem->n = initVal;
  return sem;
}

int sem_del(SEM* sem) {
  if(pthread_cond_destroy(&sem->cond) != 0) {
    free(sem);
    return -1;
  }
  if(pthread_mutex_destroy(&sem->mutex) != 0) {
    free(sem);
    return -1;
  }
  
  free(sem);
  return 0;
}

void P(SEM* sem) {
  pthread_mutex_lock(&sem->mutex);
  sem->n++;
  pthread_mutex_unlock(&sem->mutex);
  pthread_cond_signal(&sem->cond);
}

void V(SEM* sem) {
  pthread_mutex_lock(&sem->mutex);
  while (sem->n == 0)
    pthread_cond_wait(&sem->cond, &sem->mutex);
  sem->n--;
  pthread_mutex_unlock(&sem->mutex);
}