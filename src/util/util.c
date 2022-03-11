#include "util.h"
#include "types/error.h"

#include <stdio.h>
#include <regex.h>

// illegal paths "../" need to escape (\) dots "." and backward slashes "/"
#define ILLEGAL_PATH "\\.\\.\\/"

inline bool illegal_path(char* path) {
  regex_t regex;
  if (regcomp(&regex, ILLEGAL_PATH, REG_EXTENDED) != 0)
    ERROR("Could not compile pattern %s", ILLEGAL_PATH);

  int res = regexec(&regex, path, 0, NULL, 0);
  bool match = false;
  if (res == 0)
    match = true;
  
  if (res != REG_NOMATCH && res != 0) {
    char buff[100];
    regerror(res, &regex, buff, sizeof(buff));
    regfree(&regex);
    ERROR("Regex match failed: %s ", buff);
  }
  regfree(&regex);
  return match;
}