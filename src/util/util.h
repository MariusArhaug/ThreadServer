#ifndef UTIL_H
#define UTIL_H

#include "env.h"
#include "file.h"
#include "types/error.h"

#include <stdbool.h>




/**
 * @brief verify path based on ILLEGAL_PATH
 * 
 * @param path path to be verified 
 * @return true if pattern matches
 * @return false if pattern does not
 */
bool illegal_path(char* path);



#endif