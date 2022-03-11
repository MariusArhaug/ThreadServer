#include "bbuffer.h"
#include "sem.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BNDBUF {
  SEM** sem_arr;
  size_t i_size;
  size_t t_size;
  uint32_t count;
  uint32_t capacity;
  int* buffer;
  
  int head;
  int tail;

};


BNDBUF* bb_init(uint32_t capacity) {
  BNDBUF* bb = malloc(sizeof(struct BNDBUF));
  memset(bb, 0, sizeof(struct BNDBUF));

  bb->sem_arr = malloc(capacity*sizeof(SEM*));

  for (int i = 0; i < capacity; i++) {
    bb->sem_arr[i] = sem_init(0);
  } 

  bb->capacity = capacity;
  bb->count = 0;
  bb->i_size = sizeof(int);
  bb->t_size = bb->i_size * bb->capacity;
  bb->buffer = malloc(bb->t_size);
  memset(bb->buffer, 0, bb->t_size);
  bb->head = 0;
  bb->tail = 0;
  return bb;
}

void bb_del(BNDBUF* bb) {
  free(bb->buffer);
  free(bb);
}

int bb_get(BNDBUF *bb) {
  if (bb->count == 0) {
      
  } 

  int fd = bb->buffer[bb->head++];
  bb->head %= bb->capacity;
  return fd;
}


void bb_add(BNDBUF *bb, int fd) {
  if (bb->count == bb->capacity) {
    //block;
  }

  bb->buffer[bb->tail++] = fd;
  bb->tail %= bb->capacity;  
}

