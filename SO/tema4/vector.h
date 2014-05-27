/*	Tema 4: Planificator de threaduri
 *
 *	nume: Bojin Andrei Florian
 *	grupa/seria: 333 CA
*/
#include <stdlib.h>

typedef struct {

	void **data;
	int size;
	int capacity;

} vector;

void vector_init(vector *v);
void vector_push_back(vector *v, void *element);
int vector_get_size(vector *v);
void *vector_get(vector *v, int index);
void vector_remove(vector *v, int index);
void vector_destroy(vector *v);

