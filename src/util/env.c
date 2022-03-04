#define _GNU_SOURCE
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ENV_PATH(p) strcat(p, "/.env")
#define ENV_SEPARATOR "="
#define ERROR(msg) do { \
  fprintf(stderr, "Error: %s at line: %d\n", msg, __LINE__);\
  exit(EXIT_FAILURE);\
} while (0)

#define ENV_NOT_FOUND "Could not find .env file."
#define UNEXPECTED_ERROR "Unexpected error when splitting line."
#define KEY_NOT_FOUND "Key not found."
#define READ "r"


const char* getEnvVar(char* key) {
  char cwd[FILENAME_MAX];
  getcwd(cwd, FILENAME_MAX);
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(ENV_PATH(cwd), READ);
  if (fp == NULL)
    ERROR(ENV_NOT_FOUND);
  
  while ((read = getline(&line, &len, fp)) != -1) {
    const char* token = strtok(line, ENV_SEPARATOR);
    if (token == NULL)
      ERROR(UNEXPECTED_ERROR);
    if (strcmp(token, key) == 0) {
      const char* lastToken;
      while (token != NULL) {
        lastToken = token;
        token = strtok(NULL, ENV_SEPARATOR);
      } 
      return lastToken;
    }
  }
  fclose(fp);
  if (line)
    free(line);
  ERROR(KEY_NOT_FOUND);
}