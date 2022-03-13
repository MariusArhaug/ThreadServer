#include "http-types.h"
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


#define STRING(x) #x
const char* methods[9] = {
    STRING(GET),
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

bool is_valid_methods(char* method, int len, ...) {
  va_list allowed_methods;

  va_start(allowed_methods, len);
 
  bool allow = false;
  for (int i = 0; i < len; i++) 
    if (strcmp(method, va_arg(allowed_methods, char*)) == 0) 
      allow = true;
    
  va_end(allowed_methods);
  return allow;
}