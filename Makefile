
CFLAGS=-Wall

all: fatattr

fatattr: fatattr.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -f fatattr
