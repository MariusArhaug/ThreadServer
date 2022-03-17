#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>
#include <stdbool.h>

#define READ "r"

/**
 * @brief read file  
 * 
 * @param fp pointer to file pointer
 * @param path location of given file
 * @return returns 0 if no errors or -1 file not found
 */
int read_file(FILE** fp ,char* path);

/**
 * @brief transalte content of file pointer to string
 * 
 * @param fp file pointer 
 * @return char* stirng convertion
 */
char* file_to_str(FILE* fp);


/**
 * @brief checks if path is path to file
 * 
 * @param path path to check 
 * @return true is path to file
 * @return false  is not path to file but directory or something else..
 */
bool is_regular_file(const char *path);

#endif