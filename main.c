// Matthew Turi
// ID: 822202323

#include "consumer.h"
#include "producer.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

int main(int argc, char **argv) {
    // first need to parse args for sleep times, so setup variables
    int opt = 0;
    int sleep_f = 0;
    int sleep_e = 0;
    int sleep_E = 0;
    int sleep_L = 0;

    // getopt cases
    while ((opt = getopt(argc, argv, "E:L:f:e:")) != -1) {
        switch (opt) {
            case 'E': /* Number of addresses to process */
                sleep_E = atoi(optarg);
                break;
            case 'L':/* produce map of pages */
                sleep_L = atoi(optarg);
                break;
            case 'f':/* produce map of pages */
                sleep_f = atoi(optarg);
                break;
            case 'e':/* produce map of pages */
                sleep_e = atoi(optarg);
                break;             
            default:
                // print something about the usage and die...
                fprintf(stderr, "UNKNOWN ARGS PROVIDED\n");
                exit(-1);
        }
    }

    // create all the variables we will need for our producers and consumers
    sem_t lock;
    sem_t slots;
    sem_t unconsumed;
    sem_t fc;
    sem_t frogs;
    sem_t suckers;
    sem_t produced;
    sem_t consumed;
    int b[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int l_f = 0;
    int l_e = 0;
    int e_f = 0;
    int e_e = 0;

    // initialize sempahores to their respective values
    sem_init(&lock, 1, 1);
    sem_init(&slots, 1, 10);
    sem_init(&unconsumed, 1, 0);
    sem_init(&fc, 1, 0);
    sem_init(&frogs, 1, 0);
    sem_init(&suckers, 1, 0);
    sem_init(&produced, 1, 0);
    sem_init(&consumed, 1, 0);

    // create our barrier
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, 4);


    // now init our producer and two consumers
    producer p = { &lock, &b, &slots, &produced, &unconsumed, &fc, &frogs, &suckers, sleep_f, sleep_e, &barrier };
    consumer lucy = { &lock, &b, &slots, &unconsumed, &fc, &consumed, &l_f, &l_e, sleep_L, -1, &barrier}; 
    consumer ethel = { &lock, &b, &slots, &unconsumed, &fc, &consumed, &e_f, &e_e, -1, sleep_E, &barrier}; 
    
    // now start up the threads
    pthread_t ptid[4];
    pthread_create( &ptid[0], NULL,
                    &produce_frogs, (void*)&p);
    pthread_create( &ptid[1], NULL,
                    &produce_suckers, (void*)&p);
    pthread_create( &ptid[2], NULL,
                    &consume_candy, (void*)&lucy);
    pthread_create( &ptid[3], NULL,
                    &consume_candy, (void*)&ethel); 

    // wait condition: once all 4 wait on the barrier, we are good to shut them down
    pthread_barrier_wait(&barrier);
    for (int i = 0; i < 4; i++){
        pthread_cancel(ptid[i]);
    }

    // setup for io production report 
    int val = 0;
    int hold[2] = {0};
    sem_getvalue(&frogs, &val);     // number of frogs produced
    hold[0] = val;
    sem_getvalue(&suckers, &val);   // number of suckers produced
    hold[1] = val;
    int *cons_arr[2];
    int lucy_arr[2] = {l_f, l_e};   // lucy's consumption stats
    int ethel_arr[2] = {e_f, e_e};  // ethel's consumption stats
    cons_arr[0] = &lucy_arr;
    cons_arr[1] = &ethel_arr;

    // call production report, and exit cleanly
    io_production_report(hold, cons_arr);
    exit(0);
}