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
  state.thread_args = malloc(state.n_threads*sizeof(struct thread_arg_t*));
  for (int i = 0; i < state.n_threads; i++) {
    struct thread_arg_t* arg = malloc(sizeof(struct thread_arg_t));
    arg->buffer = state.buffer;
    arg->thread_no = i;
    state.thread_args[i] = arg;
    pthread_create(&state.thread_pool[i], NULL, handle_thread, state.thread_args[i]); // pass buffer to handle_thread func
  }

  server_init(
    &state.server
  );
  server_start(&state.server); 
}

void destroy() {
  for (int i = 0; i < state.n_threads; i++) {
    pthread_cancel(state.thread_pool[i]);
    pthread_join(state.thread_pool[i], NULL);
    free(state.thread_args[i]);
  }
  free(state.thread_args);
  free(state.thread_pool);
  bb_del(state.buffer);
  server_destroy(&state.server);
}


static inline void check_args(int argc, char* argv[]) {
  if (argc != 5)
    ERROR_P("Usage: ./mtwwd [www-path] [port] [#threads] [#bufferslots]"); 
  
  strcpy(state.wd, argv[1]);

  errno = 0;
  state.port = strtol(argv[2], NULL, 10);
  state.n_threads = strtol(argv[3], NULL, 10);
  state.n_bufferslots = strtol(argv[4], NULL, 10);

  if (errno == ERANGE)
    ERROR_P("Number port, n_threads or n_bufferslot is not a valid number (parsing error)");

  if (state.n_bufferslots <= 0)
    ERROR_E("[#bufferslots] cannot be a negative value or zero");
  
  if (state.n_threads <= 0)
    ERROR_E("[#threads] cannot be a negative value or zero");
}


int main(int argc, char* argv[]) {
  check_args(argc, argv); 
  init();
  destroy();
}