#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util/util.h"

#define CONNECTION_SIZE 1
#define MAXREQ (4096*1024)

struct server_t {
  struct sockaddr_in serv_addr, cli_addr;
  struct sockaddr_in6 serv_addr_6, cli_addr_6;
  int sockfd;
  int sockfd_ip6;
  int connection_size;
};

/**
 * @brief initialize server 
 * 
 */
void server_init(struct server_t*);

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

/**
 * @brief thread function for each consumer thread
 * 
 * @param arg optional arg value 
 * @return void* 
 */
void* handle_thread(void* arg);

#endif