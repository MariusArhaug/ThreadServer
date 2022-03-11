#ifndef UTIL_H
#define UTIL_H

#include "env.h"
#include "file.h"
#include "types/error.h"

#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"




/**
 * @brief verify path based on ILLEGAL_PATH
 * 
 * @param path path to be verified 
 * @return true if pattern matches
 * @return false if pattern does not
 */
bool illegal_path(char* path);



#endif