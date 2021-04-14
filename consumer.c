// Matthew Turi
// ID: 822202323

#include "consumer.h"
#include <stdio.h>
#include "io.h"


void * consume_candy(void * cons){
    // cast to proper type
    consumer *c = (consumer *)cons;

    // variables we need
    int sleep_time = 0;
    int onbelt[2] = {0};
    int consumed[2] = {0};
    int removed = -1;
    int cons_type;
    int value = 0;
    int total_consumed = 0;

    // set sleep time to avoid weird values / NULLS
    if(c->sleep_e == -1){
        sleep_time = c->sleep_l;
        cons_type = 0;
    }
    else{
        sleep_time = c->sleep_e;
        cons_type = 1;
    }

    while (1){ 
        // loop exit condition (total candies consumed between two consumers)
        sem_getvalue(c->consumed, &value);
        if (value == 100){
            break;
        }
        // wait till there is something to consume
        sem_wait(c->unconsumed);
        // lock the belt
        sem_wait(c->lock);
        // then grab first item in belt, and keep track of what it was for io prints
        if(c->belt[0] == 0){
            sem_wait(c->frog_count);
            removed = 0;
            consumed[0]++;
        }
        else{
            removed = 1;
            consumed[1]++;
        }
        
        // shift array to maintain a queue
        for (int i = 0; i < 9; i++){ 
            c->belt[i] = c->belt[i+1];
            if (c->belt[i] == 1){
                onbelt[1] += 1; // also keep track of how many suckers were on the belt
            }
        }
        // setup for io print
        sem_getvalue(c->frog_count, &value); // frogs on belt
        onbelt[0] = value;
        c->belt[9] = -1; // place a sentinel at end of shifted belt
        io_remove_type(cons_type, removed, onbelt, consumed);

        // increment slots, consumed, and unlock
        sem_post(c->consumed);
        sem_post(c->slots);
        sem_post(c->lock);

        // reset onbelt array
        memset(onbelt, 0, sizeof(onbelt));

        // update how many frogs and suckers this consumer consumed
        *c->frogs = consumed[0];
        *c->suckers = consumed[1];

        // sleep
        usleep(1000 * sleep_time);
    }   
    // once we exit, call pthread_barrier_wait to signal main that we are done producing
    pthread_barrier_wait(c->barrier);
}


