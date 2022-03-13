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

extern struct State state;

struct request_t {
  char readbuffer[MAXREQ];
  int connfd;
};

// static void handle_request(char* buffer, int connfd);
static void handle_request(struct request_t* req);


static inline void accept_conn(struct request_t* req, struct server_t* self) {
  socklen_t clilen = sizeof(self->cli_addr);
  int connfd = accept(
      self->sockfd, 
      (struct sockaddr*) &self->cli_addr, 
      &clilen );
  if (connfd < 0)
     ERROR("Error reading from socket");
  req->connfd = connfd;
}

static inline void bind_sock(struct server_t* self) {
  if (
    bind(
     self->sockfd, 
     (struct sockaddr*) &self->serv_addr, 
     sizeof(self->serv_addr)
    ) == -1
  ) PERROR("Bind error");
}

static inline void read_conn(struct request_t* req) {
  int rcvd = read(req->connfd, req->readbuffer, sizeof(req->readbuffer)-1);
  if (rcvd < 0) 
    ERROR("recv() error");
  if (rcvd==0) 
    ERROR("Client disconnected unexpectedly.");
  req->connfd = rcvd;
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

  bind_sock(self);
}

void server_start(struct server_t* self) {
  listen(self->sockfd, self->connection_size);
  printf("Server listening on port: %d \n", state.port);

  struct request_t request;

  //char readBuff[MAXREQ];
  // struct req_arg_t req_arg = {
  //   .buffer = readBuff,
  // };
  

  while(true) {
    //int connfd;
    
    //check_conn((connfd = accept_conn(self))); 
    //request.connfd = accept_conn(self);
    accept_conn(&request, self);
    memset(request.readbuffer, 0, MAXREQ);
    
    //memset(readBuff, 0, MAXREQ);

    read_conn(&request);

    // pthread_mutext_lock(&(state.buffer))

    bb_add(state.buffer, (void*) &request);
    

    //handle_request(readBuff, connfd);

    //for (int i = 0; i < state.n_threads; i++)
      //pthread_create(state.thread_pool[i], )
    //close(request.connfd);
  }
  
}

// TODO: fix this
void* handle_thread(void* arg) {
  
  while (true) {
    struct request_t* req = bb_get((BNDBUF*) arg);
    if (req != NULL) {
      handle_request(req);
      close(req->connfd);
    }
  }
}

void handle_request(struct request_t* req) {  

  char* method = strtok(req->readbuffer, " \t\r\n");
  char* uri = strtok(NULL, " \t");

  printf("%s %s\n", method, uri);

  handle_route(req->connfd, method, uri);
}

void server_destroy(struct server_t* self) {
  close(self->sockfd);
  free(self);
}
