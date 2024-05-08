CC = gcc
CFLAGS = -Wall -g -std=c11
LIBS = -lpthread

all: A2checker

A2checker: main.o
	$(CC) $(CFLAGS) -o A2checker main.o $(LIBS)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
clean:
	rm -f A2Checker *.o