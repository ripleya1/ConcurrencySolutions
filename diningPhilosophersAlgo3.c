#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "diningPhilosophersAlgo3.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

int numPhilosophers = 0;
Zem_t *Fork;
Zem_t *Phil;

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
    Fork = malloc(sizeof(Zem_t) * numPhilosophers);
    for(int i = 0; i < numPhilosophers; i++){
        Zem_init(&Fork[i], 1);
    }

    printf("Dining started\n");

    Phil = malloc(sizeof(Zem_t) * numPhilosophers); // indicates whether or not the philosopher is at the table

    // init philosophers
    pthread_t ph[numPhilosophers];
    int i;
    for(i = 0; i < numPhilosophers; i++){
        int p = i;
        Pthread_create(&ph[i], NULL, philosopher, (void *)p);
        Zem_init(&Phil[i], 1);
    }

    for(;;);

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
    usleep(10);
    return;
}

void eat(){
    // printf("e\n");
    usleep(10);
    return;
}

void *philosopher(void *arg){
    int p = (int) arg;
    for(;;){
        printf("think %d\n", p);
        think();
        getForks(p);
        printf("eat %d\n", p);
        eat();
        putForks(p);
    }
    return NULL;
}

void getForks(int p){
    printf("get %d\n", p);
    // if(p == numPhilosophers - 1){
    //     Zem_wait(&Fork[right(p)]);
    //     Zem_wait(&Fork[left(p)]);
    // }
    // else{
        Zem_wait(&Fork[left(p)]);
        // interleaving here
        Zem_wait(&Fork[right(p)]);
    // }
}

void putForks(int p){
    printf("put %d\n", p);
    Zem_post(&Fork[left(p)]);
    Zem_post(&Fork[right(p)]);
}
