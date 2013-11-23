CFLAGS=-Wall -g -DDEBUG
# <3 GCC
POSTFLAGS=-lm
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c $(POSTFLAGS) -o $(PROG)

test:
	$(CC) $(CFLAGS) test-nap.c $(POSTFLAGS) -o test-nap
	./test-nap
