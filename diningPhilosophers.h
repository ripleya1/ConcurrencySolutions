#ifndef diningPhilosophers
#define diningPhilosophers

#define TRUE  1
#define FALSE 0
#define EMPTY -INT32_MAX

typedef struct {
    int p;
    int numLoops;
} threadArgs;

extern int left(int p);
extern int right(int p);
extern void think();
extern void eat();
extern void philosopher(void *arg);
extern void getForks(int p);
extern void putForks(int p);

#endif