/* Tema 1: Multi-platform Development
 *
 * nume: Bojin Andrei Florian
 * grupa/seria: 333 CA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// tipul structurii unui element al hashtable-ului
struct bucket {
	char *word;	// campul continand cuvantul
	struct bucket *next; // campul continand legatura la urmatorul element
};

struct bucket *hash_t; // hashtable-ul folosit, un vector de bucket-uri
int size; // dimensiunea hashtable-ului

// functia de intializare (alocare) a hashtable-ului
void init() {

	hash_t = (struct bucket *)calloc(size, sizeof(struct bucket));
}

// functie asemanatoare cu strdup
char *my_strdup(char *s) {

	char *p = malloc(strlen(s)+1);
	strcpy(p, s);
	return p;
}

// functia de adaugare a unui cuvant in hashtable
void add(char *value) {

	struct bucket *aux, *new;
	// excluziunea sirului vid
	if(strcmp(value, "") == 0) {
		printf("Sirul vid nu este valid\n");
		return;
	}
	// obtinerea adresei bucketului unde se va adauga cuvantul
	// indicele il reprezinta valoarea intoarsa de functia hash
	aux = &hash_t[ (unsigned int)(hash(value, size)) ];
	// parcurgerea bucketului pana la sfarsit,
	// asigurandu-ne ca nu exista duplicate
	while(aux->next != NULL) {
		if(strcmp(aux->next->word, value) == 0)
			break;
		aux = aux->next;
	}
	// daca s-a ajuns la sfarsit se va crea si adauga un element nou
	if(aux->next == NULL) {
		new = (struct bucket *)calloc(1, sizeof(struct bucket));
		// se foloseste strdup pentru a retine cuvantul
		new->word = my_strdup(value);
		new->next = NULL;
		aux->next = new;
	}
}

// functia de eliminare a unui cuvant din hashtable
void remove_h(char *value) {

	struct bucket *aux, *new;
	// excluziunea sirului vid
	if(strcmp(value, "") == 0) {
		printf("Sirul vid nu este valid\n");
		return;
	}
	// obtinerea adresei bucketului de unde se va elimina cuvantul
	// indicele il reprezinta valoarea intoarsa de functia hash
	aux = &hash_t[ (unsigned int)(hash(value, size)) ];
	// parcurgerea bucketului pana la sfarsit
	while(aux->next != NULL) {
		if(strcmp(aux->next->word, value) == 0)
			break;
		aux = aux->next;
	}
	// daca s-a atins sfarsitul bucketului atunci cuvantul nu exista
	if(aux->next == NULL) {
		printf("Sirul de eliminat nu este in hashtable\n");
		return;
	}
	// se elimina elementul si se refac legaturile
	new = aux->next;
	aux->next = new->next;
	free(new->word);
	free(new);
}

// functia de eliminare a tuturor cuvintelor din hashtable
void clear() {

	int i;
	struct bucket *aux, *new;
	// se parcurge hashtable-ul
	for(i=0; i<size; i++) {
		// se ia bucket-ul curent
		aux = &hash_t[i];
		// se parcurge bucket-ul curent
		while(aux->next != NULL) {
			// se elimina elementul si se refac legaturile
			new = aux->next;
			aux->next = new->next;
			free(new->word);
			free(new);
		}			
		aux = NULL;
	}
}

// functia de cautare a unui cuvant in hashtable
void find(char *value, char *out) {

	struct bucket *aux;
	FILE *f;
	// excluziunea sirului vid
	if(strcmp(value, "") == 0) {
		printf("Sirul vid nu este valid\n");
		return;
	}
	// obtinerea adresei bucketului unde se va cauta cuvantul
	// indicele il reprezinta valoarea intoarsa de functia hash
	aux = &hash_t[ (unsigned int)(hash(value, size)) ];
	// parcurgerea bucketului pana la sfarsit
	while(aux->next != NULL) {
		if(strcmp(aux->next->word, value) == 0)
			break;
		aux = aux->next;
	}
	// daca nu a fost dat ca parametru un fisier de output
	if(out == NULL) {
		// daca s-a atins sfarsitul bucketului atunci cuvantul nu exista
		if(aux->next == NULL)
			printf("False\n");
		else
			printf("True\n");
	}
	else {
		// se deschide fisierul de output in modul append
		f = fopen(out, "a");
		if(f == NULL) {
			printf("Nu s-a putut deschide fisierul\n");
			return;
		}
		// daca s-a atins sfarsitul bucketului atunci cuvantul nu exista
		if(aux->next == NULL)
			fprintf(f, "False\n");
		else
			fprintf(f, "True\n");
		fclose(f);	
	}	
}

// functia de afisare a cuvintelor dintr-un bucket de la un anumit index
void print_bucket(int index, char *out) {

	struct bucket *aux;
	FILE *f;
	// obtinerea adresei bucketului pentru care se va face afisarea
	aux = &hash_t[index];
	// daca nu a fost dat ca parametru un fisier de output
	if(out == NULL) {
		// parcurgerea bucketului pana la sfarsit
		while(aux->next != NULL) {
			// afisarea fiecarui cuvant
			printf("%s", aux->next->word);
			if(aux->next->next != NULL)
				printf(" "); // cu spatiu intre cuvinte
			aux = aux->next;
		}
		if(hash_t[index].next != NULL)
			printf("\n"); // fiecare bucket pe un rand
	}
	else {
		// se deschide fisierul de output in modul append
		f = fopen(out, "a");
		if(f == NULL) {
			printf("Nu s-a putut deschide fisierul\n");
			return;
		}
		// parcurgerea bucketului pana la sfarsit
		while(aux->next != NULL) {
			// afisarea fiecarui cuvant
			fprintf(f, "%s ", aux->next->word);
			if(aux->next->next != NULL)
				fprintf(f, " "); // cu spatiu intre cuvinte
			aux = aux->next;
		}
		if(hash_t[index].next != NULL)
			fprintf(f, "\n"); // fiecare bucket pe un rand

		fclose(f);	
	}	
}

// functia de afisare a intreg continutului hashtable-ului
void print(char *out) {

	int i;
	// se parcurge hashtable-ul
	for(i=0; i<size; i++)
		// se afiseaza fiecare bucket
		print_bucket(i, out);
}

// functia de redimensionare a hashtable-ului
void resize(char *mode)
{
	struct bucket *new, *aux, *aux2, *aux3, *aux4;
	int newsize, i;
	// se identifica modul redimensionarii si se actualizeaza dimensiunea
	if (strcmp(mode,"double") == 0)
		newsize = size*2;
	else
		newsize = size/2;
	// se aloca noul hashtable
	new = (struct bucket *)calloc(newsize, sizeof(struct bucket));
	// se parcurge vechiul hashtable
	for(i=0; i<size; i++) {
		aux = hash_t[i].next;
		// se parcurge fiecare bucket
		while(aux != NULL) {
			aux2 = aux;
			aux = aux->next;
			// se retine cuvantul dintr-un element al bucketului si se
			// adauga in noul hashtable
			aux3 = &new[ (unsigned int)(hash(aux2->word, newsize)) ];
			// parcurgerea bucketului pana la sfarsit,
			// asigurandu-ne ca nu exista duplicate
			while(aux3->next != NULL) {
				if(strcmp(aux3->next->word, aux2->word) == 0)
					break;
				aux3 = aux3->next;
			}
			// daca s-a ajuns la sfarsit se va crea si adauga un element nou
			if(aux3->next == NULL) {
				aux4 = (struct bucket *)calloc(1, sizeof(struct bucket));
				// se foloseste strdup pentru a retine cuvantul
				aux4->word = my_strdup(aux2->word);
				aux4->next = NULL;
				aux3->next = aux4;
			}
			// se sterge elementul din bucket
			free(aux2);
		}
		// se sterge bucketul
		hash_t[i].next = NULL;
	}
	// se actualizeaza hashtable-ul si dimensiunea vechi
	hash_t = new;
	size = newsize;
}

// functia de procesare a unei comenzi
void process(char *command) {
	
	// se identifica comanda si posibilii parametrii ai ei
	char *p = strtok(command, " ");
	char *q = strtok(NULL, " ");
	char *r = strtok(NULL, " ");
	char *pos;
	// se elimina eventuale \n rezultate in urma folosirii fgets
	if((pos = strstr(p,"\n")) != NULL)
		*pos = '\0';
	if(q != NULL && (pos = strstr(q,"\n")) != NULL)
		*pos = '\0';
	if(r != NULL && (pos = strstr(r,"\n")) != NULL)
		*pos = '\0';
	// se apeleaza functia corespunzatoare comenzii identificate
	if(strcmp(p, "add") == 0)
		add(q);
	if(strcmp(p, "remove") == 0)
		remove_h(q);
	if(strcmp(p, "clear") == 0)
		clear();
	if(strcmp(p, "find") == 0)
		find(q, r);
	if(strcmp(p, "print_bucket") == 0)
		print_bucket(atoi(q), r);
	if(strcmp(p, "print") == 0)
		print(q);
	if(strcmp(p, "resize") == 0)
		resize(q);
}

// functia de dezalocare si eliberare a memoriei hashtable-ului
void clear_free() {

	clear();
	free(hash_t);
}

int main(int argc, char **argv) {

	char command[20000]; // o comanda poate avea maxim 20000 caractere
	int no_files; // numarul de fisiere primite ca parametru
	FILE *f;
	// trebuie cel putin 2 parametri
	if(argc < 2) {
		printf("Trebuie cel putin 2 parametri\n");
		return 0;
	}
	size = atoi(argv[1]); // retinerea dimensiunii hashtable-ului
	// dimensiunea hash-ului trebuie sa fie pozitiva
	if(size < 0) {
		printf("Dimensiunea hash-ului trebuie sa fie pozitiva\n");
		return 0;
	}
	init(); // alocarea memoriei hashtable-ului
	// daca nu exista fisiere de intrare ca parametrii
	if(argc == 2) {
		// se va citi de la tastatura
		while(fgets(command, sizeof(command), stdin))
			// se face abstractie de liniile goale
			if(strlen(command) > 1)
				process(command); // se proceseaza comanda
	} else { // daca exista fisiere de intrare ca parametrii
		no_files = argc - 2; // se calculeaza numarul de fisiere
		// se ia fiecare fisier la rand
		while(no_files) {
			f = fopen(argv[argc - no_files], "r");
			if(f == NULL) {
				printf("Nu s-a putut deschide fisierul\n");
				return 0;
			}
			// se face citirea din fisier
			while(fgets(command, sizeof(command), f))
				// se face abstractie de liniile goale din fisier
				if(strlen(command) > 1)
					process(command); // se proceseaza comanda
			fclose(f);
			no_files --; // se decrementeaza numarul de fisiere
		}
	}
	clear_free(); // eliberarea memoriei

	return 0;
}

