CC = g++
CFLAGS = -c

all: program

program: solver.o
	$(CC) -o program solver.o

solver.o: solver.cpp
	$(CC) $(CFLAGS) solver.cpp

clean:
	rm -f *.o program \
	./tests/output/test_LPs/* ./tests/output/test_LPs_volume2/*