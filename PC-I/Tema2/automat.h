/**
 * Homework 2 - C Programming
 *
 * Authors: Victor Carbune (victor@rosedu.org)
 * 	    Laura Vasilescu (laura@rosedu.org)
 */

#ifndef __AUTOMAT_H
#define __AUTOMAT_H

#define MAX_NUMBER_OF_PRODUCTS	100

/**
 * Tipurile de bancnote disponibile.
 */
typedef enum {
	_1_DOLLAR = 1,
	_5_DOLLARS = 5,
	_10_DOLLARS = 10,
	_50_DOLLARS = 50
} bill_type;

/**
 * Seteaza numarul de bancnote disponibile din tipul dat.
 */
void set_available_bills(bill_type bill, int count);

/**
 * Intoarce numarul de bancnote disponibile de un anumit tip.
 * (-1, daca tipul respectiv de bancnote nu exista).
 */
int get_available_bills(bill_type bill);

/**
 * Seteaza numarul de produse disponibile. Initial, acest
 * numar trebuie sa fie 0.
 */
void set_number_of_products(int count);

/**
 * Intoarce numarul total de produse disponibile.
 * Este vorba de numarul total de produse, indiferent de cantitate.
 */
int get_number_of_products();

/**
 * Intoarce costul unui anumit produs, dat prin codul sau.
 * Fiecare produs este identificat printr-un cod unic.
 */
int get_product_cost(int code);

/**
 * Seteaza costul unui produs.
 */
void set_product_cost(int code, int cost);

/**
 * Seteaza cantitatea disponibila a unui anumit produs.
 */
void set_product_quantity(int code, int quantity);

/**
 * Intoarce cantitatea disponibila a unui anumit produs.
 */
int get_product_quantity(int code);

/**
 * Cere un produs.
 *
 * Urmatoarele coduri de eroare trebuie intoarse, in diferite situatii:
 * 	-1 -- a fost cerut un produs care nu exista (cod invalid)
 * 	-2 -- produsul nu mai este disponibil (cantitate = 0)
 * 	-3 -- insuficienti bani introdusi pentru a cumpara produsul.
 * 	-4 -- imposibil de returnat rest cu bancnotele disponibile.
 * 
 * 
 * Daca nu se intoarce un cod de eroare, request_product va intoarce
 * suma totala pe care ar putea sa o ofere drept rest, in cazul in care
 * request_change este apelata imediat dupa aceea. Intern, bancnotele
 * disponibile nu vor fi modificate pana la apelul request_change.
 */
int request_product(int code);

/**
* Functia de rest: calculeaza pe baza sumei initiale introduse si a 
* numarului de bancnote de fiecare tip din automat daca e posibil de 
* returnat rest cu bancnotele disponibile. 
* Returneaza 1 daca e posibil si 0 daca nu e posibil.
* De asemenea, retine in vectorul declarat global rest[4], numarul de 
* fiecare tip de bancnota necesar intoarcerii restului, in caz ca e posibil.
*/
int restul(int code);

/**
 * Cere restul.
 *
 * Nota: acest automat este special, in sensul ca un client poate sa
 * nu ceara intotdeauna rest, sau sa uite!
 *
 * Daca functia este apelata dupa request_product (*doar daca* cumparatorul
 * doreste sa faca asta) i se va intoarce restul disponibil.
 *
 * Daca request_product nu a fost apelat si au fost bani introdusi,
 * aceasta functie va intoarce aceeasi suma, in orice bancnote
 * disponibile.
 *
 * Daca force_request_product este apelata, cumparatorul nu doreste
 * rest, si aceasta functie nu ar trebui apelata.
 */
int request_change();

/**
 * Cere un produs, chiar daca nu exista rest disponibil.
 * Codurile intoarse sunt aceleasi ca si la request_product,
 * cu exceptia erorii in privinta restului.
 */
int force_request_product(int code);

/**
 * Insereaza o bancnota.
 * (-1, daca bancnota nu a putut fi identificata).
 */
int insert_bill(bill_type bill);

/**
 * Reseteaza automatul (reset ar trebui sa reseteze intreaga stare,
 * numar de produs, cantitati, costuri, etc).
 */
void reset();

#endif /* __AUTOMAT_H */
