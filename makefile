CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99
LFLAGS = -Wall -pedantic -std=gnu99

all: thing series

clean:
	rm -f *.o

thing: thing.o
	$(CC) $(CFLAGS) thing.o -o thing

thing.o: thing.c
	$(CC) $(LFLAGS) -c thing.c -o thing.o


series: series.o
	$(CC) $(CFLAGS) series.o -o series

series.o: series.c
	$(CC) $(LFLAGS) -c series.c -o series.o


