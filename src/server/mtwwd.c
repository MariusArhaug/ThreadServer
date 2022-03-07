#include "server.h"
#include "state.h"
#include "router/router.h"
#include "util/env.h"
#include "types/error.h"

#include <stdio.h>
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

static void handle_request(char* buffer, int connfd);

static inline int accept_conn(struct server_t* self) {
  socklen_t clilen = sizeof(self->cli_addr);
  return accept(
      self->sockfd, 
      (struct sockaddr*) &self->cli_addr, 
      &clilen );
}

static inline int bind_sock(struct server_t* self) {
  return bind(
     self->sockfd, (struct sockaddr*) &self->serv_addr, sizeof(self->serv_addr)
  );
}

void server_init(struct server_t* self, int n_threads, int n_buff) {
  memset(self, 0, sizeof(struct server_t));
  memset(&self->serv_addr, 0, sizeof(struct sockaddr_in));
  memset(&self->cli_addr, 0, sizeof(struct sockaddr_in));

  self->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  self->serv_addr.sin_family = AF_INET;
  self->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  self->serv_addr.sin_port = htons(state.port);
  self->connection_size = CONNECTION_SIZE;

  while (1) {
    if(bind_sock(self) == 0)   
      break;
    self->serv_addr.sin_port = htons(++state.port);
  }
}

void server_start(struct server_t* self) {
  listen(self->sockfd, self->connection_size);
  printf("Server listening on port: %d \n", PORT);
  char readBuff[MAXREQ];
  while(1) {
    int connfd = accept_conn(self);
    if (connfd < 0)
      ERROR("Error reading from socket");
    
    memset(readBuff, 0, MAXREQ);
    int rcvd = read(connfd, readBuff, sizeof(readBuff)-1);
    if (rcvd < 0) 
      ERROR("recv() error");
    if (rcvd==0) 
      ERROR("Client disconnected unexpectedly.");

    handle_request(readBuff, connfd);

    close(connfd);
  }
  
}

void handle_request(char* buffer, int connfd) {  

  char* method = strtok(buffer, " \t\r\n");
  char* uri = strtok(NULL, " \t");

  printf("%s %s\n", method, uri);

  handle_route(connfd, method, uri);
}

void server_destroy(struct server_t* self) {
  close(self->sockfd);
  free(self);
}
