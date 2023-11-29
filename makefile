CC = gcc

all: diningPhilosophersAlgo1.o diningPhilosophersAlgo2.o
	$(CC) diningPhilosophersAlgo1.o -o dp1.exe
	$(CC) diningPhilosophersAlgo2.o -o dp2.exe

clean:
	rm *.o *.exe

.c.o:
	$(CC) -c $< -o $@
