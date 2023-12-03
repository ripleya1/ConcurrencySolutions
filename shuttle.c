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
- How does the shuttle know to go? (This is a little like the Old Woman, it needs to wait for a post from someone.)
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
int numAttendees = 0;

/*
need some sort of boolean indicating that the number of attendees is at 0 so the shuttle can leave
maybe that being true can trigger a post which triggers the shuttle leaving
but remember that the shuttle also needs to leave if it's full
so shuttle_is_full or attendees_at_0 whichever comes first should tell the shuttle to leave

need a "waiting queue" of some kind for attendees so they don't board if they arrived while the shuttle was boarding
*/

int main(int argc, char *argv[]){
    // represents the space left on the shuttle
    sh = malloc(sizeof(Zem_t));
    Zem_init(&sh, 1);

    // make sure that we can check the attendees variable
    checkAttendees = malloc(sizeof(Zem_t));
    Zem_init(&checkAttendees, 1);

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
        int empty = TRUE;
        while(empty){
            Zem_wait(&checkAttendees);
            empty = numAttendees == 0;
            Zem_post(&checkAttendees);
            if(!empty){
                fillBus();
                travel();
                Zem_post(&sh); // shuttle is back and ready
            }
        }
    }
}

void *attendees(void *arg){
    for(;;){
        srand(time(NULL));
        usleep(rand() % 20 + 1);
        
        Zem_wait(&sh); // wait on the shuttle
        Zem_wait(&checkAttendees);
        if(numAttendees < 30){
            getOn();
        }
        Zem_post(&checkAttendees);
    }
}

void fillBus(){
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