#ifndef ____BBUFFER___H___
#define ____BBUFFER___H___

#include <inttypes.h>

typedef struct BNDBUF BNDBUF;

/**
 * @brief create new buffer
 * 
 * @param capacity The number of integers that can be stored in the bounded buffer.
 * @return BNDBUF* pointer for bounded buffer, or NULL if an error occured.
 */
BNDBUF *bb_init(uint32_t capacity);

/**
 * @brief Release buffer and all resources associated with the buffer.
 * 
 * @param bb andle of the bounded buffer that shall be freed.
 */
void bb_del(BNDBUF *bb);

/**
 * @brief Remove element from bunded buffer, if empty, the function blocks until an element is added to the buffer
 * 
 * @param bb bunded buffer
 * @return int element
 */
int bb_get(BNDBUF *bb);

/**
 * @brief  Adds element to bounded buffer. if full it blocks until an element is removed.
 * 
 * @param bb bound buffer
 * @param fd value to be added
 */
void bb_add(BNDBUF *bb, int fd);

#endif