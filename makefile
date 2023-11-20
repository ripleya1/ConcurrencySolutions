CC = gcc

all: diningPhilosophers.o
	$(CC) diningPhilosophers.o -o dp.exe

clean:
	rm *.o *.exe

.c.o:
	$(CC) -c $< -o $@
