#include "state.h"
#include "server/server.h"
#include "types/error.h"
#include "types/bbuffer.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

// global state
struct State state;

// handle ctrl+c to free resources
void sig_handler(int signum) {
  printf("\nAre you sure you want to shutdown the server? [y/n]\n> ");
  char *line = NULL;
  size_t size;
  if (getline(&line, &size, stdin) == -1) {
    printf("Unexpected behaviour, shutting down.. \n"); 
    state.run = false;
    for (int i = 0; i < state.n_threads; i++)
      pthread_cancel(state.thread_pool[i]);
    return;
  }
  if (strcmp(line, "y\n") == 0) {
    printf("yes, shutting down..\n");
    state.run = false;
    for (int i = 0; i < state.n_threads; i++)
      pthread_cancel(state.thread_pool[i]);
  }
  else if (strcmp(line,"n\n") == 0) {
    printf("no, continuing..\n");
  }
  else 
    printf("Unrecognized input, continuing server \n");
}

void init() {
  // init poijters, malloc(0) returns NULL so we need to watch out when n_threads == 0
  struct sigaction act;
  memset(&act,0,sizeof(struct sigaction)); 
  act.sa_handler = sig_handler;
  sigaction(SIGINT, &act, NULL);
  
  state.run = true;
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

// free resources
void destroy() {
  if (state.is_multihreaded) {
    for (int i = 0; i < state.n_threads; i++) {
      free(state.thread_args[i]);
    }
    free(state.thread_args);
    free(state.thread_pool);
  }
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

  if (state.n_bufferslots < 0)
    ERROR_E("[#bufferslots] must be greater than zero");
  
  if (state.n_threads < 0)
    ERROR_E("[#threads] cannot be a negative value");

  state.is_multihreaded = state.n_threads == 0 ? false : true;
}


int main(int argc, char* argv[]) {
  check_args(argc, argv); 
  init();
  destroy();
}