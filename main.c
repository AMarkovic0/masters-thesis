#include<pthread.h>
#include<sys/time.h>
#include <sys/wait.h>

#include "tcp_server.h"
#include "broker.h"

#include "drivers.h"

#define PORT		54321u

pthread_mutex_t lock;
char myIP[ MAX_IP_SIZE ];

void* broker_exec( void* vargp );

int main( int argc , char* argv[ ] )
{
	pthread_t thread_id[ NUM_OF_DEVICES ];
	int newSocket[ NUM_OF_DEVICES ];
	struct sockaddr_in newAddr[ NUM_OF_DEVICES ];

	getIP( myIP , LOG_ON );
	if ( 0 != pthread_mutex_init( &lock , NULL ) )
	{
		printf( "\n Mutex init failed\n" );
		return -1;
	}

	tcp_server_init( myIP , PORT , LOG_ON );
	tcp_server_listen( LOG_ON );

	pthread_create( &thread_id[ 0 ] , NULL , &broker_exec , NULL );
	pthread_create( &thread_id[ 1 ] , NULL , &gen_alg_driv_exec , NULL );

	for( int dev_cnt = 0 ; dev_cnt < NUM_OF_DEVICES ; dev_cnt++ )
	{

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
			pthread_mutex_lock( &lock );
			brokerQ_pop( msg );
			pthread_mutex_unlock( &lock );

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
