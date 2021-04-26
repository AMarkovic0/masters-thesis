#ifndef _DRIVER_API_
#define _DRIVER_API_

#include "broker.h"
#include "tcp_server.h"

#define DRIV_CLASS 		"GEN_ALG"

char* drivAPI_set_name( char driv_class[ ] );
int* drivAPI_init_driver( void* vargp , char* my_name );
void drivAPI_send_broker( int conSocket , char to_name[ ] , char msg[ ] );
int drivAPI_read_broker( char msg[ ] , char* my_name );
int drivAPI_get_drivID( char name[ ] );

void drivAPI_send_network( int conSocket , char msg[ ] );
void drivAPI_read_network( int conSocket , char msg[ ] );

#endif // DRIVER_API
