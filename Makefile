CC = gcc
CFLAGS = -Wall -pedantic -std=c99 $(shell pkg-config --cflags webkit2gtk-3.0 gtk+-3.0 sqlite3)
LDFLAGS = $(shell pkg-config --libs webkit2gtk-3.0 gtk+-3.0 sqlite3)

all: swb

swb: swb.c swb.h config.h
	$(CC) $(CFLAGS) swb.c -o swb $(LDFLAGS) 

debug: swb.c swb.h config.h
	$(CC) -g $(CFLAGS) swb.c -o swb $(LDFLAGS) 


clean:
	rm -f swb
