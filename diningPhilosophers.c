#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

Zem_t Fork[5];

int left(int p){
    return p;
}

int right(int p){
    return (p + 1) % 5;
}

void think(){
    sleep(1);
    return;
}

void eat(){
    sleep(1);
    return;
}

void Philosopher(int p, int loops){
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

int main(int argc, char *argv[]) {
    printf("Usage: DiningPhilosophers [philosophers] [forks]\n");
    // init forks
    for(int i = 0; i < 5; i++){
        Zem_init(&Fork[i], 1);
    }
}