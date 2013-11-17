CFLAGS=-Wall -g
CC=clang
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)
