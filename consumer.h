// Matthew Turi
// ID: 822202323

#ifndef CONSUMER_H
#define CONSUMER_H

#include <semaphore.h>
#include <pthread.h>


////////////////////////////////////////////////////////////////////////////////
// Consumer
////////////////////////////////////////////////////////////////////////////////

typedef struct consumer{
    sem_t *lock;                    /// base "lock" semaphore for our belt
    int *belt;                      /// conveyor belt array
    sem_t *slots;                   /// semaphore for available slots in the belt
    sem_t *unconsumed;              /// semaphore for number of unconsumed candies          
    sem_t *frog_count;              /// semaphore for number of frogs on the belt
    sem_t *consumed;                /// semaphore for number of candies consumed (total)
    int *frogs;                     /// int * for storing individual consumer frog consumption
    int *suckers;                   /// int * for storing individual consumer sucker consumption
    float sleep_l;                  /// float sleep value for lucy
    float sleep_e;                  /// float sleep value for ethel
    pthread_barrier_t *barrier;     /// pthread barrier for successful closure upon completion
} consumer;

/**
 * @brief       This is the thread is used by both consumers to "consume" candy on the belt
 *              until we have reached our termination condition (100 candies produced and 
 *              consumed).
 * 
 * @param[in]   cons    void pointer to a consumer struct (all values must be filled)
 */

void * consume_candy(void * cons);

#endif