#ifndef __ERROR_H
#define __ERROR_H

#include <stdio.h>
#include <stdlib.h>

// perror and die
#define ERROR_P(_msg) do {\
  perror(_msg);           \
  exit(EXIT_FAILURE);     \
} while(0)

// error and return
#define ERROR_R(_msg, ...)                    \
  fprintf(stderr, _msg "\n", ##__VA_ARGS__);  \
  return;

// error and die
#define ERROR_E(_msg, ...) do {                 \
  fprintf(stderr, _msg "\n", ##__VA_ARGS__);    \
  exit(EXIT_FAILURE);                           \
} while(0)

#endif