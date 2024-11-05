CFLAGS = -Wall -Wextra -g -std=c99 # flags de compilacao
CC = gcc

all: cli.o algoritmos.o
	$(CC) -o cli cli.o algoritmos.o

algoritmos.o: algoritmos.c algoritmos.h
	$(CC) -c $(CFLAGS) algoritmos.c

cli.o: cli.c algoritmos.h
	$(CC) -c $(CFLAGS) cli.c

clean:
	rm -f *.o *~ cli
