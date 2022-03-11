#ifndef ____BBUFFER___H___
#define ____BBUFFER___H___

#include <inttypes.h>
/*
 * Bounded Buffer implementation to manage int values that supports multiple 
 * readers and writers.
 *
 * The bbuffer module uses the sem module API to synchronize concurrent access 
 * of readers and writers to the bounded buffer.
 */

/* Opaque type of a Bounded Buffer.
 * ...you need to figure out the contents of struct BNDBUF yourself
 */

typedef struct BNDBUF BNDBUF;

/* Creates a new Bounded Buffer. 
 *
 * This function creates a new bounded buffer and all the helper data 
 * structures required by the buffer, including semaphores for 
 * synchronization. If an error occurs during the initialization the 
 * implementation shall free all resources already allocated by then.
 *
 * Parameters:
 *
 * capacity     The number of integers that can be stored in the bounded buffer.
 *
 * Returns:
 *
 * handle for the created bounded buffer, or NULL if an error occured.
 */

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
 * @brief Remove element from bunded buffer, if empty the function blocks until an element is added to the buffer
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