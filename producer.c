// Matthew Turi
// ID: 822202323

#include "producer.h"
#include "io.h"

void * produce_frogs(void * prod){
    // cast to proper type
    producer *p = (producer *)prod; 

    // variables we need 
    int sleep_time = 0;
    int onbelt[2] = {0};
    int produced[2] = {0};
    int value = 0;
    int total_produced;

    // set sleep time to avoid weird values / NULLS
    if(p->sleep_f != -1){
        sleep_time = p->sleep_f;
    }

    // start keeping track of how many candies have been produced so we can use as loop condition
    sem_getvalue(p->produced, &total_produced);

    while(total_produced < 100){
        // check if frogs on belt >- 3 - if so, wait till there aren't
        sem_getvalue(p->frog_count, &value);
        if(value >= 3){
            sem_getvalue(p->produced, &total_produced);
            continue;
        }
        // first we wait till the belt has room
        sem_wait(p->slots);
        // then lock the belt
        sem_wait(p->lock);
        // then insert into belt
        for (int i = 0; i < 10; i++){ // look until the empty spot is found
            if (p->belt[i] == 1){
                onbelt[1] += 1; // keep track of what is on the belt for io prints
            }
            if (p->belt[i] == -1){
                p->belt[i] = 0; // 0 for frogs
                // post semaphores for tracking values
                sem_post(p->frog_count);
                sem_post(p->f_prod);
                sem_post(p->produced);
                break;
            }
        }
        // setup for io print
        onbelt[0] = value + 1; // +1 since we just added a frog
        sem_getvalue(p->f_prod, &value); // frogs produced
        produced[0] = value;
        sem_getvalue(p->e_prod, &value); // suckers produced
        produced[1] = value;
        io_add_type(0, onbelt, produced);
        
        // increment slots, unconsumed, and unlock 
        sem_post(p->slots);
        sem_post(p->unconsumed);
        sem_post(p->lock);

        // reset onbelt array
        memset(onbelt, 0, sizeof(onbelt));

        // sleep, then update produced value
        usleep(sleep_time * 1000);
        sem_getvalue(p->produced, &total_produced);
    }  
    // once we exit, call pthread_barrier_wait to signal main that we are done producing
    pthread_barrier_wait(p->barrier);
}

void * produce_suckers(void * prod){
    // cast to proper type
    producer *p = (producer *)prod;

    // variables we need
    int sleep_time = 0;
    int onbelt[2] = {0};
    int produced[2] = {0};
    int value = 0;
    int total_produced;

    // set sleep time to avoid weird values / NULLS
    if(p->sleep_e != -1){
        sleep_time = p->sleep_e;
    }

    // start keeping track of how many candies have been produced so we can use as loop condition
    sem_getvalue(p->produced, &total_produced);

    while(total_produced < 100){
        // first we wait till the belt has room
        sem_wait(p->slots);
        // then lock the belt
        sem_wait(p->lock);
        // then insert into belt
        for (int i = 0; i < 10; i++){ // look until the empty spot is found
            if (p->belt[i] == 1){
                onbelt[1] += 1; // keep track of what is on the belt for io prints
            }
            if (p->belt[i] == -1){
                p->belt[i] = 1; // 1 for suckers 
                // post semaphores for tracking values
                sem_post(p->e_prod);
                sem_post(p->produced);
                onbelt[1] += 1;
                break;
            }
        }
        // setup for io print
        sem_getvalue(p->f_prod, &value); // frogs produced
        produced[0] = value;
        sem_getvalue(p->e_prod, &value); // suckers produced
        produced[1] = value;
        sem_getvalue(p->frog_count, &value); // frogs on belt
        onbelt[0] = value;
        io_add_type(1, onbelt, produced);

        // increment slots, unconsumed, and unlock 
        sem_post(p->slots);
        sem_post(p->unconsumed);
        sem_post(p->lock);

        // reset onbelt array
        memset(onbelt, 0, sizeof(onbelt));

        // sleep, then update produced value
        usleep(sleep_time * 1000);
        sem_getvalue(p->produced, &total_produced);
    }
    // once we exit, call pthread_barrier_wait to signal main that we are done producing
    pthread_barrier_wait(p->barrier);
}

