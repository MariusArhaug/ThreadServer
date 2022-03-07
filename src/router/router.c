#include "router.h"
#include "http-types.h"
#include "types/response.h"
#include "util/file.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_ARGS(...)\
  (int) ((sizeof((char*[HTTP_METHOD_LENGTH]) {0, ##__VA_ARGS__}))/(sizeof(char*[HTTP_METHOD_LENGTH])) - 1)

#define ROUTE(_endpoint, _route, _method, ...)\
  if (\
    strncmp(_endpoint, _route, strlen(_endpoint)) == 0 &&\
    allow_methods(_method, (NUM_ARGS(__VA_ARGS__)), ##__VA_ARGS__)\
  ) 
    

static void not_found_handler(int connfd);
static void doc_handler(int connfd, char* route);


void handle_route(int connfd, char* method, char* route) {
  
  ROUTE("/doc/", route, method, "GET", "PATCH") 
    doc_handler(connfd, route);
  
  ROUTE("/", route, method, "GET")
    not_found_handler(connfd);

  
}

#define NOT_FOUND_BODY "{\"error\": \"endpoint not found\"}"

void not_found_handler(int connfd) {
  
  response_t* resp = response_init();
  
  set_response_header(resp, NOT_FOUND);
  set_body(resp, NOT_FOUND_BODY);

  char* resp_str = response_to_str(resp);
  
  write(connfd, resp_str, strlen(resp_str));

  free(resp_str);
  response_destroy(resp);
}

void doc_handler(int connfd, char* route) {
 
  FILE* fp;
  if (read_doc_file(&fp, route) == -1) 
    return not_found_handler(connfd);

  response_t* resp = response_init();

  char* body = file_to_str(fp);
  set_response_header(resp, OK);
  set_body(resp, body);

  char* resp_str = response_to_str(resp);

  write(connfd, resp_str, strlen(resp_str));
  free(resp_str);
  response_destroy(resp);
  free(body);
}

 