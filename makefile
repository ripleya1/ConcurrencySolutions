CC = gcc

all: dp1 dp2 dp3

dp1: diningPhilosophersAlgo1.o
	$(CC) $^ -o $@.exe

dp2: diningPhilosophersAlgo2.o
	$(CC) $^ -o $@.exe

dp3: diningPhilosophersAlgo3.o
	$(CC) $^ -o $@.exe

clean:
	rm -f *.o *.exe

.c.o:
	$(CC) -c $< -o $@
