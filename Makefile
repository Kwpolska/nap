CFLAGS=-Wall -pedantic -g -DDEBUG -std=c99 -D_POSIX_C_SOURCE=200809L
# <3 GCC
POSTFLAGS=-lm
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c $(POSTFLAGS) -o $(PROG)

test:
	$(CC) $(CFLAGS) test-nap.c $(POSTFLAGS) -o test-nap
	./test-nap
