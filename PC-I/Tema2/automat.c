// nume: Bojin
// prenume: Andrei Florian
// grupa/seria: 313CA

#include"automat.h"
#include<stdio.h>

int billsin[4];		//retine numarul de bancnote disponibile din tipul dat, din automat.
int produse[2][MAX_NUMBER_OF_PRODUCTS];		//retine pe linia 0 costul produselor si pe linia 1 cantitatea produselor
int nrProduse;		//retine numarul de tipuri diferite de produse disponibile.
int sumain;	    //retine suma totala introdusa.
int rest[4];	     //retine numarul de bancnote din fiecare tip pentru rest.
int intr[4]; 	     //retine numarul de bancnote din fiecare tip introduse.
int rst=-1,ok;		//variabile pt a verifica daca functia request_product este apelata inainte sau dupa functia request_change

/**
 * Seteaza numarul de bancnote disponibile din tipul dat, din automat.
 */
void set_available_bills(bill_type bill, int count) 
{
	if(count<0)
		return;
	if(bill==_1_DOLLAR) 
			billsin[0]=count;
	else 
		if(bill==_5_DOLLARS) 
				billsin[1]=count;
		else 
			if(bill==_10_DOLLARS) 
					billsin[2]=count;
			else 
				if(bill==_50_DOLLARS) 
						billsin[3]=count;
}

/**
 * Intoarce numarul de bancnote disponibile de un anumit tip, din automat.
 * (-1, daca tipul respectiv de bancnote nu exista).
 */
int get_available_bills(bill_type bill) 
{
	if(bill==_1_DOLLAR)
		return billsin[0];
	else
		if(bill==_5_DOLLARS)
			return billsin[1];
		else
			if(bill==_10_DOLLARS)
				return billsin[2];
			else
				if(bill==_50_DOLLARS)
					return billsin[3];
	return -1;
}

/**
 * Seteaza numarul de tipuri diferite de produse disponibile. Initial, acest
 * numar trebuie sa fie 0.
 */
void set_number_of_products(int count) 
{
	if(count>=0 && count<=MAX_NUMBER_OF_PRODUCTS)
		nrProduse=count;
}

/**
 * Intoarce numarul tipurilor diferite de produse disponibile.
 * Este vorba de numarul total de produse, indiferent de cantitate.
 */
int get_number_of_products() 
{
	return nrProduse;
}

/**
 * Seteaza costul unui produs.
 */
void set_product_cost(int code, int cost) 
{
	if(code>=0 && code<=MAX_NUMBER_OF_PRODUCTS)
		if(cost>=0)
			produse[0][code]=cost; 
}

/**
 * Intoarce costul unui anumit produs, dat prin codul sau.
 * Fiecare produs este identificat printr-un cod unic.
 */
int get_product_cost(int code) 
{
	if(code>=0 && code<=MAX_NUMBER_OF_PRODUCTS)
		return produse[0][code];
	else 
		return -1;
}

/**
 * Seteaza cantitatea disponibila a unui anumit produs.
 */
void set_product_quantity(int code, int quantity) 
{
	if(code>=0 && code<=MAX_NUMBER_OF_PRODUCTS)
		if(quantity>=0)
			produse[1][code]=quantity; 
}

/**
 * Intoarce cantitatea disponibila a unui anumit produs.
 */
int get_product_quantity(int code) 
{
	if(code>=0 && code<=MAX_NUMBER_OF_PRODUCTS)
		return produse[1][code];
	else 
		return -1;
}

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
int request_product(int code) 
{
	ok=1;
	if(code<0 || code>get_number_of_products())
		return -1;
	else 
		if(get_product_quantity(code)==0)
			return -2;
		else
			if(sumain<get_product_cost(code))
				return -3;
			else 
				if(restul(code)==0)
					return -4;
				else 
				{
					if(get_product_cost(code)==0)
						set_product_quantity(code,get_product_quantity(code)-1);
					else
						set_product_quantity(code,get_product_quantity(code)-1);	
					rst=sumain-get_product_cost(code);
					return rst;
				}
}

/**
* Functia de rest: calculeaza pe baza sumei initiale introduse si a 
* numarului de bancnote de fiecare tip din automat daca e posibil de 
* returnat rest cu bancnotele disponibile. 
* Returneaza 1 daca e posibil si 0 daca nu e posibil.
* De asemenea, retine in vectorul declarat global rest[4], numarul de 
* fiecare tip de bancnota necesar intoarcerii restului, in caz ca e posibil.
*/
int restul(int code)
{
	int d;
	d=sumain-get_product_cost(code);
	if(d>0)
	{
		if(d/_50_DOLLARS && get_available_bills(_50_DOLLARS)>=d/_50_DOLLARS)
		{	
			d=d%_50_DOLLARS;
			rest[3]=d/_50_DOLLARS;      
		}
		else
			if(d>_50_DOLLARS) 
			{
				d=d-get_available_bills(_50_DOLLARS)*_50_DOLLARS;
				rest[3]=get_available_bills(_50_DOLLARS);
			}		
		if(d/_10_DOLLARS && get_available_bills(_10_DOLLARS)>=d/_10_DOLLARS)
		{	
			d=d%_10_DOLLARS;
			rest[2]=d/_10_DOLLARS;      
		}
		else 
			if(d>_10_DOLLARS) 
			{
				d=d-get_available_bills(_10_DOLLARS)*_10_DOLLARS;
				rest[2]=get_available_bills(_10_DOLLARS);
			}		
		if(d/_5_DOLLARS && get_available_bills(_5_DOLLARS)>=d/_5_DOLLARS)
		{	
			d=d%_5_DOLLARS;
			rest[1]=d/_5_DOLLARS;      
		}
		else 
			if(d>_5_DOLLARS) 
			{
				d=d-get_available_bills(_5_DOLLARS)*_5_DOLLARS;
				rest[1]=get_available_bills(_5_DOLLARS);
			}		
		if(d/_1_DOLLAR && get_available_bills(_1_DOLLAR)>=d/_1_DOLLAR)
		{	
			d=d%_1_DOLLAR;
			rest[0]=d/_1_DOLLAR;      
		}
		else 
			if(d>_1_DOLLAR) 
			{
				d=d-get_available_bills(_1_DOLLAR)*_1_DOLLAR;
				rest[0]=get_available_bills(_1_DOLLAR);
			}	
	}
	if(d==0) 
		return 1;	//se poate returna rest
	else	
		return 0;	//nu se poate returna rest
}

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
int request_change() 
{
	if(ok==1)	
		if(rst!=-1)
		{	
			set_available_bills(_1_DOLLAR,get_available_bills(_1_DOLLAR)-rest[0]);
			set_available_bills(_5_DOLLARS,get_available_bills(_5_DOLLARS)-rest[1]);
			set_available_bills(_10_DOLLARS,get_available_bills(_10_DOLLARS)-rest[2]);
			set_available_bills(_50_DOLLARS,get_available_bills(_50_DOLLARS)-rest[3]);
			return rst;
		}
		else
			return rst;
	else
		if(sumain!=0)
		{
			set_available_bills(_1_DOLLAR,get_available_bills(_1_DOLLAR)-intr[0]);
			set_available_bills(_5_DOLLARS,get_available_bills(_5_DOLLARS)-intr[1]);		
			set_available_bills(_10_DOLLARS,get_available_bills(_10_DOLLARS)-intr[2]);
			set_available_bills(_50_DOLLARS,get_available_bills(_50_DOLLARS)-intr[3]);
			return sumain;
		}
		else
			return -1;
}

/**
 * Cere un produs, chiar daca nu exista rest disponibil.
 * Codurile intoarse sunt aceleasi ca si la request_product,
 * cu exceptia erorii in privinta restului.
 */
int force_request_product(int code) 
{
	if(code<0 || code>get_number_of_products())
		return -1;
	else 
		if(get_product_quantity(code)==0)
			return -2;
		else
			if(sumain<get_product_cost(code))
				return -3;
			else 
				{
					if(get_product_cost(code)==0)
						set_product_quantity(code,get_product_quantity(code)-1);
					else
						set_product_quantity(code,get_product_quantity(code)-1);	
					return sumain-get_product_cost(code);
				}
}

/**
 * Insereaza o bancnota.
 * (-1, daca bancnota nu a putut fi identificata).
 */
int insert_bill(bill_type bill) 
{
	if(bill==_1_DOLLAR) 
	{
		set_available_bills(bill,get_available_bills(bill)+1);
		sumain+=_1_DOLLAR;
		intr[0]++;		
		return _1_DOLLAR;
	}
	else 
		if(bill==_5_DOLLARS)
		{ 
			set_available_bills(bill,get_available_bills(bill)+1);
			sumain+=_5_DOLLARS;		
			intr[1]++;
			return _5_DOLLARS;
		}
		else 
			if(bill==_10_DOLLARS) 
			{
				set_available_bills(bill,get_available_bills(bill)+1);	
				sumain+=_10_DOLLARS;
				intr[2]++;
				return _10_DOLLARS;
			}
			else 
				if(bill==_50_DOLLARS)
				{
					set_available_bills(bill,get_available_bills(bill)+1);
					sumain+=_50_DOLLARS;
					intr[3]++;
					return _50_DOLLARS;
				}
	return -1;
}

/**
 * Reseteaza automatul (reset ar trebui sa reseteze intreaga stare,
 * numar de produs, cantitati, costuri, etc).
 */
void reset() 
{
	int i;
	set_number_of_products(0);
	for(i=0;i<MAX_NUMBER_OF_PRODUCTS;i++) 
	{	
		set_product_cost(i,0);
		set_product_quantity(i,0);
	}
	set_available_bills(_1_DOLLAR,0);
	set_available_bills(_5_DOLLARS,0);		
	set_available_bills(_10_DOLLARS,0);
	set_available_bills(_50_DOLLARS,0);
	for(i=0;i<4;i++)
		rest[i]=0;
	sumain=0;
	rst=-1;
	ok=0;
}

