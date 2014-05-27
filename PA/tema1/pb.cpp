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
	long long N, T, i, j, max, min, *C, *CB, ***S;
	long long prev, previ, nr, k, d, offset;

	// citirea din fisier:
	fscanf(f1,"%lld",&N);
	fscanf(f1,"%lld",&T);

	// alocari de memorie:
	C = new long long[N];	// vector cu castigurile fiecarui slot
	CB = new long long[N];	// vector in care reconstruiesc vectorul C, dar
				// rotit la stanga, pana ajunge primul minim din
				// C pe prima pozitie
	I = new struct interval[T];	// vector de intervale
	S = new long long**[N];	// matrice tridimensionala care retine starile
							// anterioare

	// citirea castigurilor si alocarea matricei tridimensionale:
	min=1000000000;
	for(i=0;i<N;i++) {
		fscanf(f1,"%lld",&C[i]);	// citire castiguri
		if(min > C[i]) {	// calcul minim
			min=C[i];
			offset=i;	// retinerea deplasamentului fata de
					// inceputul vectorului C
		}
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

	// construirea vectorului CB:
	for(i=0; i < N-offset; i++)
		CB[i] = C[i+offset];
	for(i=N-offset; i < N; i++)
		CB[i] = C[i-N+offset];

	// aplic acelasi algoritm ca la problema 2, dar pe vectorul CB

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
			if (CB[i]<0)	// daca avem un castig negativ se procedeaza ca pentru
						// o stare din afara intervalelor
				// initializarea matricei asociate unei stari in interiorul 
				// unui interval (1)
				if(S[i-1][j-1][1] > S[i-1][j][0])
					S[i][j][1] = S[i-1][j-1][1];	//maximul dintre S[i-1][j-1][1]
				else
					S[i][j][1] = S[i-1][j][0];	// si S[i-1][j][0]
			else // pentru un profit maxim, se iau in considerare numai
				 // castigurile pozitive
				if(S[i-1][j-1][1]+CB[i] > S[i-1][j][0])
					S[i][j][1] = S[i-1][j-1][1]+CB[i];	//maximul dintre 
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
	if(CB[k]>0) {
		prev=CB[k];
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
		prev=CB[i];	// retinerea castigului corespunzator acestui profit
		previ=i;	// retinerea indicelui castigului
	}
	I[nr].start=previ;
	
	// afisarea numarului de intervale
	fprintf(f2,"%lld\n",T-d);

	// afisarea intervalelor, cu readucerea cu ajutorul offsetului la vectorul C:
	for(i=nr;i>=0;i--) {
		if(I[i].start+offset > N)
			fprintf(f2,"%lld ",I[i].start+offset-N);
		else
			fprintf(f2,"%lld ",I[i].start+offset);
		if(I[i].end+offset > N)
			fprintf(f2,"%lld\n",I[i].end+offset-N);
		else
			fprintf(f2,"%lld\n",I[i].end+offset);
				
	}

	// dezalocari de memorie:
	for(i=0;i<N;i++) {
		for(j=0;j<T;j++)
			delete [] S[i][j];
		delete [] S[i];
	}
	delete [] S;
	delete [] I;
	delete [] CB;
	delete [] C;

	// inchiderea fisierelor:
	fclose(f1);
	fclose(f2);
	
	return 0;
}

