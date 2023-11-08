#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

int numPhilosophers = 0;
Zem_t *Fork; // TODO: not sure how not allowed this is

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Usage: DiningPhilosophers [philosophers]\n");        exit(1);
    }
    numPhilosophers = atoi(argv[1]);
    if(numPhilosophers < 3 || numPhilosophers > 20){
        printf("There must be between 3 and 20 philosophers\n");
    }
    
    // init forks
    Zem_t Fork[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i++){
        Zem_init(&Fork[i], 1);
    }

    // init philosophers

}

int left(int p){
    return p;
}

int right(int p){
    return (p + 1) % numPhilosophers;
}

void think(){
    sleep(1);
    return;
}

void eat(){
    sleep(1);
    return;
}

void philosopher(int p, int loops){
    for(int i = 0; i < loops; i++){
        think();
        getForks(p);
        eat();
        putForks(p);
    }
}

void getForks(int p){
    if(p == 0){
        Zem_wait(&Fork[left(p)]);
        Zem_wait(&Fork[right(p)]);
    }
    Zem_wait(&Fork[left(p)]);
    // interleaving here
    Zem_wait(&Fork[left(p)]);
}

void putForks(int p){
    Zem_post(&Fork[left(p)]);
    Zem_post(&Fork[right(p)]);
}
