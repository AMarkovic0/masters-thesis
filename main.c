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

	tcp_server_accept( &newSocket[ 1 ] , &newAddr[ 1 ] , LOG_ON );
	pthread_create( &thread_id[ 1 ] , NULL , &gen_alg_driv_exec , (void*)&newSocket[ 1 ] );

	sleep( 3 );

	tcp_server_accept( &newSocket[ 2 ] , &newAddr[ 2 ] , LOG_ON );
	pthread_create( &thread_id[ 2 ] , NULL , &test_driv_exec , (void*)&newSocket[ 2 ] );

	sleep( 3 );

	for( int i = 0 ; i < NUM_OF_DEVICES ; i++ )
		pthread_join( thread_id[ i ] , NULL );

	return 0;
}

void* broker_exec( void* vargp )
{
	char rmsg[ BUF_SIZE ] = { '\0' };
	int from = 0;
	int to = 0;
	_drivList* list = drivers_list;

	signal( SIGUSR1 , gen_alg_driv_handler );

	while( 1 )
	{
		if( !brokerQ_isempty( ) )
		{
			brokerQ_pop( rmsg , &from , &to );
			if( 0 != rmsg[ 0 ] )
			{
				printf( "%s %d %d \r\n" , rmsg , from , to );
				while( NULL != list )
				{
					if( list->driver_id != to )
					{
						printf( "Listing: %s \r\n" , list->name );
						list = list->next;
					}
					else
					{
						break;
					}
				}

				if( NULL != list )
				{
					strcat( list->msg , rmsg );
					pthread_kill( thread_id[ 1 ] , SIGUSR1 );
				}
				list = drivers_list;
				memset( rmsg, 0 , sizeof( rmsg ) );
				from = 0;
				to = 0;
			}
			sleep( 1 );
		}
	}

	return NULL;
}

