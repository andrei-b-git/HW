/*	Tema 4: Planificator de threaduri
 *
 *	nume: Bojin Andrei Florian
 *	grupa/seria: 333 CA
*/
#include "vector.h"

void vector_init(vector *v) {

	v->size = 0;
	v->capacity = 10;
	v->data = malloc(v->capacity * sizeof(void *));
}

void vector_push_back(vector *v, void *element) {

	if(v->size+1 > v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(void *));
	}
	v->data[v->size] = element;

	v->size ++;
}

int vector_get_size(vector *v) {

	return v->size;
}

void *vector_get(vector *v, int index) {

	return v->data[index];
}

void vector_remove(vector *v, int index) {

	v->size --;

	int i;
	for(i=index; i<v->size; i++)
		v->data[i] = v->data[i+1];
}

void vector_destroy(vector *v) {

	v->size = 0;
	v->capacity = -1;
	free(v->data);
}
