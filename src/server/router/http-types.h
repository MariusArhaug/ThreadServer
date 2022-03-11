#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H

#include <stdbool.h>

#define HTTP_LENGTH 10

typedef enum {
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE,
  PATCH
} http_methods;

bool is_valid_method(const char* method);

bool allow_methods(char* method, int len, ...);
#endif