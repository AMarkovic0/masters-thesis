#ifndef _BROKER_
#define _BROKER_

#include<stdio.h>
#include<malloc.h>
#include<stdint.h>
#include<string.h>

#define BUF_SIZE 255

typedef enum driver_states{ BORN , HANDSHAKE , INIT , READY , CLOSED }drivstat_t;

typedef struct drivers_list
{
	int thread_id;
	char* name;
	char* msg;
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

void brokerQ_push( int from , int to , char msg[ ] );
int brokerQ_pop( char* msg );
uint8_t brokerQ_isempty( void );

extern _brokerQ broker;
extern _drivList driver_list;

#endif //BROKER


