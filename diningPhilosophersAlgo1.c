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
    
    // printf("numPhilosophers: %d\n", numPhilosophers);
    // init forks
    Zem_t Fork[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i++){
        Zem_init(&Fork[i], 1);
        // printf("%p\n", &Fork[i]);
    }

    printf("Dining started\n");

    // init philosophers
    pthread_t ph[numPhilosophers];
    int i;
    for(i = 0; i < numPhilosophers; i++){
        int p = i;
        Pthread_create(&ph[i], NULL, philosopher, (void *)p);
    }
    // printf("Philosophers inited\n");
    printf("Dining finished\n");
}

int left(int p){
    // printf("l: %d\n", p);
    return p;
}

int right(int p){
    // printf("r: %d\n", p);
    return (p + 1) % numPhilosophers;
}

void think(){
    // printf("t\n");
    sleep(1);
    return;
}

void eat(){
    // printf("e\n");
    sleep(1);
    return;
}

void *philosopher(void *arg){
    int p = (int) arg;
    for(int i = 0;; i++){ // infinite loop
        think();
        getForks(p);
        eat();
        putForks(p);
    }
    return NULL;
}

void getForks(int p){
    printf("g: %d\n", p);
    // if(p == numPhilosophers - 1){ // TODO: p == numPhilosophers - 1?
    //     Zem_wait(&Fork[left(p)]);
    //     Zem_wait(&Fork[right(p)]);
    // }
    // TODO: else?
    Zem_wait(&Fork[left(p)]);
    // interleaving here
    Zem_wait(&Fork[right(p)]);
}

void putForks(int p){
    // printf("p: %d\n", p);
    Zem_post(&Fork[left(p)]);
    Zem_post(&Fork[right(p)]);
}
