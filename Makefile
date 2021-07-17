CC = g++
CFLAGS = -c

all: program

program: solver.o
	$(CC) -o program solver.o

solver.o: solver.cpp
	$(CC) $(CFLAGS) solver.cpp

clean:
	rm -f *.o program \
	./output/test_LPs/* ./output/test_LPs_volume2/*