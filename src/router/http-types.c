#include "http-types.h"
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>


#define STRING(x) #x
const char* methods[9] = {
    STRING(GET),
    STRING(HEAD),
    STRING(POST),
    STRING(PUT),
    STRING(DELETE),
    STRING(CONNECT),
    STRING(OPTIONS),
    STRING(TRACE),
    STRING(PATCH),
};
#undef STRING



bool is_valid_method(const char* method) {
  for (int i = 0; i < 9; i++) 
    if (strcmp(methods[i], method) == 0) return true;
  return false; 
}

bool allow_methods(char* method, int len, ...) {
  va_list methods;

  va_start(methods, len);
 
  bool allow = true;

  for (int i = 0; i < len; i++) 
    if (strcmp(method, va_arg(methods, char*)) != 0) 
      allow = false;
    
  va_end(methods);
  return allow;
}