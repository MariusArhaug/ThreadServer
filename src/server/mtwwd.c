#include "server.h"
#include "state.h"
#include "router/router.h"
#include "util/env.h"
#include "types/error.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>

#define SOCKETERROR -1

extern struct State state;

static void handle_request(int connfd, int thread_no);

static int check(int fd, const char* msg) {
  if (fd == SOCKETERROR)
    ERROR_P(msg);
  return fd;
}

static int accept_conn(struct server_t* self) {
  socklen_t clilen = sizeof(self->cli_addr);
  int connfd = accept(
      self->sockfd, 
      (struct sockaddr*) &self->cli_addr, 
      &clilen );
  return connfd;
}


void server_init(struct server_t* self) {
  memset(self, 0, sizeof(struct server_t));
  memset(&self->serv_addr, 0, sizeof(struct sockaddr_in));
  memset(&self->cli_addr, 0, sizeof(struct sockaddr_in));

  self->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  self->serv_addr.sin_family = AF_INET;
  self->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  self->serv_addr.sin_port = htons(state.port);
  self->connection_size = CONNECTION_SIZE;

  check(bind(self->sockfd, (struct sockaddr*) &self->serv_addr, sizeof(self->serv_addr)), "Bind failed!");
}

void server_start(struct server_t* self) {
  check(listen(self->sockfd, self->connection_size), "Failed to listen");
  printf("Server listening on port: %d \n", state.port);

  while(true) {
    fflush(stdout);
    int connfd = accept_conn(self);
    if (connfd < 0)
      continue;
    bb_add(state.buffer, connfd);    
  }
}

void* handle_thread(void* arg) {
  struct thread_arg_t* arg_t = arg;

  // keep thread alive
  while (true) 
    handle_request(bb_get(arg_t->buffer), arg_t->thread_no); 
}

//TODO: fix read_conn errors...
void handle_request(int connfd, int thread_no) {  
  char readbuffer[MAXREQ];

  check(read(connfd, readbuffer, sizeof(readbuffer)-1), "Failed to read");
  char* method = strtok(readbuffer, " \t\r\n");
  char* uri = strtok(NULL, " \t");

  printf("%s %s \t Request handled by thread #%d \n\n", method, uri, thread_no);
  handle_route(connfd, method, uri);

  close(connfd);
}

void server_destroy(struct server_t* self) {
  close(self->sockfd);
  free(self);
}
