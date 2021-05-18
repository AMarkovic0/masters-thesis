/******************************************************************************
*
* test.h je fajl koji sadrzi neophodne deklaracije funkcija,
* definicije makroa i
* preprocesorske instrukcije koje omogucavaju normalan rad probnog upravljackog
* dela progama. Probni upravljacki deo programa je namenjen za testiranje
* rada sistema a moze se koristiti kao interfejs za razvoj novog dela programa
*
* Fajl je pisan koristeci biblioteke POSIX standarda.
*
* Kreator: Aleksa Markovic
******************************************************************************/

#ifndef _TEST_H_
#define _TEST_H_

#include<poll.h>

#include "driver_api.h"

void* test_driv_exec( void* vargp );

#endif // _TEST_H_
