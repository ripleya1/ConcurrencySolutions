#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "diningPhilosophersAlgo2.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

int numPhilosophers = 0;
Zem_t *Fork;
Zem_t *LookAtForks;

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

    // semaphore indicating whether we can safely look at the values in the fork semaphore
    LookAtForks = malloc(sizeof(Zem_t));
    Zem_init(&LookAtForks, 1);

    printf("Dining started\n");

    // init philosophers
    pthread_t ph[numPhilosophers];
    int i;
    for(i = 0; i < numPhilosophers; i++){
        int p = i;
        Pthread_create(&ph[i], NULL, philosopher, (void *)p);
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
    int forksAvailable = FALSE;

    // if either fork is being used/looked at, keep checking if they aren't until they're not
    while(!forksAvailable){
        Zem_wait(&LookAtForks);
        forksAvailable = &Fork[left(p)].value != 1 || &Fork[right(p)].value != 1;
        if(forksAvailable){ // once they're available, get the forks
            Zem_wait(&Fork[left(p)]);
            Zem_wait(&Fork[right(p)]);
        }
        Zem_post(&LookAtForks);
    }
}

void putForks(int p){
    printf("put %d\n", p);
    Zem_post(&Fork[left(p)]);
    Zem_post(&Fork[right(p)]);
}
