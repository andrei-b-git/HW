/*
* nume: Bojin
* prenume: Andrei Florian
* grupa/seria: 323 CA
*/
#include<stdio.h>
#include<stdlib.h>

void afisare(long *pret, long *prev, long sf, FILE *f) {
	if(sf > -1) {	//parcurg secventa de cadouri incepand de la ultimul
		afisare(pret,prev,prev[sf],f);
		fprintf(f,"%ld ",sf+1);	//afisarea indicilor din vectorul de preturi
	}
}

void cadouri(long *pret, long n, FILE *f) {
	long *nr = malloc(n*sizeof(long));	//nr de cadouri bune pt fiecare indice
	long *prev = calloc(n,sizeof(long));	//contine indicele cadoului precedent
	long i,j,pv;
	long nr_max = 1;		//nr max de cadouri bune
	long sf = 0;		//indicele ultimului cadou bun
	nr[0]= 1;	//macar un cadou este bun
	prev[0]= -1;	//primul cadou nu are un cadou precedent cu pret mai mic
	for(i=1;i<n;i++) {		//parcurg lista cu preturile cadourilor
		pv = -1;	//indice local al cadoului precent 
		nr[i] = 1;	//indicele curent are macar un cadou potrivit ca pret
		for(j=0;j<i;j++) {	//parcurg lista de cadouri pana la indicele curent
			if(pret[j]<pret[i] && nr[i]<(nr[j]+1)) { //daca am vreun cadou bun
				nr[i]= nr[j]+1;	//creste numarul de cadouri bune ptr indice
				pv = j;		//indicele cadoului precedent se actualizeaza
			}
		}
		prev[i] = pv;	//se retine indicele ultimului cadou bun indicelui curent
		if(nr_max < nr[i]) {	//daca am un nr de cadouri mai mare ca maximul
			nr_max = nr[i];		//se retine
			sf = i;		//retin indicele ulimului cadou din secventa
		}
	}
	afisare(pret,prev,sf,f);	//afisarea indicilor buni in fisier
	free(prev);		//eliberarea zonelor de memorie
	free(nr);
}

int main(int argc, char **argv) {
	FILE *f1=NULL, *f2=NULL;
	f1=fopen(argv[1],"r");	//deschiderea fisierului .in pt citire
	f2=fopen(argv[2],"w");	//deschiderea fisierului .out pt scriere
	long n,*pret,i;
	fscanf(f1,"%ld",&n);   //numarul de luni
	pret=malloc(n*sizeof(long));		//alocare pt vectorul ce retine preturile
	for(i=0;i<n;i++)
		fscanf(f1,"%ld",&pret[i]);	//citirea preturilor din fisier
	cadouri(pret,n,f2);
	fclose(f1);		//inchiderea fisierelor
	fclose(f2);
	return 0;
}
