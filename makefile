CC = gcc

all: diningPhilosophersAlgo1.o
	$(CC) diningPhilosophersAlgo1.o -o dp1.exe

clean:
	rm *.o *.exe

.c.o:
	$(CC) -c $< -o $@
