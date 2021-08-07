#include "driver_api.h"

char* drivAPI_set_name( char driv_class[ ] )
{
	char* name = (char*)calloc( NAME_SIZE , sizeof( char ) );
	printf( "Input name for driver from class: %s \r\n" , driv_class );
	scanf( "%s" , name );

	return name;
}

int* drivAPI_init_driver( void* vargp , char* my_name )
{
	int* conSocket = (int*)calloc( 1 , sizeof( int ) );
	*conSocket = *( (int*)vargp );
	drivList_add( *conSocket , pthread_self( ) , my_name );

	return conSocket;
}

void drivAPI_send_broker( int conSocket , char to_name[ ] , char msg[ ] )
{
	brokerQ_push( conSocket , drivAPI_get_drivID( to_name ) , msg );
	return;
}

int drivAPI_read_broker( char msg[ ] , char* my_name )
{
	_drivList* tmp = drivList_find( my_name );
	strcpy( msg , (*tmp).msg );
	memset( (*tmp).msg , 0 , sizeof( BUF_SIZE ) );

	return (*tmp).driver_id;
}

int drivAPI_get_drivID( char name[ ] )
{
	_drivList* tmp = drivList_find( name );
	return (*tmp).driver_id;
}

void drivAPI_send_network( int conSocket , char msg[ ] )
{
	tcp_server_send( conSocket , msg );
	return;
}

void drivAPI_read_network( int conSocket , char msg[ ] )
{
	tcp_server_recv( conSocket , msg );
	return;
}

unsigned int start_timer( unsigned long int us , unsigned long int sec , void (*handler)(int) )
{
	signal( SIGALRM , handler );
	return alarm_us( us , sec );
}

unsigned int alarm_us( unsigned long int us , unsigned long int sec )
{
	struct itimerval old, new;
	new.it_interval.tv_usec = 0;
  	new.it_interval.tv_sec = 0;
  	new.it_value.tv_usec = us;
  	new.it_value.tv_sec = sec;
  	if (setitimer (ITIMER_REAL, &new, &old) < 0)
    		return 0;
  	else
    		return old.it_value.tv_sec;
}


