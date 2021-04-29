CC = gcc
CFLAGS = -g -Wall -o
LIBS = -lm -lpthread

CFILES = ./src/tcp_server.c ./src/broker.c ./driv/test.c ./driv/gen_alg.c ./src/driver_api.c main.c
HDIR = -I./inc/ -I./driv/
OUTFILE = main

all: app

app:
	$(CC) $(CFLAGS) $(OUTFILE) $(CFILES) $(LIBS) $(HDIR)

run:
	 systemd-run --scope -p CPUQuota=100% ./main

clear:
	rm *.o
