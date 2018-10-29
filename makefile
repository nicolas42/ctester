CC = gcc
CFLAGS = -Wall -pedantic -Werror -std=gnu99
LFLAGS = -Wall -pedantic -Werror -std=gnu99

all: thing

clean:
	rm -f *.o

thing: thing.o
	$(CC) $(CFLAGS) thing.o -o thing

thing.o: thing.c
	$(CC) $(LFLAGS) -c thing.c -o thing.o


