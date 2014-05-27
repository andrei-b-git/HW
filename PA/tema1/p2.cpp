/*
* nume: Bojin
* prenume: Andrei Florian
* grupa/seria: 323 CA
*/

#include <stdio.h>

int main() {

	// vector de structuri de tipul interval cu start si end
	struct interval{
		long long start;	// inceputul intervalului
		long long end;	// sfarsitul intervalului
	} *I;

	// deschiderea fisierului pentru citire:
	FILE *f1=NULL;
	f1=fopen("date.in","r");

	// declararea variabilelor:
	long long N, T, i, j, max, *C, ***S;
	long long prev, previ, nr, k, d, t;

	// citirea din fisier:
	fscanf(f1,"%lld",&N);
	fscanf(f1,"%lld",&T);

	// alocari de memorie:
	C = new long long[N];	// vector cu castigurile fiecarui slot
	I = new struct interval[T];	// vector de intervale
	S = new long long**[N];	// matrice tridimensionala care retine starile
							// anterioare

	// citirea castigurilor si alocarea matricei tridimensionale:
	for(i=0;i<N;i++) {
		fscanf(f1,"%lld",&C[i]);	// citire castiguri
		S[i] = new long long*[T];	// alocare matrice tridimensionala
		for(j=0;j<T;j++) {

			// ultima dimensiune a matricei are dimensiunea 2 si poate
			// lua doua valori, 0 sau 1. 0 este caracteristic unei stari
			// in afara intervalurilor luate pentru a maximiza profitul,
			// 1 este caracteristic interiorului intervalelor cu pricina
			S[i][j] = new long long[2];
			S[i][j][0]=0;	// initializare cu 0 a ultimei dimensiuni a
			S[i][j][1]=0;	// matricei
		}
	}

	// algoritmul de gasire a profitului maxim:
	max=0;	// profitul maxim cautat
	// construirea matricei de stari si profituri partiale posibile, S:
	for (i = 1; i < N; i++) {
		for (j = 1; j < T && j <= i; j++) {
			// initializarea matricei asociate unei stari in afara 
			// intervalelor (0)
			if(S[i-1][j-1][1] > S[i-1][j][0])
				S[i][j][0] = S[i-1][j-1][1];	//maximul dintre S[i-1][j-1][1]
			else
				S[i][j][0] = S[i-1][j][0];	// si S[i-1][j][0]
			if (C[i]<0)	// daca avem un castig negativ se procedeaza ca pentru
						// o stare din afara intervalelor
				// initializarea matricei asociate unei stari in interiorul 
				// unui interval (1)
				if(S[i-1][j-1][1] > S[i-1][j][0])
					S[i][j][1] = S[i-1][j-1][1];	//maximul dintre S[i-1][j-1][1]
				else
					S[i][j][1] = S[i-1][j][0];	// si S[i-1][j][0]
			else // pentru un profit maxim, se iau in considerare numai
				 // castigurile pozitive
				if(S[i-1][j-1][1]+C[i] > S[i-1][j][0])
					S[i][j][1] = S[i-1][j-1][1]+C[i];	//maximul dintre 
												// S[i-1][j-1][1]+C[i]
				else
					S[i][j][1] = S[i-1][j][0];	// si S[i-1][j][0]
		}
		if(i>=T-1 && max < S[i][T-1][1]) {	// cautarea pe ultima coloana a
											// maximului (solutia cautata)
			max = S[i][T-1][1];	// retinerea profitului maxim
			k = i;	// retinerea indicelui din vectorul de costuri, C, la
					// care s-a gasit profitul maxim
		}
	}

	// deschiderea fisierului spre scriere:
	FILE *f2=NULL;	
	f2=fopen("date.out","w");

	// scrierea in fisier a profitului maxim:	
	fprintf(f2,"%lld\n",max);

	// algoritmul de reconstruire a solutiei (intervalelor):
	nr=0;	// numarul de intervale
	d=1;	// numarul de catiguri din care e compus profitul maxim
	// algoritmul consta in pornirea de la profitul maxim gasit, care se afla
	// pe ultima coloana in matricea 3d si parcurgerea pe diagonala principala
	// in sus cat de mult se poate, si apoi in sus cat de mult se poate, pana
	// valoarea gasita esti diferita de profitul maxim de la starea succesiva
	// minus castigul corespunzator acelui profit
	I[nr].end=k+1;
	i=k;
	j=T-1;
	if(C[k]>0) {
		prev=C[k];
		previ=k;
	}
	else {
		prev=0;
		previ=k+1;
	}	
	while(max-prev!=0) {	// conditia de oprire a parcurgerii in sens opus,
							// a solutiei, pornind de la rezultat
		while(S[i-1][j-1][1] == max-prev) { // parcurg cat de mult pot pe
											// diagonala in sus
			i--;
			j--;
		}
		// dupa care parcurg cat de mult pot in sus, pe coloana
		if(S[i-1][j][1] == max-prev || S[i-1][j][0] == max-prev) {
			while(S[i-1][j][1] == max-prev || S[i-1][j][0] == max-prev)
				i--;
			i--;
			j--;
		}
		if(previ-i!=1) {	// daca e cazul crearii unui nou interval
			I[nr].start=previ;
			nr++;
			I[nr].end=i+1;
		}
		d++;
		max=S[i][j][1];	// reinitializarea profitului maxim local
		prev=C[i];	// retinerea castigului corespunzator acestui profit
		previ=i;	// retinerea indicelui castigului
	}
	I[nr].start=previ;
	
	// afisarea numarului de intervale
	fprintf(f2,"%lld\n",T-d);

	// afisarea intervalelor:

	// daca avem un numar de intervale de afisat (T-d) mai mare decat numarul
	// de intervale gasite (nr+1). Adica cazul in care e necesara afisarea unui 
	// interval de tipul [a, a] in cazul unui castig negativ, pentru a nu se lua
	// in calcul valoarea castigului
	// realizam acest lucru daca se poate la inceputul intervalelor
	t=0;
	for(i=1;i<I[nr].start && T-d-t>nr+1;i++) {
		fprintf(f2,"%lld %lld\n",i,i);
		t++;
	}
	// afisarea intervalelor gasite cu intermitente de intervale de tipul [a, a]
	// acolo unde se poate
	for(i=nr;i>=0;i--) {
		fprintf(f2,"%lld %lld\n",I[i].start,I[i].end);
		if(i-1>=0 && T-d-t>nr+1 && I[i-1].start-I[i].end >=2) {
			j=I[i].end+1;
			while(T-d-t>nr+1 && j<I[i-1].start) {
				fprintf(f2,"%lld %lld\n",j,j);
				j++;
				t++;
			}
		}
	}
	// realizam acelasi lucru ca la inceputul intervalelor daca se poate si la 
	// sfarsitul intervalelor
	for(i=I[0].end+1;i<=N && T-d-t>nr+1;i++) {
		fprintf(f2,"%lld %lld\n",i,i);
		t++;
	}

	// dezalocari de memorie:
	for(i=0;i<N;i++) {
		for(j=0;j<T;j++)
			delete [] S[i][j];
		delete [] S[i];
	}
	delete [] S;
	delete [] I;
	delete [] C;

	// inchiderea fisierelor:
	fclose(f1);
	fclose(f2);
	
	return 0;
}

