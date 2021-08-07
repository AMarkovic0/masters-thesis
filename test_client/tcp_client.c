#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>

//Networking headers:
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<sys/ioctl.h>

#define BUF_SIZE 1024
#define PORT 54321u

#define LOG_ON 0x01
#define LOG_OFF 0x00

uint8_t tcp_client_init( char* ip , unsigned int port , uint8_t log );
uint8_t tcp_client_connect( uint8_t log );
unsigned int alarm_us( unsigned int ms );
void heartbeat_handler( int sig );

int clientSocket;
struct sockaddr_in serverAddr;

uint8_t buf[BUF_SIZE] = { 0 };
uint8_t id[ 5 ] = { 0 };

int main( int argc , char* argv[] )
{
	tcp_client_init( "192.168.0.129" , PORT , LOG_ON );
	tcp_client_connect( LOG_ON );
	signal( SIGALRM , heartbeat_handler );
	alarm_us( 100 );

	char msg[ 32 ] = { '\0' };
	while( 1 )
	{
		printf( "Enter: \r\n" );
		scanf( "%s" , msg );
		sleep(0.1);
		send( clientSocket , msg , strlen( msg ) , 0 );
		recv( clientSocket , buf , BUF_SIZE , MSG_DONTWAIT );
		printf( "%s \r\n" , buf );
		memset(buf, 0, BUF_SIZE);
	}

	return 0;
}

void heartbeat_handler( int sig )
{
	recv( clientSocket , buf , BUF_SIZE , MSG_DONTWAIT );
	if ( buf[ 0 ] )
		printf( "%s \r\n" , buf );
	memset(buf, 0, BUF_SIZE);

	alarm_us( 100 );
}

unsigned int alarm_us( unsigned int ms )
{
	struct itimerval old, new;
	new.it_interval.tv_usec = 0;
  	new.it_interval.tv_sec = 0;
  	new.it_value.tv_usec = ( long int )ms;
  	new.it_value.tv_sec = 0;
  	if (setitimer (ITIMER_REAL, &new, &old) < 0)
    		return 0;
  	else
    		return old.it_value.tv_sec;
}

uint8_t tcp_client_init( char* ip , unsigned int port , uint8_t log )
{
	clientSocket = socket( PF_INET , SOCK_STREAM , 0 );

	if( (-1 == clientSocket) && log )
	{
		printf( "Socket creation failed. Return -1. \r\n" );
		return -1;
	}
	else if ( (-1 == clientSocket) && !log)
	{
		return -1;
	}
	else if ( log )
		printf( "Sockret creation successful. \r\n" );

	memset( &serverAddr , 0 , sizeof( serverAddr ) );

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( port );
	serverAddr.sin_addr.s_addr = inet_addr( ip );

	return 1;
}

uint8_t tcp_client_connect( uint8_t log )
{
	if( connect( clientSocket , (struct sockaddr*)&serverAddr , sizeof(serverAddr) ) && log )
	{
		printf( "Connection with server %s failed. Return -1. \r\n" , inet_ntoa(serverAddr.sin_addr) );
		return -1;
	}
	else if ( connect( clientSocket , (struct sockaddr*)&serverAddr , sizeof(serverAddr) ) && !log)
		return -1;
	else if ( log )
		printf( "Connected to the server. \r\n" );

	return 1;
}
