#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

#define READ "r"

int read_doc_file(FILE** fp ,char* path);
char* file_to_str(FILE* fp);
#endif