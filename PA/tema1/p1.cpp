/*
* nume: Bojin
* prenume: Andrei Florian
* grupa/seria: 323 CA
*/

#include <stdio.h>

#define MAX 100000

	/* Folosind functia "ok", caut binar cea mai mare distanta pentru care
	* "ok" intoarce "true", fiind, de altfel, maximizarea distantei minime
	* dintre doi stalpi consecutivi. "ok" intoarce true de la 0 la distanta
	* cautata si false pentru valori mai mari decat aceasta:
	* 
	* Exemplu, distanta cautata = 4
	* 	    _________
	* 	ok:          |_______
	* dist: 0 1 2 3 4 5 6 7 8
	* 
	* Remember! (lower + upper) / 2 is bad!
	*/
int ok(unsigned int dist, unsigned int N, unsigned int M, unsigned int D[MAX]) {
	unsigned int j = 1;
	unsigned int prev = 0;
	for (unsigned int i = 1; i < N; i++) {	// parcurg vectorul de distante, D
		if (D[i] - D[prev] >= dist) {	// daca am o distanta mai mare decat dist
			prev = i;	// retin ultimul indice la care acest lucru s-a intamplat
			j++;	// contorizez numarul de distante mai mari decat dist,
					// pornind de la ultimul indice din vectorul D pentru care
					// dist era mai mare decat distanta dintre indicele respectiv
					// si indicele curent la momentul respectiv
		}
		if (j == N-M)	// daca contorul j a ajuns cat numarul de stalpi cati ar
						// trebui sa ramana dupa eliminare
			return 1;
	}
	return (j == N-M) ? 1:0;
}

int main() {

	// deschiderea fisierului pentru citire:
	FILE *f1=NULL;
	f1=fopen("date.in","r");

	// declararea variabilelor:
	unsigned int N, M, D[MAX];	// D este vectorul care retine distantele
								// stalpilor fata de primul stalp
	unsigned int L, i=1, low, high, result, mid;

	// citirea din fisier:
	fscanf(f1,"%u",&N);
	fscanf(f1,"%u",&M);
	fscanf(f1,"%u",&L);
	for(i=0;i<N;i++)
		fscanf(f1,"%u",&D[i]);

	// algoritmul de binary search pe vectorul D, de distante
	low = 0;
	high = L;
	result = 0;
	while (low <= high) {	// conditia de iesire din binary search
		mid = low + ((high-low)>>1);	// mid=(low+mid)/2
		if (ok(mid,N,M,D)) {	// daca distanta "mid" este buna
			low = mid + 1;	// caut o distanta mai mare decat "mid-ul" actual
			result = mid;	// retin ultima distanta buna
		} 
		else
			high = mid - 1;	// daca "mid" nu e bun, caut o distanta mai mica
	}

	// deschiderea fisierului spre scriere:
 	FILE *f2=NULL;	
	f2=fopen("date.out","w");

	// scrierea in fisier a rezultatului:
	fprintf(f2,"%u\n",result);

	// reconstruirea solutiei:
	unsigned int count=0, prev=0;	// count reprezinta numarul de stalpi ramasi

	// parcurg vectorul de distante, D si contorizez numarul de stalpi pentru
	// care solutia este mai mare decat distanta dintre doi stalpi consecutivi
	for(i=1;i<N;i++) {
		if(D[i] - D[prev] < result)
			continue;
		count++;	// contorizez numarul de stalpi buni
		prev=i;	// retin ultimul stalp pentru care solutia s-a dovedit buna
	}	

	// afisez numarul de stalpi ramasi:
	fprintf(f2,"%u\n",count+1);
	
	// refac algoritmul de parcurge si aflare a stalpilor buni, dar de data
	// aceasta ii si afisez
	prev = 0;
	for(i=1;i<N;i++) {
		if(D[i] - D[prev] < result)
			continue;
		fprintf(f2,"%u\n",D[prev]);
		prev=i;
	}
	fprintf(f2,"%u\n",D[N-1]);	// afisez ultimul stalp

	// inchiderea fisierelor:
	fclose(f1);
	fclose(f2);

	return 0;
}

