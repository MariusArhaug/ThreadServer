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

#define ROUTE(_endpoint, _route, _method, _handler, _n, ...)       \
  if (                                                             \
    strncmp(_endpoint, _route, strlen(_endpoint)) == 0           &&\
    !illegal_path(_route)                                          \
  ) {                                                              \
    if (allow_methods(_method, _n, __VA_ARGS__) == false)          \
      return illegal_method_handler(connfd, _method);              \
    else                                                           \
      return _handler(connfd, _route);                             \
  }
    

static void not_found_handler(int connfd, char* route);
static void doc_handler(int connfd, char* route);
static void illegal_method_handler(int connfd, char* route);

void handle_route(int connfd, char* method, char* route) {
  ROUTE("/doc/write/", route, method, not_found_handler, 1, "POST")
  ROUTE("/doc/"      , route, method, doc_handler, 1,       "GET")
  ROUTE("/"          , route, method, not_found_handler, 1, "GET")

}

#define NOT_FOUND_BODY (\
  "{\n"\
    "\t\"error\": {\n"\
    "\t\t\"code\": %d,\n"\
    "\t\"message\": \"endpoint '%s' not found\""\
    "\t}\n"\
  "}\n")

void not_found_handler(int connfd, char* route) {
  
  response_t* resp = response_init();
  
  set_response_status(resp, NOT_FOUND_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, NOT_FOUND_BODY, NOT_FOUND_C, route);
  set_body(resp, body);

  char* resp_str = response_to_str(resp);
  
  write(connfd, resp_str, strlen(resp_str));

  free(resp_str);
  response_destroy(resp);
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

  char* resp_str = response_to_str(resp);

  write(connfd, resp_str, strlen(resp_str));
  free(resp_str);
  response_destroy(resp);
  free(body);
}

#define ILLEGAL_METHOD_BODY (\
  "{\n"\
    "\t\"error\": {\n"\
    "\t\t\"code\": %d,\n"\
    "\t\"message\": \"method '%s' not allowed\""\
    "\t}\n"\
  "}\n")

void illegal_method_handler(int connfd, char* method) {
  response_t* resp = response_init();
  
  set_response_status(resp, METHOD_NOT_ALLOWED_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, ILLEGAL_METHOD_BODY, METHOD_NOT_ALLOWED_C, method);
  set_body(resp, body);

  char* resp_str = response_to_str(resp);
  
  write(connfd, resp_str, strlen(resp_str));

  free(resp_str);
  response_destroy(resp);
}
 