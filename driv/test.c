#include "test.h"

void* test_driv_exec( void* vargp )
{
	char* my_name = drivAPI_set_name( "TEST" );
        int* conSocket = drivAPI_init_driver( vargp , my_name );
        char read_buffer[ BUF_SIZE ] = { '\0' };
        char write_buffer[ BUF_SIZE ] = { '\0' };
	char wbuf_broker[ BUF_SIZE ] = { '\0' };
	char rbuf_broker[ BUF_SIZE ] = { '\0' };

        while( 1 )
        {
                drivAPI_read_network( *conSocket , read_buffer );
		if( 0 == strcmp( read_buffer , "START" ) )
		{
			printf( "D2 Received: %s \r\n" , read_buffer );
			drivAPI_send_broker( *conSocket , "D1" , "RETURN80" );
		}
		else if( 0 == strcmp( read_buffer , "END" ) )
		{
			printf( "D2 Received: %s \r\n" , read_buffer );
			drivAPI_send_broker( *conSocket , "D1" , "TURN80" );
		}
		else
		{
		}

		drivAPI_read_broker( rbuf_broker , my_name );
		if( 0 == rbuf_broker[ 0 ] )
		{
		}
		else
		{
		}

		memset( read_buffer , 0 , sizeof( read_buffer ) );
                memset( write_buffer , 0 , sizeof( write_buffer ) );
                memset( rbuf_broker , 0 , sizeof( rbuf_broker ) );
                memset( wbuf_broker , 0 , sizeof( wbuf_broker ) );
	}

	return NULL;
}

