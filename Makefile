CC = gcc
CFLAGS = -Wall -pedantic -std=c99 $(shell pkg-config --cflags webkit2gtk-3.0 gtk+-3.0 sqlite3)
LDFLAGS = $(shell pkg-config --libs webkit2gtk-3.0 gtk+-3.0 sqlite3)

all: swb

swb: swb.o helpers.o handlers.o
	$(CC) $(CFLAGS) handlers.o swb.o helpers.o -o swb $(LDFLAGS) 

swb.o: swb.c swb.h config.h 
	$(CC) $(CFLAGS) -c swb.c -o swb.o 

helpers.o: helpers.c helpers.h config.h
	$(CC) $(CFLAGS) -c helpers.c -o helpers.o 

handlers.o: handlers.c handlers.h config.h
	$(CC) $(CFLAGS) -c handlers.c -o handlers.o 

clean:
	rm -f swb *.o
