#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util/util.h"

#define CONNECTION_SIZE 10
#define MAXREQ (4096*1024)

#define STDOUT(_x, ...) \
  printf(ANSI_COLOR_GREEN _x ANSI_COLOR_RESET, ##__VA_ARGS__)

typedef void (*thread_f)(void *);

struct server_t {
  struct sockaddr_in serv_addr, cli_addr;
  char inet6[INET6_ADDRSTRLEN];
  int sockfd;
  int connection_size;
  thread_f thread_function;
};

struct req_arg_t {
  char* buffer;
  int connfd;
};

/**
 * @brief initialize server 
 * 
 */
void server_init(struct server_t*, thread_f);

/**
 * @brief main server loop to handle connections and to send them to worker threads for the system.
 * 
 */
void server_start(struct server_t*);

/**
 * @brief release all memory stored by server. 
 * 
 */
void server_destroy(struct server_t*);

#endif