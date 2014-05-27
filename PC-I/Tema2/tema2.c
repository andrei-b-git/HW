// nume: Bojin
// prenume: Andrei Florian
// grupa/seria: 313CA

#include"automat.h"
#include<stdio.h>

int main()
{
	int a,b,c,d=1,n,i,ca,co,t;
	printf("Pentru configurarea automatului si apoi cumpararea unui produs, apasati 1.\n");
	printf("Pentru cumpararea unui produs dintr-un automat deja configurat, apasati 2.\n");
	scanf("%d",&a);
	if(a==1)
	{
		printf("Setati nr. de produse diferite:\n");
		scanf("%d",&n);
		set_number_of_products(n);
		for(i=0;i<get_number_of_products();i++)
		{
			printf("Setati costul pt. produsul cu codul %d:\n",i);
			scanf("%d",&co);
			set_product_cost(i,co);
			printf("Setati cantitatea pt. produsul cu codul %d:\n",i);
			scanf("%d",&ca);
			set_product_quantity(i,ca);
		}
		printf("Setati nr. de bancnote de 1$ din automat:\n");
		scanf("%d",&n);
		set_available_bills(_1_DOLLAR,n);	
		printf("Setati nr. de bancnote de 5$ din automat:\n");
		scanf("%d",&n);
		set_available_bills(_5_DOLLARS,n);
		printf("Setati nr. de bancnote de 10$ din automat:\n");
		scanf("%d",&n);
		set_available_bills(_10_DOLLARS,n);
		printf("Setati nr. de bancnote de 50$ din automat:\n");
		scanf("%d",&n);
		set_available_bills(_50_DOLLARS,n);		
		printf("Automatul a fost configurat. Puteti incepe sa cumparati.\n");
		while(d==1)
		{
			reset();
			printf("Introduceti fiecare nr. de bancnote de tipul 1$,5$,10$,50$ cu care vreti sa efectuati cumpararea:\n");
			printf("Nr. de bancnote de 1$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_1_DOLLAR);
			printf("Nr. de bancnote de 5$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_5_DOLLARS);
			printf("Nr. de bancnote de 10$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_10_DOLLARS);
			printf("Nr. de bancnote de 50$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_50_DOLLARS);
			printf("Alegeti codul produsului pe care vreti sa-l cumparati:\n");
			scanf("%d",&n);
			printf("Daca vreti sa primiti rest, apasati tasta 1, daca nu, apasati tasta 2:\n");
			scanf("%d",&t);
			if(t==1)
			{
				printf("Daca vreti sa primiti banii inapoi fara ca cumparati ceva, acum e ultima sansa.\n");
				printf("Apasati 1 ca sa anulati si sa va primiti banii inapoi sau 2 pt a continua cu achizitia:\n");
				scanf("%d",&c);
				if(c==1)
				{
					b=request_change();
					printf("Ati primit suma %d inapoi.\n",b);
				}
				else
				{
					while(request_product(n)==-1)
					{
						printf("Produsul cu codul introdus nu exista. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-2)
					{
						printf("Produsul cu codul introdus nu se mai afla pe stoc. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-3)
					{
						printf("Produsul cu codul introdus costa mai mult decat suma pe care ati introdus-o. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-4)
					{
						printf("Pt. produsul cu codul introdus nu se poate returna rest. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					b=request_change();
					printf("Achizitia s-a incheiat cu succes. Ati primit restul: %d.\n",request_product(n));
				}
			}
			else
			{
				printf("Daca vreti sa primiti banii inapoi fara ca cumparati ceva, acum e ultima sansa.\n");
				printf("Apasati 1 ca sa anulati si sa va primiti banii inapoi sau 2 pt a continua cu achizitia:\n");
				scanf("%d",&c);
				if(c==1)
				{
					b=request_change();
					printf("Ati primit suma %d inapoi.\n",b);
				}
				else
				{
					while(force_request_product(n)==-1)
					{
						printf("Produsul cu codul introdus nu exista. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(force_request_product(n)==-2)
					{
						printf("Produsul cu codul introdus nu se mai afla pe stoc. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(force_request_product(n)==-3)
					{
						printf("Produsul cu codul introdus costa mai mult decat suma pe care ati introdus-o. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					printf("Achizitia s-a incheiat cu succes. La cerere, nu ati mai primit rest.\n");
				}	
			}
			printf("Pt a efectua si alte achizitii, apasati tasta 1. Pentru a iesi din acest meniu, apasati orice tasta.\n");
			scanf("%d",&d);
		}	
	}
	else
	{
		set_number_of_products(5);
		set_product_cost(0,123);
		set_product_quantity(0,6);
		set_product_cost(1,54);
		set_product_quantity(1,2);
		set_product_cost(2,676);
		set_product_quantity(2,1);
		set_product_cost(3,23);
		set_product_quantity(3,5);
		set_product_cost(4,87);
		set_product_quantity(4,4);
		set_available_bills(_1_DOLLAR,5);	
		set_available_bills(_5_DOLLARS,3);
		set_available_bills(_10_DOLLARS,7);
		set_available_bills(_50_DOLLARS,2);		
		while(d==1)
		{
			reset();
			printf("Introduceti fiecare nr. de bancnote de tipul 1$,5$,10$,50$ cu care vreti sa efectuati cumpararea:\n");
			printf("Nr. de bancnote de 1$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_1_DOLLAR);
			printf("Nr. de bancnote de 5$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_5_DOLLARS);
			printf("Nr. de bancnote de 10$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_10_DOLLARS);
			printf("Nr. de bancnote de 50$:\n");
			scanf("%d",&n);
			for(i=0;i<n;i++)
				insert_bill(_50_DOLLARS);
			printf("Alegeti codul produsului pe care vreti sa-l cumparati:\n");
			scanf("%d",&n);
			printf("Daca vreti sa primiti rest, apasati tasta 1, daca nu, apasati tasta 2:\n");
			scanf("%d",&t);
			if(t==1)
			{
				printf("Daca vreti sa primiti banii inapoi fara ca cumparati ceva, acum e ultima sansa.\n");
				printf("Apasati 1 ca sa anulati si sa va primiti banii inapoi sau 2 pt a continua cu achizitia:\n");
				scanf("%d",&c);
				if(c==1)
				{
					b=request_change();
					printf("Ati primit suma %d inapoi.\n",b);
				}
				else
				{
					while(request_product(n)==-1)
					{
						printf("Produsul cu codul introdus nu exista. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-2)
					{
						printf("Produsul cu codul introdus nu se mai afla pe stoc. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-3)
					{
						printf("Produsul cu codul introdus costa mai mult decat suma pe care ati introdus-o. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(request_product(n)==-4)
					{
						printf("Pt. produsul cu codul introdus nu se poate returna rest. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					b=request_change();
					printf("Achizitia s-a incheiat cu succes. Ati primit restul: %d.\n",request_product(n));
				}
			}
			else
			{
				printf("Daca vreti sa primiti banii inapoi fara ca cumparati ceva, acum e ultima sansa.\n");
				printf("Apasati 1 ca sa anulati si sa va primiti banii inapoi sau 2 pt a continua cu achizitia:\n");
				scanf("%d",&c);
				if(c==1)
				{
					b=request_change();
					printf("Ati primit suma %d inapoi.\n",b);
				}
				else
				{
					while(force_request_product(n)==-1)
					{
						printf("Produsul cu codul introdus nu exista. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(force_request_product(n)==-2)
					{
						printf("Produsul cu codul introdus nu se mai afla pe stoc. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					while(force_request_product(n)==-3)
					{
						printf("Produsul cu codul introdus costa mai mult decat suma pe care ati introdus-o. Va rog incercati din nou.\n");
						scanf("%d",&n);
					}
					printf("Achizitia s-a incheiat cu succes. La cerere, nu ati mai primit rest.\n");
				}	
			}
			printf("Pt a efectua si alte achizitii, apasati tasta 1. Pentru a iesi din acest meniu, apasati orice tasta.\n");
			scanf("%d",&d);
		}		
	}
	return 0;
}
