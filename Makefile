CC = g++
CFLAGS = -c

all: program

program: solver.o
	$(CC) -o program solver.o

solver.o: solver.cpp
	$(CC) $(CFLAGS) solver.cpp

run:
	./program

clean:
	rm -f *.o program