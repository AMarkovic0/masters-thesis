#include "driver_api.h"

static int conSocket;
static char my_name[ NAME_SIZE ] = { '\0' };

void drivAPI_init_driver( void* vargp , char driv_name[ ] )
{
	conSocket = *( (int*)vargp );
	strcpy( my_name , driv_name );
	drivList_add( conSocket , my_name );

	return;
}

void drivAPI_send_broker( char to_name[ ] , char msg[ ] )
{
	brokerQ_push( conSocket , drivAPI_get_drivID( to_name ) , msg );
	return;
}

int drivAPI_read_broker( char msg[ ] )
{
	_drivList tmp = drivList_find( my_name );
	strcpy( msg , tmp.msg );

	return tmp.driver_id;
}

int drivAPI_get_drivID( char name[ ] )
{
	_drivList tmp = drivList_find( my_name );
	return tmp.driver_id;
}

void drivAPI_send_network( char msg[ ] )
{
	tcp_server_send( conSocket , msg );
}

void drivAPI_read_network( char msg[ ] )
{
	tcp_server_recv( conSocket , msg );
}
