/*
Dining Philosophers, Algorithm #3
Each philosopher has their own seat at the table, but they do all of their thinking away from the table.
When they get hungry, they try to sit at the table, and then picks up their forks (first the right and then the left).
Allow at most N-1 philosophers to sit at the table simultaneously.
When a philosopher is done eating, they get up from the table.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "diningPhilosophersAlgo3.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

int numPhilosophers = 0;
int numAtTable = 0;
Zem_t *Fork;
Zem_t *checkTable;

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

    // semaphore indicating whether we can safely look at/modify 
    // the number of philosophers currently at the table (numAtTable)
    checkTable = malloc(sizeof(Zem_t));
    Zem_init(&checkTable, 1);

    // init philosophers
    pthread_t ph[numPhilosophers];
    int i;
    for(i = 0; i < numPhilosophers; i++){
        int p = i;
        Pthread_create(&ph[i], NULL, philosopher, (void *)p);
    }

    // run forever
    for(;;);

    // this shouldn't print
    printf("Dining finished\n");
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
    int roomForOneMore = FALSE;

    // if the table doesn't have room for more philosophers,
    // keep checking if it does until it does,
    // at which point sit down (ie take forks)
    while(!roomForOneMore){
        Zem_wait(&checkTable);
        roomForOneMore = numAtTable < numPhilosophers;
        if(roomForOneMore){
            numAtTable++;
            Zem_wait(&Fork[right(p)]);
            Zem_wait(&Fork[left(p)]);
        }
        Zem_post(&checkTable);
    }

    printf("get %d\n", p);
}

void putForks(int p){
    Zem_post(&Fork[right(p)]);
    Zem_post(&Fork[left(p)]);

    Zem_wait(&checkTable);
    numAtTable--;
    Zem_post(&checkTable);

    printf("put %d\n", p);
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
    usleep(10);
    return;
}

void eat(){
    usleep(10);
    return;
}
