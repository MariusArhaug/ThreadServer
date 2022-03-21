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

// handle incomming request to respective handlers, a little pre-processor fun.
#define ROUTE(_endpoint, _route, _method, _handler, _n_m, ...)     \
  if (                                                             \
    strncmp(_endpoint, _route, strlen(_endpoint)) == 0             \
  ) {                                                              \
    if ( illegal_path(_route) )                                    \
      return forbidden_request_handler(connfd, _route);            \
    if ( !is_valid_methods(_method, _n_m, __VA_ARGS__) )           \
      return illegal_method_handler(connfd, _method);              \
    if ( is_regular_file(_route) == false )                        \
      return bad_request_handler(connfd, _route);                  \
    else                                                           \
      return _handler(connfd, _route);                             \
  } else                                                           \
    return not_found_handler(connfd, _route);
    
#define BAD_REQUEST_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"bad request '%s' unable to parse\""\
    "}"\
  "}\n")
  
#define NOT_FOUND_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"endpoint '%s' not found\""\
    "}"\
  "}\n")

#define FORBIDDEN_METHOD_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"the request '%s' has forbidden values\""\
    "}"\
  "}\n")

#define ILLEGAL_METHOD_BODY (\
  "{"\
    "\"error\": {"\
    "\"code\": %d,"\
    "\"message\": \"method '%s' not allowed\""\
    "}"\
  "}\n")

// bad request handlers
static void bad_request_handler(int connfd, char* route);
static void not_found_handler(int connfd, char* route);
static void illegal_method_handler(int connfd, char* route);
static void forbidden_request_handler(int connfd, char* route);

//route handlers
static void doc_handler(int connfd, char* route);


void handle_route(int connfd, char* method, char* route) {
  // allow route under /doc/ with GET method to be accessed, req handled by doc_handler
  ROUTE("/doc/"      , route, method, doc_handler, 1,       "GET") 

  //TODO add more endpoints to make server also behave like REST api.
}

void not_found_handler(int connfd, char* route) {
  response_t* resp = response_init();
  
  set_response_status(resp, NOT_FOUND_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, NOT_FOUND_BODY, NOT_FOUND_C, route);
  set_body(resp, body);

  send_response(resp, connfd);
}

void illegal_method_handler(int connfd, char* method) {
  response_t* resp = response_init();
  
  set_response_status(resp, METHOD_NOT_ALLOWED_S);
  set_response_content(resp, JSON_CONTENT);
  char body[1024];
  sprintf(body, ILLEGAL_METHOD_BODY, METHOD_NOT_ALLOWED_C, method);
  set_body(resp, body);

  send_response(resp, connfd);
}

void bad_request_handler(int connfd, char* route) {
  response_t* resp = response_init();

  set_response_status(resp, BAD_REQUEST_S);
  set_response_content(resp, JSON_CONTENT);

  char body[1024];
  sprintf(body, BAD_REQUEST_BODY, BAD_REQUEST_C, route);
  set_body(resp, body);

  send_response(resp, connfd);
}

void forbidden_request_handler(int connfd, char* route) {
  response_t* resp = response_init();

  set_response_status(resp, FORBIDDEN_S);
  set_response_content(resp, JSON_CONTENT);

  char body[1024];
  sprintf(body, FORBIDDEN_METHOD_BODY, FORBIDDEN_C, route);
  set_body(resp, body);

  send_response(resp, connfd); 
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

  send_response(resp, connfd);
  free(body);
}
