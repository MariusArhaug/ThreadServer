#ifndef STATE_H
#define STATE_H

#include "server/server.h"
#include "types/bbuffer.h"

#include <pthread.h>
#define WD_L 1024

struct State {
  struct server_t server; 
  int port;
  char wd[WD_L];
  int n_threads;
  int n_bufferslots;
  pthread_t *thread_pool;
  BNDBUF* buffer;
  
};

extern struct State state; // global state


#endif 