# Concurrency Solutions
## How to Run
Running `make` should make all of the executables. If you want to make a specific executable you can do so by running `make [executable name]`.

The executables for Dining Philosophers are named `dpN.exe` where `N` is the number of the algorithm. The executable for the Shuttle Problem is named `sh.exe`.

Dining Philosophers accepts one command line argument - the number of philosophers, which must be between 3 and 20 (nclusive).

## Dining Philosophers
Each algorithm is implemented in separate .c files named `diningPhilosophersAlgoN.c` where `N` is the number of the algorithm. Each also has a .h file with the same naming scheme.

### Algorithm Analysis (WIP)
- [`diningPhilosophersAlgo1.c`](diningPhilosophersAlgo1.c)
    - This algorithm deadlocks.
- [`diningPhilosophersAlgo2.c`](diningPhilosophersAlgo2.c)
    - This algorithm does not deadlock.
- [`diningPhilosophersAlgo3.c`](diningPhilosophersAlgo3.c)
    - This algorithm does not deadlock.

## Shuttle Problem
### A Note
For some reason, every once in a while this randomly stalls at the beginning (ie none of the threads get started), so if that happens just retry running the executable a few times and it should fix itself.

### Comments (WIP)
- See my comments in my code for slightly more detail, but the main thing to note is that the `numAttendees` variable keeps track of the number of attendees currently waiting at the bus stop. There is a semaphore called `checkAttendees` for locking around this variable. I think of this as the "waiting queue" (not actually a queue obviously because we aren't keeping track of the position of each individual). This variable is decremented by 30 every time the bus comes, or by the value of the variable if the value is less than 30 (ie the value of the variable becomes 0).
- Also, there are 3 attendees threads that sleep a random amount of time and then increment the `numAttendees` variable, representing a new attendee waiting at the bus stop.
