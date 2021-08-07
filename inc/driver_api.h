/******************************************************************************
*
* driver_api.h je fajl koji sadrzi neophodne deklaracije funkcija,
* definicije makroa i preprocesorske instrukcije koje omogucavaju razvoj
* upravljackih delova programa - drajvera.
*
* Fajl je pisan koristeci biblioteke POSIX standarda.
*
* Kreator: Aleksa Markovic
******************************************************************************/

#ifndef _DRIVER_API_
#define _DRIVER_API_

#include<sys/time.h>

// Uvoz neophodnih nestandardnih biblioteka
#include "broker.h"	// Sadrzi definicije i deklaracije neophodne za rad brokera
#include "tcp_server.h" // Sadrzi definicije i deklaracije neophodne za TCP komunikaciju

/*
* Funkcija za unos imena drajvera sa komandne linije
* Ulazni argumenti:
*	char driv_class[ ]	-> String koji sadrzi klasu drjavera
* Izlazni argumenti:
*	char* name		-> Ime drajvera
*/
char* drivAPI_set_name( char driv_class[ ] );

/*
* Ova funkcija inicijalizuje drajver - brokerQ i driverList
* Ulazni argumenti:
*	void* vargp		-> ID konekcije - ulazni argumenti funkcije za
*				   pokretanje niti
*	char* my_name		-> Ime drajvera
* Izlazni argumenti:
*	uint8_t id		-> Identifikacioni broj drajvera (soket id)
*/
int* drivAPI_init_driver( void* vargp , char* my_name );

/*
* Ova funkcija salje poruku brokeru.
* Ulazni argumenti:
*	int conSocket		-> Soket id konekcije sa klijentom
*	char to_name[]		-> Ime drajvera koji prima poruku
*	char msg[]		-> Sadrzaj poruke
*/
void drivAPI_send_broker( int conSocket , char to_name[ ] , char msg[ ] );

/*
* Ova funkcija cita poruku poslatu od brokera.
* Ulazni argumenti:
*	char msg[]		-> Sadrzaj poruke
*	char my_name[]		-> Ime drajvera
*/
int drivAPI_read_broker( char msg[ ] , char* my_name );

/*
* Ova funkcija vraca ID drajvera - soket id konekcije sa klijentom.
* Ulazni argumenti:
*	char name[]		-> Ime trazenog drajvera
* Izlazni argument:
*	int id			-> Id trazenog drajvera
*/
int drivAPI_get_drivID( char name[ ] );

/*
* Ova funkcija salje poruku konektovanom uredjaju.
* Ulazni argumenti:
*	int conSocket		-> Soket id konekcije sa klijentom
*	char msg[]		-> Sadrzaj poruke
*/
void drivAPI_send_network( int conSocket , char msg[ ] );

/*
* Ova funkcija cita poruku od konektovanog uredjaja.
* Ulazni argumenti:
*	int conSocket		-> Soket id konekcije sa klijentom
*	char msg[]		-> Niz koji cuva sadrzaj primljene poruke
*/
void drivAPI_read_network( int conSocket , char msg[ ] );

/*
* Ova funkcija postavlja casovnik koji poziva handler funkciju nakon
* isteka vremena sec.usec.
* Ulazni argumenti:
*	unsinged long int us	-> vreme u us
*	unsinged long int sec	-> vreme us sekundama
*	void (*fun)(int)	-> adresa callback funckije
*	boo repeat		-> postavlja timer ponovo ili ne
*/
unsigned int start_timer( unsigned long int us , unsigned long int sec , void (*handler)(int) );

/*
* Ova funkcija postavlja alarm na ms mikrosekundi. Pozvati ovu funkciju
* u hendleru za ponovno postavljanje alarma.
* Ulazni argumenti:
*	unsigned int ms		-> broj mikrosekundi
* Izlazni argumenti:
*	unsigned int		-> 0 u slucaju uspeha, staro vreme u slucaju neuspeha
*/
unsigned int alarm_us( unsigned long int us , unsigned long int sec );

#endif // DRIVER_API
