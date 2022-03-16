#include "state.h"
#include "router.h"
#include "http-types.h"
#include "types/response.h"
#include "util/util.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ROUTE(_endpoint, _route, _method, _handler, _n_m, ...)     \
  if (                                                             \
    strncmp(_endpoint, _route, strlen(_endpoint)) == 0           &&\
    !illegal_path(_route)                                          \
  ) {                                                              \
    if ( is_valid_methods(_method, _n_m, __VA_ARGS__) )            \
      return _handler(connfd, _route);                             \
    else                                                           \
      return illegal_method_handler(connfd, _method);              \
  }
    
#define NOT_FOUND_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"endpoint '%s' not found\""\
    "}"\
  "}\n")

#define ILLEGAL_METHOD_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"method '%s' not allowed\""\
    "}"\
  "}\n")


static void not_found_handler(int connfd, char* route);
static void doc_handler(int connfd, char* route);
static void illegal_method_handler(int connfd, char* route);

void handle_route(int connfd, char* method, char* route) {
  ROUTE("/doc/write/", route, method, not_found_handler, 1, "POST")
  ROUTE("/doc/"      , route, method, doc_handler, 1,       "GET")
  ROUTE("/"          , route, method, not_found_handler, 1, "GET")

}

void not_found_handler(int connfd, char* route) {
  response_t* resp = response_init();
  
  set_response_status(resp, NOT_FOUND_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, NOT_FOUND_BODY, NOT_FOUND_C, route);
  set_body(resp, body);

  send_response(connfd, resp);
}

void illegal_method_handler(int connfd, char* method) {
  response_t* resp = response_init();
  
  set_response_status(resp, METHOD_NOT_ALLOWED_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, ILLEGAL_METHOD_BODY, METHOD_NOT_ALLOWED_C, method);
  set_body(resp, body);

  send_response(connfd, resp);
}

void doc_handler(int connfd, char* route) {
  FILE* fp;

  char cwd[FILENAME_MAX]; 
  strcpy(cwd, state.wd);
  strcat(cwd, route);
  
  if (read_file(&fp, cwd) == -1) 
    return not_found_handler(connfd, route);

  response_t* resp = response_init();

  char* body = file_to_str(fp);
  set_response_status(resp, OK_S);
  set_response_content(resp, HTML_CONTENT);

  set_body(resp, body);

  send_response(connfd, resp);
  free(body);
}
