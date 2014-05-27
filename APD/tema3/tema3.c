/*
 *	Tema 3 APD: Calcul paralel folosind MPI
 *
 *	nume: 		 Bojin
 *	prenume: 	 Andrei Florian
 *	grupa/seria: 333 CA
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

char *in, *out;			// numele fisierelor din in, out
int tip;				// tipul de multime care va fi generata
double x_min, x_max, y_min, y_max;	// subspatiul din planul complex
double rez;				// rezolutia in cadrul subspatiului ales
long MAX_STEPS;			// numarul maxim de iteratii
double ja, jb;			// parametrul complex in cazul multimii Julia

int **image;			// matricea ce retine imaginea pgm
int width, height;		// dimensiunile imaginii/matricii

// task ce va fi trimis fiecarui thread care retine o fasie din matrice
// ce va trebui procesata
typedef struct Task{
	int start;			// pozitia de start a fasiei in matrice
	int length;			// lungimea fasiei
} task;

// functia de citire a fisierului de intrare
void citire(char ** argv){

	in = argv[1];		// retinerea numelui fisierului de intrare
	out = argv[2];		// retinerea numelui fisierului de iesire

	FILE *f = fopen(in,"r");	// deschiderea fisierului spre citire

	fscanf(f, "%d", &tip);	// citirea tipului de multime care va fi generata
	// citirea parametrilor din subspatiul complex
	fscanf(f, "%lf %lf %lf %lf", &x_min, &x_max, &y_min, &y_max);
	fscanf(f, "%lf", &rez);		// citirea rezolutiei
	fscanf(f, "%ld", &MAX_STEPS);	// citirea numarului maxim de iteratii
	if(tip == 1)				// daca multimea de generat este de tipul Julia
		fscanf(f, "%lf %lf", &ja, &jb);	// citeste si parametrul complex

	fclose(f);			// inchide fisierul de intrare
}

// functia de afisare a imaginii
void afisare(){

	FILE *f = fopen(out,"w");	// deschiderea fisierului spre scriere

	// scrierea primilor parametrii ai fisierului pgm
	fprintf(f, "P2\n%d %d\n255\n", width, height);

	// afisarea matricii/imaginii rasturnate
	int i, j;
	for(i=height-1; i>=0; i--){
		for(j=0; j<width; j++)
			fprintf(f, "%d ", image[i][j]);
		fprintf(f, "\n");
	}

	fclose(f);			// inchide fisierului de iesire
}

// functia de alocare a matricii/imaginii
void alocare(){

	width = (int)round((x_max-x_min)/rez);	// calculul lungimii imaginii
	height = (int)round((y_max-y_min)/rez);	// calculul latimii imaginii

	// alocarea memoriei
	image = malloc(height*sizeof(int*));
	int i;
	for(i=0; i<height; i++)
		image[i] = calloc(width,sizeof(int));
}

// functia de dezalocare a memoriei
void dezalocare(){

	int i;
	for(i=0; i<height; i++)
		free(image[i]);
	free(image);
}

// functia de procesare a unui task care calc o fasie din imagine pentru
// o multime de tipul Mandelbrot si retinerea acesteia intr-o solutie partiala
void mandelbrot(task t, int *sol_part){

	int i;
	// se parcurge fasia
	for(i=0; i<t.length; i++){

		double za = 0;			// componenta reala
		double zb = 0;			// componenta imaginara
		int step = 0;		// nr de iteratii initiale

		// cat timp modulul este mai mic ca 2 si nu s-a ajuns la MAX_STEPS
		while(sqrt(za*za+zb*zb) < 2 && step < MAX_STEPS){

			// salvarea parametrilor initiali
			double aux_a = za;
			double aux_b = zb;

			// calculul noilor parametrii in functie de cei initiali
			za = x_min + aux_a*aux_a - aux_b*aux_b + (t.start+i)%width*rez;
			zb = y_min + 2*aux_a*aux_b + (t.start+i)/width*rez;

			// incrementarea numarului de iteratii
			step++;
		}
		// retinerea in vectorul de solutii partiale a fiecarei culori
		sol_part[i] = step%256;
	}
}

// functia de procesare a unui task care calc o fasie din imagine pentru
// o multime de tipul Julia si retinerea acesteia intr-o solutie partiala
void julia(task t, int *sol_part){

	int i;
	// se parcurge fasia
	for(i=0; i<t.length; i++){

		double za = x_min + (t.start+i)%width*rez;		// componenta reala
		double zb = y_min + (t.start+i)/width*rez;		// componenta imaginara
		int step = 0;		// nr de iteratii initiale

		// cat timp modulul este mai mic ca 2 si nu s-a ajuns la MAX_STEPS
		while(sqrt(za*za+zb*zb) < 2 && step < MAX_STEPS){

			// salvarea parametrilor initiali
			double aux_a = za;
			double aux_b = zb;

			// calculul noilor parametrii in functie de cei initiali
			za = aux_a*aux_a - aux_b*aux_b + ja;
			zb = 2*aux_a*aux_b + jb;

			// incrementarea numarului de iteratii
			step++;
		}
		// retinerea in vectorul de solutii partiale a fiecarei culori
		sol_part[i] = step%256;
	}
}

// functia de merge a unei solutii partiale cu matricea/imaginea
void merge_sol_part(int start, int length, int *sol_part){

	int i;
	// se parcurge fasia din matrice si se echivaleaza cu solutia partiala
	for(i=start; i<start+length; i++)
		image[ i/width ][ i%width ] = sol_part[ i-start ];
}

// functia de definire a unui tip MPI de date de tipul structurii Task
void setTaskType(MPI_Datatype *new_type){

	int blocklens[1];			// numarul de elemente din fiecare bloc
	MPI_Aint indices[1];		// vector cu deplasarea blocurilor de date
    MPI_Datatype old_types[1];	// vector cu tipurile de date din structura

	blocklens[0] = 2;			// 2 variabile (start, length)
    indices[0] = 0;
    old_types[0] = MPI_INT;		// de tipul MPI_INT

	// crearea noului tip de date new_type
	MPI_Type_struct(1, blocklens, indices, old_types, new_type);	
    MPI_Type_commit(new_type);
}


int main(int argc, char ** argv){

	citire(argv);				// citirea din fisierul de intrare
	alocare();					// alocarea memoriei pentru imagine

	int rank, size;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Datatype task_type;		// tipul datelor din structura Task
	setTaskType(&task_type);	// crearea tipului de date

	if(rank==0){				// daca este vorba de threadul master

		// calculez lungimea unei fasii de care se va ocupa un thread
		long stripsize = (long)ceil((float)(width*height)/size);
		// calculez indicele de start de unde se va incepe trimiterea
		// catre threaduri
		long from = width*height - (size-1)*stripsize;

		task t;					// creez un task
		int i;
		// impart taskurile cu fasiile fiecaruia, fiecarui thread
		for(i=1; i<size; i++){
			// calculez de unde incepe fasia unui task
			t.start = from + stripsize*(i-1);
			t.length = stripsize;	// lungimea va fi stripsize
			// trimit taskurile create fiecarui thread
			MPI_Send(&t, 1, task_type, i, 0, MPI_COMM_WORLD);
		}
		// setarea parametrilor taskului threadului master
		t.start = 0;
		t.length = from;
		// crearea si alocarea unui vector care va retine fasia
		int *sol_part = malloc(stripsize*sizeof(int));
		// procesez taskul curent in functie de tipul multimii de calculat
		if(tip == 0)
			mandelbrot(t, sol_part);
		else
			julia(t, sol_part);
		// adaugarea rezultatului taskului la matricea ce retine imaginea
		merge_sol_part(0, t.length, sol_part);
		// primirea si a celorlalte solutii partiale de la celelalte threaduri
		for (i = 1; i < size; i++){
			MPI_Recv(sol_part, stripsize, MPI_INT, MPI_ANY_SOURCE, 0,
					 MPI_COMM_WORLD, &stat);
			// adaugarea acestora la matricea/imaginea finala
			merge_sol_part(from + stripsize*(stat.MPI_SOURCE-1), stripsize,
						   sol_part);
		}
		afisare();				// afisarea matricii/imaginii finale
		dezalocare();			// dezalocarea memoriei
	}
	else {		// daca este vorba de toate celelalte threaduri

		task t;	// creez un task in care voi primi ce are de facut threadul
		// primesc taskul cu fasia pe care va lucra threadul
		MPI_Recv(&t, 1, task_type, 0, 0, MPI_COMM_WORLD, &stat);
		// crearea si alocarea unui vector care va retine fasia
		int *sol_part = malloc(t.length*sizeof(int));
		// procesez taskul curent in functie de tipul multimii de calculat
		if(tip == 0)
			mandelbrot(t, sol_part);
		else
			julia(t, sol_part);
		// trimit solutia partiala threadului master
		MPI_Send(sol_part, t.length, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
