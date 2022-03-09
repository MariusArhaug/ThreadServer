#include "state.h"
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

#define ROUTE(_endpoint, _route, _method, _handler, ...)                    \
  if (                                                            \
    strncmp(_endpoint, _route, strlen(_endpoint)) == 0 &&         \
    allow_methods(_method, (NUM_ARGS(__VA_ARGS__)), ##__VA_ARGS__)\
  ) {\
    _handler(connfd, _route);\
  }
    

static void not_found_handler(int connfd, char* route);
static void doc_handler(int connfd, char* route);


void handle_route(int connfd, char* method, char* route) {
  ROUTE("/doc/write/", route, method, not_found_handler, "POST")
  ROUTE("/doc/", route, method, doc_handler, "GET")
  ROUTE("/", route, method, not_found_handler, "GET")
}

#define ERROR_BODY (\
  "{\n"\
    "\t\"error\": {\n"\
    "\t\t\"code\": %d,\n"\
    "\t\"message\": \"endpoint '%s' not found\""\
    "\t}\n"\
  "}")

void not_found_handler(int connfd, char* route) {
  
  response_t* resp = response_init();
  
  set_response_header(resp, NOT_FOUND_S);
  char body[1024];
  sprintf(body, ERROR_BODY, NOT_FOUND_C, route);
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
  strcpy(cwd, route);
  
  if (read_file(&fp, route) == -1) 
    return not_found_handler(connfd, cwd);

  response_t* resp = response_init();

  char* body = file_to_str(fp);
  set_response_header(resp, OK_S);
  set_body(resp, body);

  char* resp_str = response_to_str(resp);

  write(connfd, resp_str, strlen(resp_str));
  free(resp_str);
  response_destroy(resp);
  free(body);
}

 