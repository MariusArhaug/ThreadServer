#include "bbuffer.h"
#include "sem.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct BNDBUF {
  SEM* sem_full;
  SEM* sem_empty;
  uint32_t count;
  uint32_t capacity;
  int* buffer;
  
  int head;
  int tail;
};

BNDBUF* bb_init(uint32_t capacity) {
  BNDBUF* bb = malloc(sizeof(struct BNDBUF));
  memset(bb, 0, sizeof(struct BNDBUF));
  bb->sem_empty = sem_init(0);
  bb->sem_full = sem_init(0);
  bb->capacity = capacity;

  bb->buffer = malloc(bb->capacity*sizeof(int));
  if (bb->buffer == NULL) {
    free(bb);
    return NULL;
  }
  memset(bb->buffer, 0, bb->capacity*sizeof(int));
  
  return bb;
}

void bb_del(BNDBUF* bb) {
  free(bb->buffer);
  sem_del(bb->sem_empty);
  sem_del(bb->sem_full);
  free(bb);
}

int bb_get(BNDBUF *bb) {
  if (bb->count == 0) {
    V(bb->sem_empty);
  } 

  int fd = bb->buffer[bb->head++];
  bb->head %= bb->capacity;
  bb->count--;
  P(bb->sem_full);
  return fd;
}

void bb_add(BNDBUF *bb, int fd) {
  if (bb->count == bb->capacity) {
    V(bb->sem_full);
  }
  bb->buffer[bb->tail++] = fd;
  bb->tail %= bb->capacity;  
  bb->count++;
  P(bb->sem_empty);
}

