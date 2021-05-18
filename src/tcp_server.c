#include "tcp_server.h"

static int sockfd;
static struct sockaddr_in serverAddr;

uint8_t tcp_server_init( char* ip , unsigned int port , _logs log )
{
	uint8_t check_var = 0;

	signal( SIGPIPE , SIG_IGN ); // block SIGPIPE signal in case client disconnect

	sockfd = socket( PF_INET , SOCK_STREAM , 0 );
	if( log && ( -1 == sockfd ) )
	{
		printf( "Socket creation failed. \r\n" );
		return -1;
	}
	else if( !log && ( -1 == sockfd ) )
	{
		return -1;
	}
	else if( log )
		printf( "Socket creation sucessfull. \r\n" );

	memset( &serverAddr , '\0' , sizeof( serverAddr ) );

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( port );
	serverAddr.sin_addr.s_addr = inet_addr( ip );

	check_var = bind( sockfd , ( struct sockaddr* )&serverAddr , sizeof( serverAddr ) );
	if( check_var && log )
	{
		printf( "Bind failed %d. \r\n" , errno );
		return -1;
	}
	else if( check_var && !log )
	{
		return -1;
	}
	else if( !check_var && log )
		printf( "Bind sucessfull \r\n" );

	return -1;
}

uint8_t tcp_server_listen( _logs log )
{
	uint8_t check = listen( sockfd , NUM_OF_DEVICES );
	if( check && log )
	{
		printf( "Listen failed. \r\n" );
		return -1;
	}
	else if( check && !log )
	{
		return -1;
	}
	else if( !check && log )
		printf( "Server listening... \r\n" );

	return 1;
}

uint8_t tcp_server_accept( int* newSocket , struct sockaddr_in* newAddr , _logs log )
{
	socklen_t addr_size = sizeof( newAddr );

	*newSocket = accept( sockfd , ( struct sockaddr* )newAddr , &addr_size );
	if( ( *newSocket < 0 ) && log )
	{
		printf( "Acception failed. \r\n" );
		return -1;
	}
	else if( ( *newSocket < 0 ) && !log )
	{
		return -1;
	}
	else if( log )
		printf( "Client sucessfully accepted. \r\n" );

	return 1;
}

uint8_t tcp_server_send( int sockfd , char* w_buf )
{
	return send( sockfd , w_buf , strlen( w_buf ) , 0 );
}

uint8_t tcp_server_recv( int sockfd , char* r_buf )
{
	return recv( sockfd , r_buf , BUF_SIZE , MSG_DONTWAIT );
}

void getIP( char* IPaddr , _logs log )
{
	int fd;
	struct ifreq ifr;

	fd = socket( AF_INET , SOCK_DGRAM , 0 );
	ifr.ifr_addr.sa_family = AF_INET;
	memcpy( ifr.ifr_name , WIFI_INTERFACE , IFNAMSIZ-1 );
	ioctl( fd , SIOCGIFADDR , &ifr );

	close( fd );

	strcpy( IPaddr , inet_ntoa( ( ( struct sockaddr_in * )&ifr.ifr_addr )->sin_addr ) );

	if( log )
		printf("Host IP Address is: %s\n", IPaddr );

	return;
}

