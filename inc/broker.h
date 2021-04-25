#ifndef _BROKER_
#define _BROKER_

#include<stdio.h>
#include<malloc.h>
#include<stdint.h>
#include<string.h>
#include<pthread.h>

#define BUF_SIZE 255
#define NAME_SIZE 32

typedef enum driver_states{ BORN , HANDSHAKE , INIT , READY , CLOSED }drivstat_t;

typedef struct drivers_list
{
	int driver_id;
	char name[ NAME_SIZE ];
	char msg[ BUF_SIZE ];
	struct drivers_list* next;
}_drivList;

typedef struct driver_message
{
	int from;
	int to;
	char msg[ BUF_SIZE ];
	struct driver_message* next;
}_drivMsg;

typedef struct broker_queue
{
        _drivMsg* head;
        unsigned int size;
}_brokerQ;

int broker_setup( void );

void brokerQ_push( int from , int to , char msg[ ] );
int brokerQ_pop( char* msg );
uint8_t brokerQ_isempty( void );

void drivList_add( int driver_id , char name[ ] );
pthread_t drivList_del( char name[ ] );
_drivList drivList_find( char name[ ] );

extern pthread_mutex_t lockQ;
extern pthread_mutex_t lockList;

extern _brokerQ broker;
extern _drivList driver_list;

extern _drivList* drivers_list;

#endif //BROKER


