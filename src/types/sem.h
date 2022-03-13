#ifndef SEM_H
#define SEM_H

#include <inttypes.h>
/*
 * Semaphore implementation for the synchronization of POSIX threads.
 *
 * This module implements counting P/V semaphores suitable for the 
 * synchronization of POSIX threads. POSIX mutexes and condition variables 
 * shall be utilized to implement the semaphore operations.
 */

/* Opaque type of a semaphore. 
 * ...you need to figure out the contents of struct SEM yourself!
 */
typedef struct SEM SEM;

/**
 * @brief creates a new semaphore. If an error occurs during the 
 * initialization, the implementation shall free all resources already 
 * allocated so far.
 * 
 * @param initVal the initial value of the semaphore
 * @return SEM* handle for the created semaphore, or NULL if an error occured.
 */
SEM *sem_init(uint32_t initVal);

/**
 * @brief Destroys a semaphore and frees all associated resources.
 * 
 * @param sem handle of the semaphore to destroy
 * @return int 0 on success, negative value on error. 
 */
int sem_del(SEM *sem);

/**
 * @brief P (wait) operation. Attempts to decrement the semaphore value by 1. If the semaphore value 
 * is 0, the operation blocks until a V operation increments the value and 
 * the P operation succeeds.
 * 
 * @param sem semaphore handler to decrement 
 */
void P(SEM *sem);

/**
 * @brief V (signal) operation.
 * Increments the semaphore value by 1 and notifies P operations that are 
 * blocked on the semaphore of the change.
 * 
 * @param sem handle of the semaphore to increment
 */
void V(SEM *sem); 

#endif
