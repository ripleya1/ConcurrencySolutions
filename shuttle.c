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

Zem_t *sh;
Zem_t *checkAttendees;
Zem_t *shuttleCanLeave;
int numAttendees = 0;

int main(int argc, char *argv[]){
    // tells us if the shuttle is there
    sh = malloc(sizeof(Zem_t));
    Zem_init(&sh, 1);

    // make sure that we can check the attendees variable
    checkAttendees = malloc(sizeof(Zem_t));
    Zem_init(&checkAttendees, 1);

    // tells us if there are attendees waiting for the shuttle
    shuttleCanLeave = malloc(sizeof(Zem_t));
    Zem_init(&shuttleCanLeave, 0);

    pthread_t s;
    Pthread_create(&s, NULL, shutt, NULL);

    pthread_t a[5];
    // Pthread_create(&a, NULL, attendees, NULL);
    int i;
    for(i = 0; i < 5; i++){
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
        Zem_wait(&sh); 
        
        int numOnBus = 0;
        Zem_wait(&checkAttendees);
        if(numAttendees < 30){
            numOnBus = numAttendees;
        }
        else{
            numOnBus = 30;
        }
        numAttendees -= numOnBus;
        printf("Bus filled with %d passengers, %d attendees left\n", numOnBus, numAttendees);
        Zem_post(&checkAttendees);

        travel();
        Zem_post(&sh); // shuttle is back and ready
    }
}

void *attendees(void *arg){
    for(;;){
        srand(time(NULL));
        usleep(rand() % 20 + 1);
        
        Zem_wait(&sh);

        Zem_wait(&checkAttendees);
        if(numAttendees < 30){
            getOn();
            if(numAttendees == 30){
                Zem_post(&sh); // shuttle is full
            }
            // Zem_post(&checkAttendees);
            // Zem_wait(&sh) // wait on shuttle to leave
        }
        Zem_post(&checkAttendees);
        Zem_post(&sh);
    }
}

void getOn(){
    numAttendees++;
    // usleep(2);
    return;
}

void travel(){
    printf("travel\n");
    usleep(20);
    return;
}