CFLAGS=-Wall -g
# <3 GCC
POSTFLAGS=-lm
PROG=nap

all:
	$(CC) $(CFLAGS) $(PROG).c $(POSTFLAGS) -o $(PROG)

test:
	echo "tests do not exist yet"
