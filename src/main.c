#include "state.h"
#include "server/server.h"
#include "types/error.h"
#include "types/bbuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

// global state
struct State state;


void init() {
  state.buffer = bb_init(state.n_bufferslots); 
  state.thread_pool = (pthread_t*) malloc(state.n_threads * sizeof(pthread_t));
  for (int i = 0; i < state.n_threads; i++) 
    pthread_create(&state.thread_pool[i], NULL, handle_thread, state.buffer);

  server_init(
    &state.server
  );
  server_start(&state.server); 
}

void destroy() {
  free(state.thread_pool);
  server_destroy(&state.server);
}


static inline void check_args(int argc, char* argv[]) {
  if (argc != 5)
    ERROR("Usage: ./mtwwd [www-path] [port] [#threads] [#bufferslots]"); 
  
  strcpy(state.wd, argv[1]);

  errno = 0;

  state.port = strtol(argv[2], NULL, 10);
  state.n_threads = strtol(argv[3], NULL, 10);
  state.n_bufferslots = strtol(argv[4], NULL, 10);

  if (errno == ERANGE)
    ERROR("Number port, n_threads or n_bufferslot is not a valid number (parsing error)");
}


int main(int argc, char* argv[]) {
  check_args(argc, argv); 
  init();
  destroy();
}