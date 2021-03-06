#include "broker.h"

_brokerQ broker;
_drivList* drivers_list;
pthread_t* broker_thread;

// Mjuteks promenljiva koja sinhronizuje pristup broker redu
static pthread_mutex_t lockQ;
// Mjuteks promenljiva koja sinhronizuje pristup drajver listi
static pthread_mutex_t lockList;

int broker_setup( void )
{
	if ( 0 != pthread_mutex_init( &lockQ , NULL ) )
	{
		printf( "\n Mutex init failed\n" );
		return -1;
	}

	if ( 0 != pthread_mutex_init( &lockList , NULL ) )
	{
		printf( "\n Mutex init failed\n" );
		return -1;
	}

	return 1;
}

void brokerQ_push( int from , int to , char msg[ ] )
{
	pthread_mutex_lock( &lockQ );

	if( NULL == broker.head )
	{
		broker.head = ( _drivMsg* )calloc( 1 , sizeof( _drivMsg ) );
		broker.head->to = to;
		broker.head->from = from;
		strcpy( broker.head->msg , msg );
		broker.head->next = NULL;
		broker.size++;

		pthread_mutex_unlock( &lockQ );

		return;
	}

	_drivMsg* tmp = broker.head;
	while( NULL != tmp->next )
	{
		tmp = tmp->next;
	}
	tmp->next = ( _drivMsg* )calloc( 1 , sizeof( _drivMsg ) );
	tmp->next->to = to;
	tmp->next->from = from;
	strcpy( tmp->next->msg , msg );
	tmp->next->next = NULL;
	broker.size++;

	pthread_mutex_unlock( &lockQ );

	return;
}

int brokerQ_pop( char* msg , int* from , int* to )
{

	if( 0 == broker.size )
		return 0;

	pthread_mutex_lock( &lockQ );

	sprintf( msg , "%s" , broker.head->msg );
	*from = broker.head->from;
	*to = broker.head->to;
	_drivMsg* next = broker.head->next;
	free( broker.head );
	broker.head = next;
	broker.size--;

	pthread_mutex_unlock( &lockQ );

	return (int)strlen( msg );
}

uint8_t brokerQ_isempty( void )
{
	return ( 0 == broker.size );
}

void drivList_add( int driver_id , pthread_t thread_id , char name[ ] )
{
	pthread_mutex_lock( &lockList );

	if( NULL == drivers_list )
	{
		_drivList* tmp = (_drivList*)calloc( 1 , sizeof( _drivList ) );
		tmp->driver_id = driver_id;
		tmp->thread_id = thread_id;
		strcpy( tmp->name , name );
		tmp->next = drivers_list;
		drivers_list = tmp;

		pthread_mutex_unlock( &lockList );

		return;
	}

	_drivList* tmp = drivers_list;
	while(NULL != tmp->next)
		tmp = tmp->next;

	tmp->next = (_drivList*)calloc( 1 , sizeof( _drivList ) );
	tmp->next->driver_id = driver_id;
	tmp->thread_id = thread_id;
	strcpy( tmp->next->name , name );

	pthread_mutex_unlock( &lockList );

	return;
}

pthread_t drivList_del( char name[ ] )
{
	pthread_mutex_lock( &lockList );

	_drivList* _c = drivers_list;
	_drivList* _p = NULL;
	int driver_id = 0;

	if( NULL == drivers_list )
	{
		pthread_mutex_unlock( &lockList );
		return 0;
	}

	while( 0 != strcmp( _c->name , name ) )
	{
		if( NULL == _c->next )
		{
			pthread_mutex_unlock( &lockList );
			return 0;
		}
		_p = _c;
		_c = _c->next;
	}

	if( _c == drivers_list )
		drivers_list = _c->next;
	else
		_p->next = _c->next;

	driver_id = _c->driver_id;
	pthread_mutex_unlock( &lockList );

	return driver_id;

}

_drivList* drivList_find( char name[ ] )
{
	pthread_mutex_lock( &lockList );

	_drivList* tmp = drivers_list;

	if( NULL == tmp )
	{
		pthread_mutex_unlock( &lockList );
		return NULL;
	}

	while( 0 != strcmp( tmp->name , name ) )
	{
		if( NULL == tmp->next )
		{
			pthread_mutex_unlock( &lockList );
			return NULL;
		}
		tmp = tmp->next;
	}

	pthread_mutex_unlock( &lockList );

	return tmp;
}

