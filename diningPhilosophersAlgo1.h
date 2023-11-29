#ifndef diningPhilosophersAlgo1
#define diningPhilosophersAlgo1

#define TRUE 1
#define FALSE 0

extern int left(int p);
extern int right(int p);
extern void think();
extern void eat();
extern void *philosopher(void *arg);
extern void getForks(int p);
extern void putForks(int p);

#endif