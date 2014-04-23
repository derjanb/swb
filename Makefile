CC = gcc
CFLAGS = -Wall -std=c99 $(shell pkg-config --cflags webkit2gtk-3.0 gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs webkit2gtk-3.0 gtk+-3.0)

all: swb

swb: swb.c swb.h
	$(CC) $(CFLAGS) swb.c -o swb $(LDFLAGS) 

debug: swb.c swb.h
	$(CC) -g $(CFLAGS) dwb.c -o swb $(LDFLAGS) 


clean:
	rm -f swb
