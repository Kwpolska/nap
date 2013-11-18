CFLAGS=-Wall -g -lm
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)

test:
	echo "tests do not exist yet"
