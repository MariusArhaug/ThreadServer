#define _GNU_SOURCE
#include "types/error.h"

#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define ENV_PATH(p) strcat(p, "/.env")
#define ENV_SEPARATOR "="

#define ENV_NOT_FOUND "Could not find .env file."
#define UNEXPECTED_ERROR "Unexpected error when splitting line."
#define KEY_NOT_FOUND "Key not found."
#define READ "r"


char* get_envv(char* key) {
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
    char* token = strtok(line, ENV_SEPARATOR);
    if (token == NULL)
      ERROR(UNEXPECTED_ERROR);
    if (strcmp(token, key) == 0) {
      char* lastToken;
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