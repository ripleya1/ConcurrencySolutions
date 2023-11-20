#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "diningPhilosophers.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

int numPhilosophers = 0; // TODO: not sure how allowed this is
Zem_t *Fork; // TODO: not sure how not allowed this is

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: DiningPhilosophers [philosophers]\n");
        exit(1);
    }
    numPhilosophers = atoi(argv[1]); // set numPhilosophers
    if(numPhilosophers < 3 || numPhilosophers > 20){
        printf("There must be between 3 and 20 philosophers\n");
        exit(1);
    }
    
    // init forks
    Zem_t Fork[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i++){
        Zem_init(&Fork[i], 1);
    }
    printf("numPhilosophers: %d\n", numPhilosophers);

    printf("Dining started\n");

    // init philosophers
    pthread_t ph[numPhilosophers];
    threadArgs arg[numPhilosophers];
    int i;
    for(i = 0; i < numPhilosophers; i++){
        arg[i].p = i;
        arg[i].numLoops = 1;
        Pthread_create(&ph[i], NULL, philosopher, &arg[i]);
    }
    printf("Philosophers inited\n");

    // run philosphers
    for(i = 0; i < numPhilosophers; i++){
        Pthread_join(ph[i], NULL);
    }

    printf("Dining finished\n");
}

int left(int p){
    printf("l: %d\n", p);
    return p;
}

int right(int p){
    printf("r: %d\n", p);
    return (p + 1) % numPhilosophers;
}

void think(){
    printf("t\n");
    // sleep(1);
    return;
}

void eat(){
    printf("e\n");
    // sleep(1);
    return;
}

void *philosopher(void *arg){
    threadArgs *args = (threadArgs *) arg;
    int i;
    for(i = 0; i < args -> numLoops; i++){
        think();
        getForks(args -> p);
        eat();
        putForks(args -> p);
    }
    return NULL;
}

void getForks(int p){
    printf("g: %d\n", p);
    if(p == numPhilosophers - 1){ // TODO: p == numPhilosophers - 1?
        Zem_wait(&Fork[left(p)]);
        Zem_wait(&Fork[right(p)]);
    }
    // TODO: else?
    Zem_wait(&Fork[left(p)]);
    // interleaving here
    Zem_wait(&Fork[left(p)]);
}

void putForks(int p){
    printf("p: %d\n", p);
    Zem_post(&Fork[left(p)]);
    Zem_post(&Fork[right(p)]);
}
