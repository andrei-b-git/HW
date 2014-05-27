/*
 *	Tema 1 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
 *
 *	varianta: PARALEL NEOPTIMIZATA
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define INF 100000


int main(int argc, char **argv) {

	if(argc != 4) {
		printf("Nu s-au introdus destule argumente la rulare.\n");
		printf("Format executie: ./paralel nr_saptamani fisin fisout.\n");
	}
	else {
		
		int nr_sapt = atoi(argv[1]);	//numarul de saptamani

		//deschiderea fisierelor
		FILE *in = fopen(argv[2], "r");
		FILE *out = fopen(argv[3], "w");

		int N, Nc, i, j;
		fscanf(in,"%d",&N);		//citirea dimensiunii matricii
		fscanf(in,"%d",&Nc);	//citirea numarului de culori

		//cele doua matrici, cea de sapt curenta si cea de sapt urmatoare
		int **config1, **config2, **aux;

		//vectorul care retine pentru fiecare culoare numarul de senatori din 
		//configuratia curenta
		int cul_freq[Nc];

		//vectorul care retine acele culori din configuratia curenta care au nr
		// de senatori egal cu 1
		int cul_freq_1[Nc];

		//vectorul care retine acele culori care nu mai au senatori
		int disp[Nc];

		//alocarea si citirea din fisier a matricilor
		config1 = (int**)malloc(N*sizeof(int*));
		config2 = (int**)malloc(N*sizeof(int*));
		for(i=0; i<N; i++) {
			config1[i] = (int*)malloc(N*sizeof(int));
			config2[i] = (int*)malloc(N*sizeof(int));
			for(j=0; j<N; j++)
				fscanf(in,"%d",&config1[i][j]);
		}

		//initializarea cu 0 a vectorilor folositi
		for(i=0; i<Nc; i++) {
			cul_freq[i] = 0;
			cul_freq_1[i] = 0;
			disp[i] = 0;
		}

		int sapt_cur;		//numarul saptamanii curente
		int Cmax, c, d, max;
		int i1, j1, min, d1, di, dj;


		//parcurg numarul de sapt
		for(sapt_cur=0; sapt_cur<nr_sapt; sapt_cur++) {

			#pragma omp parallel for \
			shared(cul_freq) \
			private(i, j, i1, j1, c, Cmax, d, d1, di, dj, max, min) \
			schedule(runtime)
			//parcurg matricea initiala de configurare config1
			for(i=0; i<N; i++) {
				for(j=0; j<N; j++) {
					max = 0;
					//pentru fiecare culoare
					for(c=0; c<Nc; c++) {
						//daca culoarea nu mai are senatori sau culoarea de 
						//inlocuit are 1 senator
						if(disp[c] == 1 || cul_freq_1[ config1[i][j] ] == 1)
							d = 0;	//distanta minima este 0
						else {
							//calcul a distantei minime de la un elem la o 
							//culoare C
							min = INF;
							for(i1=0; i1<N; i1++) {
								for(j1=0; j1<N; j1++) {
									//daca am un element de culoarea C diferit 
									//de cel pe care il caut
									if((i1 != i || j1 != j) && 
										config1[i1][j1] == c) {
										//calcul a distantei dintre 2 elem din
										//matrice
										di = abs(i-i1);
										dj = abs(j-j1);
										if(di > dj)
											d1 = di;	//max dintre distante
										else
											d1 = dj;
										if(min > d1)
											min = d1;
									}
								}
							}
							d = min;
						}
						//calculez maximul dintre distantele minime
						if(max < d) {
							max = d;
							Cmax = c;
						}
					}
					config2[i][j] = Cmax;	//retin culoarea gasita

					#pragma omp critical
					//incrementez nr de senatori ai culorii care a inlocuit
					cul_freq[ Cmax ] ++;	
				}
			}

			//parcurg si afisez nr de senatori ai fiecarei culori
			for(i=0; i<Nc; i++) {
				if(cul_freq[i] == 0)	//daca am o culoare fara senatori
					disp[i] = 1;		//o marchez cu 1 in vectorul disp
				if(cul_freq[i] == 1)	//daca am o culoarea cu 1 senator
					cul_freq_1[i] = 1;	//o retin in vectorul dedicat
				else
					cul_freq_1[i] = 0;	//reinitializez
				fprintf(out,"%d ",cul_freq[i]);	//afisez nr de senatori ai
												//fiecarei culori
				cul_freq[i] = 0;	//reinitializez
			}
			fprintf(out,"\n");

			//configuratia din sapt urmatoare devine cea din sapt curenta
			aux = config1;
			config1 = config2;
			config2 = aux;
		}

		//afisarea configuratiei finale din ultima saptamana
		for(i=0; i<N; i++) {
			for(j=0; j<N; j++)
				fprintf(out,"%d ",config1[i][j]);
			fprintf(out,"\n");
		}


		//eliberarea memoriei
		for(i=0; i<N; i++) {
			free(config1[i]);
			free(config2[i]);
		}
		free(config1);
		free(config2);

		//inchiderea fisierelor
		fclose(in);
		fclose(out);
	}

	return 0;
}
