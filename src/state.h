#ifndef STATE_H
#define STATE_H

#include "server/server.h"
#define BUFF_L 1024

struct State {
  struct server_t server; 
  int port;
  char wd[BUFF_L];
  int n_threads;
  int n_bufferslots;
};

// global state
extern struct State state;


#endif 