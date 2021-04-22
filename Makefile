CC = gcc
CFLAGS = -g -Wall -o
LIBS = -lm

CFILES = ./src/tcp_server.c main.c
HDIR = ./inc/
OUTFILE = main

all: app

app:
	$(CC) $(CFLAGS) $(OUTFILE) $(CFILES) $(LIBS) -I$(HDIR).

clear:
	rm *.o
