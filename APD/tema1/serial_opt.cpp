/*
 *	Tema 1 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
 *
 *	varianta: SERIAL OPTIMIZATA
*/
#include <stdio.h>
#include <stdlib.h>

#define INF 100000


int main(int argc, char **argv) {

	if(argc != 4) {
		printf("Nu s-au introdus destule argumente la rulare.\n");
		printf("Format executie: ./serial_opt nr_saptamani fisin fisout.\n");
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

		//vectorul de culori unde retin numarul de aparitii al fiecarei culori
		//pornind din centru si parcurgand cercurile concentrice
		int culori[Nc];

		//vectorul care retine pentru fiecare culoare intalnita raza minima 
		//pana la aceasta, fata de centru
		int r[Nc];

		//vectorul care retine pentru fiecare culoare numarul de senatori din 
		//configuratia curenta
		int cul_freq[Nc];

		//vectorul care retine acele culori din configuratia curenta care au nr
		// de senatori egal cu 1
		int cul_freq_1[Nc];

		//alocarea si citirea din fisier a matricilor
		config1 = (int**)malloc(N*sizeof(int*));
		config2 = (int**)malloc(N*sizeof(int*));
		for(i=0; i<N; i++) {
			config1[i] = (int*)malloc(N*sizeof(int));
			config2[i] = (int*)malloc(N*sizeof(int));
			for(j=0; j<N; j++)
				fscanf(in,"%d",&config1[i][j]);
		}

		for(i=0; i<Nc; i++) {
			culori[i] = 0;
			r[i] = 0;
			cul_freq[i] = 0;
			cul_freq_1[i] = 0;
		}

		//indicii care imi delimiteaza cautarea pentru elementul curent
		int ilow, iup, jlow, jup;	
		int R;			//raza cercului curent in care fac cautarea
		int nr_nenule;	//numarul de elemente nenule din vectorul culori
		int  sum, k, t;
		int S = (Nc*(Nc-1))/2;	//suma tuturor culorilor
		int sapt_cur = 0;		//numarul saptamanii curente
		int Neap = 0;			//numarul de culori care au disparut pe parcurs
		int Cmax;				//culoarea cautata


		//cat timp nu am ajuns la ultima saptamana
		while(sapt_cur < nr_sapt) {

			//parcurg matricea initiala de configurare config1
			for(i=0; i<N; i++) {
				for(j=0; j<N; j++) {

					//reinitializare variabile
					R = 1;
					nr_nenule = 0;
					sum = 0;
					Cmax = INF;

					//reinitializez vectorul de culori si raze
					for(k=0; k<Nc; k++) {
						culori[k] = 0;
						r[k] = 0;
					}

					//cat timp nu am gasit un Cmax
					while(Cmax == INF) {

						//stabilesc limitele intre care caut
						ilow=i-R;
						while(ilow<0)
							ilow++;
						iup=i+R;
						while(iup>=N)
							iup--;
						jlow=j-R;
						while(jlow<0)
							jlow++;
						jup=j+R;
						while(jup>=N)
							jup--;

						//incerc parcurgerea laturii de sus a cadrului
						//(cercului)
						if(ilow == i-R)
							for(k=jlow; k<=jup; k++) {

								//daca intalnesc o culoare noua
								if(culori[ config1[ilow][k] ] == 0) {
									nr_nenule ++;	//cresc nr de elem nenule
									//tin minte raza la care am gasit culoarea
									r[ config1[ilow][k] ] = R;
									//adun culoarea la o suma
									sum += config1[ilow][k];
								}
								//cresc nr de aparitii al culorii
								culori[ config1[ilow][k] ] ++;
								//daca nr de elem nenule a ajuns cat nr de
								//culori existente
								if(nr_nenule == Nc-Neap) {
									//gasesc prima culoare cu raza cea mai mare
									//(raza curenta)
									for(t=0; t<Nc; t++)
										if(r[t] == R && Cmax > t)
											Cmax = t;	//retin culoarea 
														//gasita								
								}
							}

						//daca nu am gasit un Cmax
						if(Cmax == INF) {
							//incerc parcurgerea laturii din dreapta a cadrului
							//(cercului)
							if(jup == j+R)
								for(k=ilow+(ilow == i-R); k<=iup; k++) {

									//daca intalnesc o culoare noua
									if(culori[ config1[k][jup] ] == 0) {
										nr_nenule ++;	//cresc nr de elem 
										//nenule tin minte raza la care am
										//gasit culoarea
										r[ config1[k][jup] ] = R;
										//adun culoarea la o suma
										sum += config1[k][jup];
									}
									//cresc nr de aparitii al culorii
									culori[ config1[k][jup] ] ++;
									//daca nr de elem nenule a ajuns cat nr de
									//culori existente
									if(nr_nenule == Nc-Neap) {
										//gasesc prima culoare cu raza cea mai
										//mare (raza curenta)
										for(t=0; t<Nc; t++)
											if(r[t] == R && Cmax > t)
												Cmax = t;	//retin culoarea 
															//gasita							
									}
								}
						}

						//daca nu am gasit un Cmax
						if(Cmax == INF) {
							//incerc parcurgerea laturii de jos a cadrului
							//(cercului)
							if(iup == i+R)
								for(k=jup-(jup == j+R); k>=jlow; k--) {

									//daca intalnesc o culoare noua
									if(culori[ config1[iup][k] ] == 0) {
										nr_nenule ++;	//cresc nr de elem
										//nenule tin minte raza la care am
										//gasit culoarea
										r[ config1[iup][k] ] = R;
										//adun culoarea la o suma
										sum += config1[iup][k];
									}
									//cresc nr de aparitii al culorii
									culori[ config1[iup][k] ] ++;
									//daca nr de elem nenule a ajuns cat nr de
									//culori existente
									if(nr_nenule == Nc-Neap) {
										//gasesc prima culoare cu raza cea mai
										//mare (raza curenta)
										for(t=0; t<Nc; t++)
											if(r[t] == R && Cmax > t)
												Cmax = t;	//retin culoarea 
															//gasita								
									}
								}
						}

						//daca nu am gasit un Cmax
						if(Cmax == INF) {
							//incerc parcurgerea laturii din stanga a cadrului
							//(cercului)
							if(jlow == j-R)
								for(k=iup-(iup==i+R);k>=ilow+(ilow==i-R);k--) {

									//daca intalnesc o culoare noua
									if(culori[ config1[k][jlow] ] == 0) {
										nr_nenule ++;	//cresc nr de elem
										//nenule tin minte raza la care am
										//gasit culoarea
										r[ config1[k][jlow] ] = R;
										//adun culoarea la o suma
										sum += config1[k][jlow];
									}
									//cresc nr de aparitii al culorii
									culori[ config1[k][jlow] ] ++;
									//daca nr de elem nenule a ajuns cat nr de
									//culori existente
									if(nr_nenule == Nc-Neap) {
										//gasesc prima culoare cu raza cea mai
										//mare (raza curenta)
										for(t=0; t<Nc; t++)
											if(r[t] == R && Cmax > t)
												Cmax = t;	//retin culoarea 
															//gasita							
									}
								}
						}

						//daca nu am gasit un Cmax
						if(Cmax == INF) {
							//daca culoarea curenta pe care vreau s-o
							//inlocuiesc are mai mult de 1 senator
							if(cul_freq_1[ config1[i][j] ] != 1) {

								//daca nr de elem nenule a ajuns cat nr de
								//culori existente -1
								if(nr_nenule == Nc-1-Neap) {	
									Cmax = S - sum;	//retin culoarea care
													//a mai ramas negasita
								}
								else
									R ++;	//cresc raza
							}
							//daca culoarea curenta pe care vreau s-o
							//inlocuiesc are 1 senator
							else {

								//daca nr de elem nenule a ajuns cat nr de
								//culori existente -2
								if(nr_nenule == Nc-2-Neap) {
									//retin culoarea ramasa negasita
									Cmax = S - sum - config1[i][j];
								}
								else
									R ++;	//cresc raza
							}
						}
					}	//end while

					//inlocuiesc culoarea gasita in noua configuratie
					config2[i][j] = Cmax;

					cul_freq[ Cmax ] ++;	//incrementez nr de senatori
											//ai culorii care a inlocuit
				}	//end for
			}	//end for

			//reinitializez 
			S = (Nc*(Nc-1))/2;	//suma tuturor culorilor
			Neap = 0;	//numarul de culori care au disparut pe parcurs

			//parcurg si afisez nr de senatori ai fiecarei culori
			for(i=0; i<Nc; i++) {
				if(cul_freq[i] == 0) {	//daca am o culoare fara senatori
					Neap ++;			//cresc nr de culori fara senatori
					S -= i;				//o elimin din suma tuturor culorior
				}
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

			sapt_cur ++;	//trec la saptamana urmatoare

		}	//end while

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
