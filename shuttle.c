/*
Shuttle Problem
Constraints:
- Attendees milled about the front entrance, waiting for a shuttle
- When a shuttle arrived, everyone already waiting was allowed to get on the shuttle
- Atendees must wait for next shuttle if they arrive while shuttle is boarding
- The shuttles held 30 people, so if there were more people waiting, they would also have to hang out for the next shuttle.
- When the allowed attendees were onboard, the shuttle headed off.
- If a shuttle arrived and no attendees were waiting, the shuttle immediately departed.

Hints:
- Think about the state you want to keep and the places where you need a semaphore:
- It's likely you'll want to keep track of the attendees, so you'll want an integer and a mutex to do that
    - You'll probably also need another semaphore so they actually be waiting when the shuttle is not there.
- One way to think about the shuttle is that it is a semaphore initialized to 30. 
    - Getting on is a wait and getting off is a post.
- How does the shuttle know to go? (it needs to wait for a post from someone.)
- The shuttle gets to the venue and has to dump the passengers. How is that managed?
- For ease of implementation, assume the shuttle only picks up attendees at one end of the route, and then heads back.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "shuttle.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

Zem_t *checkAttendees;
int numAttendees = 0; // keeps track of the number of attendees waiting for the bus

int main(int argc, char *argv[]){
    // used to ensure that we can check/modify the numAttendees variable
    checkAttendees = malloc(sizeof(Zem_t));
    Zem_init(&checkAttendees, 1);

    pthread_t s;
    Pthread_create(&s, NULL, shutt, NULL);

    pthread_t a[3];
    int i;
    for(i = 0; i < 3; i++){
        Pthread_create(&a[i], NULL, attendees, NULL);
    }

    printf("Shuttling started\n");

    // run forever
    for(;;);

    // this shouldn't print
    printf("Shuttling finished\n");
}

void *shutt(void *arg){
    for(;;){
        // if the waiting queue is empty, wait until it isn't 
        // (ie wait until there's at least one attendee waiting),
        // then fill the bus and take the attendees
        int empty = TRUE;
        while(empty){
            Zem_wait(&checkAttendees);
            empty = numAttendees == 0;
            Zem_post(&checkAttendees);
            if(!empty){
                fillBus();
                travel();
            }
        }
    }
}

void *attendees(void *arg){
    for(;;){
        // wait a random amount of time between 1 and 20 us to add another attendee
        srand(time(NULL));
        usleep(rand() % 20 + 1);
        
        // add another attendee (this means that they're now waiting)
        Zem_wait(&checkAttendees);
        numAttendees++;
        Zem_post(&checkAttendees);
    }
}

void fillBus(){
    int numOnBus = 0;
    Zem_wait(&checkAttendees);
    // if there are less than the max number waiting, let everyone on
    if(numAttendees < 30){
        numOnBus = numAttendees;
    }
    // otherwise, let the max number on
    else{
        numOnBus = 30;
    }
    // calculate remaining waiting
    numAttendees -= numOnBus;
    printf("Bus filled with %d passengers, %d attendees left\n", numOnBus, numAttendees);
    Zem_post(&checkAttendees);
}

void travel(){
    printf("travel\n");
    usleep(20);
    return;
}