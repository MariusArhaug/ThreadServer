#ifndef __ERROR_H
#define __ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define PERROR(_msg) do { \
  perror(_msg);           \
  exit(EXIT_FAILURE);     \
} while(0)

#define ERROR(_msg, ...) do {                   \
  fprintf(stderr, _msg "\n", ##__VA_ARGS__);\
  exit(EXIT_FAILURE);                           \
} while(0)

#endif