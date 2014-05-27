/*
# nume: Bojin
# prenume: Andrei Florian
# grupa/seria: 313 CA
*/
#include<stdio.h>
#include<stdlib.h>

int ptr(int b,int k)	//functie pentru calculul lui b la puterea k (necesara pentru masca pe biti).
{
	int i,p=b;
	for(i=2;i<=k;i++)
		p*=b;
	if(k==0)
		return 1;
	return p;	//returneaza b la puterea k.
}

int main()
{
	int i,j,k,c,pas;
	unsigned int n;        //numarul de coloane al matricilor a si b (matricea a = matricea generatiei curente, matricea b = matricea generatiei urmatoare).	
	unsigned long g;	//numarul de generatii simulate.
	unsigned char **a,**b;       //adresele matricilor a si b.
	scanf("%u",&n);
	a=malloc((8*n+2)*sizeof(unsigned char *));	//alocare memorie pentru matricea a, cu dimensiunile unei matrice bordate fata de cea necesara.
	b=malloc((8*n+2)*sizeof(unsigned char *));	//alocare memorie pentru matricea b, cu dimensiunile unei matrice bordate fata de cea necesara.
	for(i=0;i<8*n+2;i++)	
	{
		*(a+i)=calloc(n+2,sizeof(unsigned char));	//alocare memorie pentru matricea a, cu dimensiunile unei matrice bordate fata de cea necesara.
		*(b+i)=calloc(n+2,sizeof(unsigned char));	//alocare memorie pentru matricea b, cu dimensiunile unei matrice bordate fata de cea necesara.
	}	
	for(i=1;i<=8*n;i++)
		for(j=1;j<=n;j++)
		{		
			scanf("%d",&c);		//utilizare a formatului "%d" pentru citirea matricei.
			*(*(a+i)+j)=(unsigned char)c;
		}
	scanf("%lu",&g);
	pas=0;
	while(pas<g)	  //parcurgerea a g generatii, pas cu pas.
	{
		k=0;
		for(i=1;i<=8*n;i++,k=0)		//parcurgerea matricei (interioare) nebordate.
			for(j=1;j<=n;j++,k=0)
				while(k<8)	//2^k reprezinta masca pe biti 
				{
					if( ( *(*(a+i)+j) & ptr(2,k) ) == ptr(2,k) )	//daca bitul cu numarul k al elementului a[i][j] al matricei a este 1 (celula vie).
					{
						if( (int) ( ( ( ( *(*(a+i-1)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
								( ( *(*(a+i)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
							      ( ( *(*(a+i+1)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
							       		        ( ( *(*(a+i+1)+j) & ptr(2,k) ) == ptr(2,k) ) +
	  					     ( ( *(*(a+i-1)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
						       ( ( *(*(a+i)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
						     ( ( *(*(a+i+1)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
						  			        ( ( *(*(a+i-1)+j) & ptr(2,k) ) == ptr(2,k) ) ) /2 ) == 1 )	//daca suma tuturor vecinilor lui a[i][j] este 2 sau 3.
					
							*(*(b+i)+j)+=ptr(2,k);	    //construiesc matricea b (initiala 0), bit cu bit, adunand numai bitii 1.
					}
					else	//daca bitul cu numarul k al elementului a[i][j] al matricei a este 0 (celula moarta).
					{
						if( ( ( ( *(*(a+i-1)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
						        ( ( *(*(a+i)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
						      ( ( *(*(a+i+1)+j+(k==7)) & ptr(2,(k+1)*(k!=7)) ) == ptr(2,(k+1)*(k!=7)) ) +
						       		        ( ( *(*(a+i+1)+j) & ptr(2,k) ) == ptr(2,k) ) +
	  				     ( ( *(*(a+i-1)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
					       ( ( *(*(a+i)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
					     ( ( *(*(a+i+1)+j-(k==0)) & ptr(2,(k-1)*(k!=0)+7*(k==0)) ) == ptr(2,(k-1)*(k!=0)+7*(k==0)) ) +
					  			        ( ( *(*(a+i-1)+j) & ptr(2,k) ) == ptr(2,k) ) ) == 3 )	    //daca suma tuturor vecinilor lui a[i][j] este 3.
					
							*(*(b+i)+j)+=ptr(2,k);	    //construiesc matricea b (initiala 0), bit cu bit, adunand numai bitii 1.
					}
					k++;
	  			}
		for(i=1;i<=8*n;i++)
			for(j=1;j<=n;j++)
			{
				*(*(a+i)+j)=*(*(b+i)+j);	//matricea generatiei urmatoare (b) devine matricea generatiei curente (a).
				*(*(b+i)+j)=0;		//matricea b redevine 0.
			}
		pas++;		//trecerea la generatia urmatoare.
	}
	for(i=1;i<=8*n;i++)
	{
		for(j=1;j<=n;j++)		
			printf("%d ",(int)*(*(a+i)+j));		//utilizare a formatului "%d" pentru afisarea matricei finale.
		printf("\n");
	}
	for(i=0;i<8*n+2;i++)
	{
		free(*(a+i));	     //eliberarea memoriei ocupate de cele doua matrici.
		free(*(b+i));
	}
	free(a);
	free(b);			
	return 0;
}
