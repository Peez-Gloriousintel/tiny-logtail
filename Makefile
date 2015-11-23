CC=clang
CFLAGS=-g -Wall
EXECUTE=tiny-logtail

all: tiny_logtail.o
	$(CC) $(CFLAGS) $< -o $(EXECUTE)

tiny_logtail.o: tiny_logtail.c tiny_logtail.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f tiny-logtail tiny_logtail.o
