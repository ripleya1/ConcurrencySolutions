# Dining philosophers
## Variables:
### Semaphores
```
Fork [5];
Fork[0] = Fork[1] = Fork[2] = Fork[3] = Fork[4] = 0;
```
### Functions:
```
int left(int p) = {return p;}
int right(int p) = {return (p + 1) % 5;}

DiningPhilosophers(int p){
    think();
    getForks(p);
    eat();
    putForks(p);
}

void getForks(int p){
    if(p == 0){
        wait(Fork[left(p)]);
        wait(Fork[right(p)]);
    }
    wait(Fork[left(p)]);
    // interleaving here
    wait(Fork[left(p)]);
}

void putForks(int p){
    post(Fork[left(p)]);
    post(Fork[right(p)]);
}
```
### What causes deadlock?
```
Philosopher	Fork Acquired	Fork Waiting
0	        0	            1
1	        1	            2
2	        2	            3
3	        3	            4
4	        4	            5
```
- ie every philosopher is waiting on the fork next to them
### Deadlock fix
- Lock wouldn’t work
    - Add case in getForks
        ```
        if(p == 0){
            wait(Fork[left(p)]);
            wait(Fork[right(p)]);
        }
        ```
        ```
        Philosopher	Fork Acquired	Fork Waiting
        0	        1	            0
        1	        	            1
        2	        2	            3
        3	        3	            4
        4	        4, 0	
        ```
        - P4, P0, P3, P2, P1
    - ie disrupt the flow of everyone going the same direction for at least one philosopher
        - The pattern is causing the problem so disrupt it
