CFLAGS=-Wall -g -lm
CC=clang
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)
