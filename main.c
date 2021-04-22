#include<stdio.h>

#include "tcp_server.h"

char myIP[ MAX_IP_SIZE ];

int main(int argc, char* argv[])
{
	int newSocket[ NUM_OF_DEVICES ];
        struct sockaddr_in newAddr[ NUM_OF_DEVICES ];

	getIP( myIP , LOG_ON );

	return 0;
}
