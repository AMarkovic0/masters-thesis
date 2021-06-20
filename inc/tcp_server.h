/******************************************************************************
*
* tcp_server.h je fajl koji sadrzi neophodne deklaracije funkcija,
* definicije makroa i preprocesorske instrukcije koje omogucavaju rad servera.
* Ovaj fajl implementira TCP server nezavisno od specificnosti aplikacije.
*
* Fajl je pisan koristeci biblioteke POSIX standarda.
*
* Kreator: Aleksa Markovic
******************************************************************************/

#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>

//Networking headers:
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<sys/ioctl.h>
#include<netdb.h>

#define MAX_IP_SIZE 15		// Velicina stringa IPv4 adrese
#define BUF_SIZE 255		// Velicina bafera
#define WIFI_INTERFACE "wlp2s0" // Mrezni interfejs
#define NUM_OF_DEVICES 10	// Maksimalan broj klijenata na mrezi


// Enumeracija koja dozvoljava ili zabranjuje ispisivanje na standardni
// izlaz tokom izvrsavanja funkcije. Prosledjuje se kao ulazni argument
typedef enum logging { LOG_ON = 1 , LOG_OFF = 0 } _logs;

/*
* Ova funkcija izvrsava inicijalizaciju TCP soket servera.
* Ukljucuje bind.
* Ulazni argumenti:
*	char* ip		-> IP adresa servera
*	unsigned int port -> Port na kom server slusa
*	_logs log		-> Dozvola za ispisivanje stanja na stdout
* Izlazni argumenti:
*	uint8_t check		-> Izvestava o uspesnosti inicijalizacije servera
*/
uint8_t tcp_server_init( char* ip , unsigned int port , _logs log );

/*
* Ova funkcija izvrsava slusanje servera na definisanom portu.
* Pre poziva ove funkcije je neophodno inicijalizovati server.
* Ulazni argumenti:
*	_logs log		-> Dozvola za ispisivanje stanja na stdout
* Izlazni argumenti:
*	uint8_t check		-> Izvestava o uspesnosti
*/
uint8_t tcp_server_listen( _logs log );

/*
* Ova funkcija prihvata konekciju sa klijentom i dodeljuje joj
* soket fajl deskriptor.
* Ulazni argumenti:
*	int* newSocket_			-> Adresa fajl deskriptora konekcije sa klijentom
*	struct sockaddr_in* newAddr	-> Adresa strukture koja cuva informacije o adresi
*					   klijenta
*	_logs log			-> Dozvola za ispisivanje stanja na stdout
* Izlazni argumenti:
*	uint8_t check		-> Izvestava o uspesnosti konekcije
*/
uint8_t tcp_server_accept( int* newSocket_ , struct sockaddr_in* newAddr , _logs log );

/*
* Ova funkcija salje poruku konektovanom klijentu
* Pre poziva ove funkcije je neophodno je uspostaviti konekciju sa klijentom
* Ulazni argumenti:
*	int sockfd	-> soket fajl deskriptor konekcije sa klijentom
*	char* w_buf	-> niz bajtova koji sadrzi informaciju o poruci
* Izlazni argumenti:
*	uint8_t check	-> Izvestava o uspesnosti (return send(...))
*/
uint8_t tcp_server_send( int sockfd , char* w_buf );

/*
* Ova funkcija cita poruku od klijenta iz bafera masine,
* smesta je u programski bafer.
* Pre poziva ove funkcije je neophodno je uspostaviti konekciju sa klijentom
* Ulazni argumenti:
*	int sockfd	-> soket fajl deskriptor konekcije sa klijentom
*	char* r_buf	-> niz bajtova u koji se smesta informaciju o poruci
* Izlazni argumenti:
*	uint8_t check	-> Izvestava o uspesnosti (return send(...))
*/
uint8_t tcp_server_recv( int sockfd , char* r_buf );

/*
* Na adresu ulaznog argumenta upisuje informaciju o IP adresi servera.
* Ulazni argumenti:
*	char* IPaddr	-> Adresa od koje pocinje smestanje informacije
*	_logs log	-> Dozvola za ispisivanje stanja na stdout
*/
void getIP( char* IPaddr , _logs log );

#endif // _TCP_SERVER_H_
