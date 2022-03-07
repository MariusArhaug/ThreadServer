#ifndef __ERROR_H
#define __ERROR_H

#include <stdio.h>

#define PERROR(_msg) do {\
  perror(_msg);\
  exit(EXIT_FAILURE);\
} while(0)

#define ERROR(_msg) do {\
  fprintf(stderr, "%s \n", _msg);\
  exit(EXIT_FAILURE);\
} while(0)

#endif