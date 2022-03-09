#include "file.h"
#include "state.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ "r"

int read_file(FILE** fp, char* path) {
  if( access( path, F_OK ) != 0 ) 
    return -1; 

  (*fp) = fopen(path, READ);
  
  return 0;
}

char* file_to_str(FILE* fp) {
  long lSize;
  char *buffer;

  fseek(fp, 0L, SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  buffer = calloc(1, lSize + 1);
  if (buffer == NULL) {
    fclose(fp);
    fprintf(stderr, "Not enough space to calloc \n");
    exit(EXIT_FAILURE);
  }

  if (fread(buffer, lSize, 1 , fp) != 1) {
    fclose(fp);
    fprintf(stderr, "Read failure \n");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
  return buffer;
}