#include<pthread.h>
#include<sys/time.h>
#include <sys/wait.h>

#include "drivers.h"

#define PORT		54321u

char myIP[ MAX_IP_SIZE ];

void* broker_exec( void* vargp );

pthread_t thread_id[ NUM_OF_DEVICES ];
int newSocket[ NUM_OF_DEVICES ];
struct sockaddr_in newAddr[ NUM_OF_DEVICES ];

int main( int argc , char* argv[ ] )
{
	getIP( myIP , LOG_ON );
	broker_setup( );

	tcp_server_init( myIP , PORT , LOG_ON );
	tcp_server_listen( LOG_ON );

	pthread_create( &thread_id[ 0 ] , NULL , &broker_exec , NULL );

	for( int i = 1 ; i < NUM_OF_DEVICES ; i++ )
	{
		tcp_server_accept( &newSocket[ i ] , &newAddr[ i ] , LOG_ON );
		pthread_create( &thread_id[ i ] , NULL , &gen_alg_driv_exec , (void*)&newSocket[ i ] );
		printf( "THREAD ID : %lu \r\n" , (uint64_t)thread_id[ i ] );
		sleep( 0.4 );
	}

	for( int i = 0 ; i < NUM_OF_DEVICES ; i++ )
		pthread_join( thread_id[ i ] , NULL );

	return 0;
}

void* broker_exec( void* vargp )
{
	char rmsg[ BUF_SIZE ] = { '\0' };
	_drivList driver;
	while( 1 )
	{
		if( !brokerQ_isempty( ) )
		{
			brokerQ_pop( rmsg );
			if( 0 != rmsg[ 0 ] )
			{
				printf( "%s" , rmsg );
				memset( rmsg, 0 , sizeof( rmsg ) );
				// Parse XML/JSON
			}
			else
			{
				driver = drivList_find( "D1" );
				pthread_kill( thread_id[ 1 ] , 99 );
			}
			sleep( 1 );
		}
	}

	return NULL;
}
