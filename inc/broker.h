/******************************************************************************************
* broker.h je fajl koji sadrzi neophodne deklaracije funkcija, definicije makroa i
* preprocesorske instrukcije koje omogucavaju normalan rad boker niti. Broker
* prevashodno sluzi medjudrajverskoj komunikaciji i vodi racuna o sinhronizaciji
* medju nitima koje izvrsavaju drajvere. Broker je zasebna nit.
*
* Fajl je pisan koristeci biblioteke POSIX standarda.
*
* Kreator: Aleksa Markovic
********************************************************************************************/


#ifndef _BROKER_
#define _BROKER_

#include<stdio.h>
#include<malloc.h>
#include<stdint.h>
#include<string.h>
#include<pthread.h>
#include<sys/types.h>

// Definicije velicina nizova
#define BUF_SIZE 255
#define NAME_SIZE 32

/*
* Jednostruko uvezana lista koja sadrzi informacije
* potrebne za rad drajver-a. Lista sluzi komunikaciji
* na relaciji broker - drajver.
*/
typedef struct drivers_list
{
	int driver_id;			// Soket fajl deskriptor na kom drajver slusa (argument tred funkcije drajvera)
	pthread_t thread_id;		// Identifikacioni broj niti u kojoj se izvrsava drajver
	char name[ NAME_SIZE ];		// Ime drajvera
	char msg[ BUF_SIZE ];		// Poruk od brokera za drajver
	struct drivers_list* next;	// Pokazivac na sledeci element u listi
}_drivList;

/*
* Element red strukture koji sadrzi informacije
* potrebne brokeru kako bi prosledio poruku na
* odgovarajucu adresu. Sluzi komunikaciji na
* relaciji drajver - broker
*/
typedef struct driver_message
{
	int from;			// driver_id posiljaoca
	int to;				// driver_id primaoca
	char msg[ BUF_SIZE ];		// Poruka
	struct driver_message* next;	// Pokazivac na sledeci element reda
}_drivMsg;

/*
* Struktura koja sadrzi pokazivac na pocetak reda driver_message
* i velicinu tog reda.
*/
typedef struct broker_queue
{
        _drivMsg* head;
        unsigned int size;
}_brokerQ;

/*
* Ova funkcija inicijalizuje broker i strukture neophodne za njegov
* pravilan rad. Mjuteks je inicijalizuje u okviru ove funkcije.
* Izlaz:
*	int error -> logicka vrednost uspesnosti inicijalizacije mjuteks promenljive
*/
int broker_setup( void );

/*
* Ova funkcija gura novi element u broker red.
* Ulazni argumenti:
*	int from   -> driver_id posiljaoca
*	int to     -> driver_id primaoca
*	char msg[] -> poruka sa elementrima velicine 8bit
*/
void brokerQ_push( int from , int to , char msg[ ] );

/*
* Ova funkcija eliminise prvi element u broker redu i iscitava
* poruku, identifikacioni broj primaoca i posiljaoce
* Ulazni argumenti:
*	char* msg  -> adresa niza koji prima sadrzaj poruke, element niza je velicine 8bit
*	int* to    -> adresa promenljive u koju se iscitava identifikacioni broj posiljaoca
*	int* from  -> adresa promenljive u koju se iscitava identifikacioni broj primaoca
* Izlazni argumenti:
*	int	   -> velicina procitane poruke u bajtovima
*/
int brokerQ_pop( char* msg , int* from , int* to );

/*
* Ova funkcija proverava da li je broker red prazan.
* Izlazni argumenti:
*	uint8_t    -> Logicka vrednost 1 ili 0 koja odgovara nepraznom ili praznom redu respektivno
*/
uint8_t brokerQ_isempty( void );


/*
* Ova funkcija dodaje novi element u listu drajvera.
* Ulazni argumenti:
*	int driver_id 	    -> Fajl deskriptor soketa na kom je drajver konketovan na mrezu
*	pthread_t thread_id -> Identifikacioni broj niti koja izvrsava drajver
*	char name[]         -> Ime drajvera, string ili niz bajtova
*/
void drivList_add( int driver_id , pthread_t thread_id , char name[ ] );

/*
* Ova funkcija brise element iz liste drajvera.
* Ulazni argumenti:
*	char name[] -> Ime drajvera, string ili niz bajtova
* Izlazni argumenti:
*	pthread_t   -> Identifikacioni broj niti koja izvrsava drajver
*/
pthread_t drivList_del( char name[ ] );

/*
* Ova funkcija trazi odgovarajuci element liste drajvera prema imenu drajvera i vraca
* adresu na kojoj se element nalazi.
* Ulazni argumenti:
*	char name[] -> Ime drajvera, string ili niz bajtova
* Izlazni argumenti:
*	_drivList*  -> Adresa elementa liste sa trazenim imenom
*/
_drivList* drivList_find( char name[ ] );

// Broker red promenljiva. Sadrzi poruke poslate od drajvera prema brokeru
extern _brokerQ broker;
// Drajver lista sadrzi informacije o aktivnim drajverima i poruke od brokera prema drajveru
extern _drivList* drivers_list;

#endif //BROKER


