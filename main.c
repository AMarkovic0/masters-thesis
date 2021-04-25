#include<pthread.h>
#include<sys/time.h>
#include <sys/wait.h>

#include "tcp_server.h"
#include "broker.h"

#include "drivers.h"

#define PORT		54321u

char myIP[ MAX_IP_SIZE ];

void* broker_exec( void* vargp );

int main( int argc , char* argv[ ] )
{
	pthread_t thread_id[ NUM_OF_DEVICES ];
	int newSocket[ NUM_OF_DEVICES ];
	struct sockaddr_in newAddr[ NUM_OF_DEVICES ];

	getIP( myIP , LOG_ON );
	broker_setup( );

	tcp_server_init( myIP , PORT , LOG_ON );
	tcp_server_listen( LOG_ON );

	pthread_create( &thread_id[ 0 ] , NULL , &broker_exec , NULL );

	char str[100];
	for( int i = 0 ; i < NUM_OF_DEVICES ; i++ )
	{
		tcp_server_accept( &newSocket[ i ] , &newAddr[ i ] , LOG_ON );
		printf( "Client Adress = %s \r\n",
			inet_ntop(AF_INET,&newAddr[i].sin_addr,str,sizeof(str)) );
		pthread_create( &thread_id[ 1 ] , NULL , &gen_alg_driv_exec , (void*)&newSocket[ i ] );
		sleep( 0.4 );
	}

	pthread_join( thread_id[ 0 ] , NULL );
	pthread_join( thread_id[ 1 ] , NULL );


	return 0;
}

void* broker_exec( void* vargp )
{
	char msg[ BUF_SIZE ] = { '\0' };

	while( 1 )
	{
		if( !brokerQ_isempty( ) )
		{
			brokerQ_pop( msg );
			if( 0 != msg[ 0 ] )
			{
				printf( "%s" , msg );
				memset( msg, 0 , sizeof( msg ) );
				// Parse XML/JSON
			}
		}
	}

	return NULL;
}
