#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CONNECTION_SIZE 10
#define MAXREQ (4096*1024)

struct server_t {
  int newfd;
  struct sockaddr_in serv_addr, cli_addr;
  char inet6[INET6_ADDRSTRLEN];
  int sockfd;
  int connection_size;
};


void server_init(struct server_t*, int n_threads, int n_buff);
void server_start(struct server_t*);
void server_destroy(struct server_t*);

#endif