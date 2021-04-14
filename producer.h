// Matthew Turi
// ID: 822202323

#ifndef PRODUCER_H
#define PRODUCER_H

#include <semaphore.h>
#include <pthread.h>


////////////////////////////////////////////////////////////////////////////////
// Producer
////////////////////////////////////////////////////////////////////////////////

typedef struct producer{
    sem_t *lock;                    /// base "lock" semaphore for our belt
    int *belt;                      /// conveyor belt array
    sem_t *slots;                   /// semaphore for available slots in the belt
    sem_t *produced;                /// semaphore for total number of candies produced
    sem_t *unconsumed;              /// semaphore for number of unconsumed candies 
    sem_t *frog_count;              /// semaphore for number of frogs on the belt at once
    sem_t *f_prod;                  /// semaphore for number of progs produced (total)
    sem_t *e_prod;                  /// semaphore for number of suckers produced (total)
    float sleep_f;                  /// float sleep value for frog producer
    float sleep_e;                  /// float sleep value for sucker producer
    pthread_barrier_t *barrier;     /// pthread barrier for successful closure upon completion
} producer;

/**
 * @brief       This is the thread is used to produce frogs until both producers have 
 *              reched a combined total of 100 candies produced. 
 * 
 * @param[in]   prod    void pointer to a producer struct (all values must be filled)
 */

void * produce_frogs(void * prod);

/**
 * @brief       This is the thread is used to produce suckers until both producers
 *              have reached a combined total of 100 candies produced.
 * 
 * @param[in]   prod    void pointer to a producer struct (all values must be filled)
 */

void * produce_suckers(void * prod);

#endif