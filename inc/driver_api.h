#ifndef _DRIVER_API_
#define _DRIVER_API_

#include "broker.h"
#include "tcp_server.h"

void drivAPI_init_driver( void* vargp , char driv_name[ ] );
void drivAPI_send_broker( char to_name[ ] , char msg[ ] );
int drivAPI_read_broker( char msg[ ] );
int drivAPI_get_drivID( char name[ ] );

void drivAPI_send_network( char msg[ ] );
void drivAPI_read_network( char msg[ ] );

#endif // DRIVER_API
