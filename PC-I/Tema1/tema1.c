/*
nume: Bojin
prenume: Andrei Florian
grupa/seria: 313 CA
*/

#include<stdio.h>
#include<math.h>

int main() {
	unsigned long int n,a,b,c;	//declararea numarului natural n
	int l=0;
	scanf("%lu",&n);	//citirea lui n
//punctul 1: determinarea daca numarul n face parte din seria Lucas
	if(n>=0&&n<1000000) {
				a=2;
				b=1;
				c=a+b;
				while(c<=n) {	//construirea seriei Lucas pana la n, inclusiv
					if(c==n){	//testarea de echivalenta cu n, a fiecarui termen al seriei Lucas  
						l=1;
						}
					a=b;
					b=c;
					c=a+b;	//trecerea la urmatorul numar din seria Lucas
				     	}
				if(l==1||n==1||n==2) {
					printf("Numarul %lu face parte din seria Lucas.\n",n);	//outputul in format corespunzator 
					}
				else {
					printf("Numarul %lu nu face parte din seria Lucas.\n",n);
					}
//punctul 2: calculul si afisarea sumei patratelor cifrelor prime pentru numerele strict mai mici decat n, strict pozitive si care sunt numere palindromice
				unsigned long int o,m,i;
				int s;
				for(i=1;i<n;i++) {	//parcurgerea tuturor numerelor strict pozitive i, mai mici strict decat n
						m=i;
						o=0;
						while(m) {
			 		        	o=o*10+m%10;	//construirea oglinditului lui i pe care l-am salvat in m, ca sa nu-l pierd
					  		m=m/10;
					  		}
						if(o==i) {
							printf("%lu | ",i);	//afisarea numarului palindrom i in format corespunzator
			 				s=0;
						        while(o) {
								if(o%10==2||o%10==3||o%10==5||o%10==7) {	//cautarea cifrelor prime din numarul palindrom
													s=s+(o%10)*(o%10);	//calculul sumei patratelor cifrelor prime
													}					     
								o=o/10;
								}
							if(s) {
						  		printf("%d\n",s);	//afisarea sumei
								}
							else {
								printf("0\n");	//afisarea lui 0 pentru suma, daca nici o cifra a numarului palindromic mai mic decat n nu este prima
								}
							}
						} 
//punctul 3: numararea si afisarea numerelor mai mici sau egale cu n, strict pozitive, care in binar au acelasi numar de cifre de 0 si 1
				int nr,j,k,z,u;
				nr=0;
				for(i=1;i<=n;i++) {	//parcurgerea tuturor numerelor strict pozitive i, mai mici sau egale cu n
						j=i;
						k=m=z=u=0;
					      	while(j) {
							m=m+j%2*pow(10,k);	//transformarea numarului din baza 10 in baza 2
						    	k++;
							j=j/2;
						        }
						while(m) {	//parcurgerea numarului in baza 2, cifra cu cifra 
							if(m%10==0) {
								z++;	//calculul numarului de cifre 0
								}
							else {
								u++;	//calculul numarului de cifre 1
								}
							m=m/10;
							}
						if(z==u) {
							nr++;	//incrementarea numarului de numere care verifica conditiile
							printf("%lu\n",i);	//afisarea numerelor care au acelasi numar de cifre 0 si 1, pe cate un rand fiecare
							}
						}
				printf("%d\n",nr);	//afisarea numarului de numere care indeplinesc conditiile
				}			        
	return 0;
}
		
