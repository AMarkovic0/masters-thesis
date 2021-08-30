/* **************************************************************************************************************
* gen_alg.h je fajl koji sadrzi preprocesorske instrukcije i deklaracije za isvrsavanje upravljackog
* dela programa za kontrolu motora jednosmerne struje. Radi pravilnog koriscenja ovog fajla, neophodno je
* da program sadrzi definicije drajver aplikativnog interfejsa. Pored deklaracija za izvrsavanje logike
* upravljackog programa, ovaj fajl sadrzi i predefinisane deklaracije za interfejs integracije sa brokerom
* i ostalim upravljackim delovima programa.
*
* Kreator: Aleksa Markovic
**************************************************************************************************************** */

#ifndef _GEN_ALG_H_
#define _GEN_ALG_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

#include<poll.h>

#include "driver_api.h" // Drajver aplikativni interfejs

#define PI acos(-1.0f)
#define GENE_LEN 3

// Komande:
#define EXECUTE_ALGORITHM	"EXEC"
#define SEND_RESULT		"SENDR"

/*
* Hromozom struktura:
*      genetic_code	-> gen (niz elemenata: Kp, Ti, Td)
*      fitness	-> Vrednost funkcije prilagodjenosti
*/
typedef struct Chromosome
{
	float genetic_code[GENE_LEN];
	float fitness;
}_chromosome;

/*
* Struktura za cuvanje parametara algoritma:
*      target			-> Zeljena vrednost funkcije prilagodjenosti
*      possibile_gene_values	-> Opseg vrednosti elemenata genetskog koda
*      max_itterations		-> Maksimalni broj iteracija
*      mutation_rate		-> Sansa za mutaciju gena
*/
typedef struct AlgorithmParameters
{
	float target;
	int possible_gene_values[2];
	int max_iterations;
	float mutation_rate;
}_parameters;

// Dodatne funkcije:

/*
* Generator pseudo nasumicnih brojeva iz opsega:
*      Neophodno je inicijalizovati seme generatora u glavnoj funkciji
*      Ulazni argumenti:
*	       int lower -> Donja granica opsega generatora
*	       int upper -> Gornja granica opsega generatora
*      Izlazni argument:
*	       int out	  -> Pseudo nasumicno generisan broj
*/
int randomX(int lower, int upper);

// Deklaracije algoritma za sortiranje quick sort
void swap(_chromosome* a, _chromosome* b);
int Qsort_partition(_chromosome* arr, int low, int high);
void Qsort(_chromosome* arr, int low, int high);

// Set funkcija genetskog algoritma:

/*
* Funkcija kreira inicijalnu populaciju hromozoma. Kreiranje se izvrsava pseudo nasumicno.
*	Ulazni argumenti:
*		_chromosome* init_pop -> Niz hromozoma koji ce sadrzati gnerisane genetske kodove
*		float (*fun)(float*)  -> Adresa funckije prilagodjenosti
*/
void initial_population(_chromosome* init_pop, float (*fitness_function)(float*));

/*
* Funkcija za sortiranje jedinki i selekciju n najboljih uzoraka
*	Ulazni argumenti:
*		_chromosome* population -> Niz definisanih jedinki za selekciju
*		_chromosome* selected 	-> Niz koji ce sadrzati izabrane jedinke
*/
void selection(_chromosome* population, _chromosome* selected);

/*
* Funkcija koja koja izvrsava kombinovanje gena roditelja 1 i 2 na prelomnom elementu
*	Ulazni argumenti:
*		float parent1[] -> Roditelj 1 - niz elemenata float
*		float patent2[] -> Roditelj 2 - niz elemenata float
*		break_point 	-> Element na kom se hromozom deli i kombinuje
*	Izlazni argument:
*		float* out 	-> Genetski kod nove jedinke
*	NOTE: Funkcija vraca genetski kod samo jedne jedinke
*	      [parent1[0:break_point], parent2[break_point:end]]
*/
float* crossover(float parent1[GENE_LEN], float parent2[GENE_LEN], int brek_point);

/*
* Funkcija kreira novu generaciju jedniki na osnovu izabranih jedinki
*	Ulazni argumenti:
*		_chromosome* selected_population -> Izabrana populacija jedinki
*		_chromosome* generation 	 -> Niz jedinki nove generacije
*		float (*fun)(float*)  		 -> Adresa funckije prilagodjenosti
*/
void create_generation(_chromosome* selected_population, _chromosome* generation, float (*fitness_function)(float*));

/*
* Funkcija koja izvrsava ceo algoritam i vraca globalno najbolju jedinku.
*	Ulazni argumenti:
*		_chromosome* population -> Niz jedinki koji cuva jedinke svake generacije. Sledeca generacija zamenjuje
*					   trenutnu
*		_chromosome* selected 	-> Niz jedniki koj cuva izabrane jedinke svake generacije. Sledeca generacija
*					   zamenjuje trenutnu
*		float (*fun)(float*)  	-> Adresa funckije prilagodjenosti
*	Izlazni argument:
*		_chromosome out		-> Globalno najprilagodjenjija jedinka
*/
_chromosome best_global_chromosome(_chromosome* population, _chromosome* selected, float (*fitness_function)(float*));

// Funkcija prilagodjenosti
float calculate_fitness( float genetic_code[ GENE_LEN ] );

// Deklaracije integrisanja u sistem

// Funkcija drajver niti
void* gen_alg_driv_exec( void* vargp );
// Signal hendler
void gen_alg_driv_handler( int sig );

float sgn( float x );

#endif	// GEN_ALG
