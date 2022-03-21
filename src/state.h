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
  struct thread_arg_t** thread_args;
  volatile bool run;
  bool is_multihreaded;
};

extern struct State state; // global state

struct thread_arg_t {
  BNDBUF* buffer;
  int thread_no;
}; // args to send to pthread_create thread handler

#endif 