#include "broker.h"

_brokerQ broker;
_drivList driver_list;

void brokerQ_push( int from , int to , char msg[ ] )
{
	if( NULL == broker.head )
	{
		broker.head = ( _drivMsg* )calloc( 1 , sizeof( _drivMsg ) );
		broker.head->to = to;
		strcpy( broker.head->msg , msg );
		broker.head->next = NULL;
		broker.size++;
		return;
	}

	_drivMsg* tmp = broker.head;
	while( NULL != tmp->next )
	{
		tmp = tmp->next;
	}
	tmp->next = ( _drivMsg* )calloc( 1 , sizeof( _drivMsg ) );
	tmp->next->to = to;
	strcpy( tmp->next->msg , msg );
	tmp->next->next = NULL;
	broker.size++;
	return;

}

int brokerQ_pop( char* msg )
{
	 if( 0 == broker.size )
                return 0;

	strcpy( msg , broker.head->msg );
	_drivMsg* next = broker.head->next;
        free( broker.head );
        broker.head = next;
        broker.size--;

	return (int)strlen( msg );
}

uint8_t brokerQ_isempty( void )
{
	return ( 0 == broker.size );
}


