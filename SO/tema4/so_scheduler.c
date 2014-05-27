/*	Tema 4: Planificator de threaduri
 *
 *	nume: Bojin Andrei Florian
 *	grupa/seria: 333 CA
*/
#include <semaphore.h>
#include "vector.h"
#include "so_scheduler.h"

#define MAX_NUM_THREADS 1000 // nr max de threaduri

struct thread {

	pthread_t tid; // id-ul threadului
	so_handler *handler; // handler-ul
	unsigned prio; // prioritatea
	int waiting; // este 0/1
	sem_t sem; // semafor specific threadului

} threads[MAX_NUM_THREADS]; // vector de threaduri

struct current_thread {

	int pos; // pozitie in cadrul vectorului threads
	unsigned quantum; // cuanta de timp ramasa

} curr_thread; // threadul curent

int num_threads; // nr total de threaduri si indice curent
unsigned num_devices; // nr total de device-uri I/O
unsigned quantum; // cuanta de timp pt fiecare thread
int init = 0; // indica daca initializarea scheduler-ului s-a facut

vector *devices; // tablou ce mapeaza deviceurile la threaduri
vector order[SO_MAX_PRIO]; // tablou ce mapeaza threadurile si ordinea lor
							// la prioritatea acestora
int prio_pos[SO_MAX_PRIO]; // vector cu pozitia prioritatii threadului curent
							// in cadrul listei prioritatii respective
							// (vectorul din tabloul order)


// functie de planificare Round Robin a threadurilor
int r_robin(int max_prio, int min_prio, int exchange) {

	int i, j;
	// se parcurge in ordinea descrescatoare a prioritatii
	for(i=max_prio; i>min_prio; i--) {
		// pt aceeasi prioritate se parcurge de la pozitia prioritatii
		// threadului curent pan la sfarsit
		for(j=prio_pos[i]; j<vector_get_size(&order[i]); j++) {

			// se identifica indicele threadului gasit
			int pos = (int)vector_get(&order[i], j);

			// daca threadul gasit nu este in asteptare
			if(threads[pos].waiting == 0) {

				// pozitia prioritatii threadului gasit devine pozitia actuala
				prio_pos[threads[pos].prio] = j;
				curr_thread.quantum = quantum; // se reinitializeaza cuanta

				// daca threadul gasit nu este threadul curent
				if(pos != curr_thread.pos) {

					// se salveaza pozitia threadului curent
					int pos_copy = curr_thread.pos;
					// threadul curent devine threadul gasit
					curr_thread.pos = pos;

					// se da drumul la threadul gasit pt rulare
					sem_post(&threads[pos].sem);

					if(exchange == 1) // (optional)
						// se opreste vechiul thread
						sem_wait(&threads[pos_copy].sem);
				}
				// odata ce a avut loc interschimbul de threaduri se iese
				return -1;
			}
		}
	}
	return 0;
}

// functie de planificare a threadurilor pe baza unui alg round_robin
void schedule(int exchange) {

	// se retine prioritatea threadului curent
	unsigned curr_prio = threads[curr_thread.pos].prio;

	// PAS PREEMPTIV: se realizeaza round robin indiferent
	// daca in urma round_robinului s-a schimbat threadul curent sau s-a
	// reinitializat cuanta threadului curent
	if(r_robin(SO_MAX_PRIO, curr_prio, exchange) == -1)
		return; // si-a terminat treaba

	// daca s-a terminat cuanta de timp a threadului curent sau acesta este in
	// asteptare
	if(curr_thread.quantum == 0 || threads[curr_thread.pos].waiting == 1) {

		prio_pos[curr_prio]++; // pozitia prioritatii threadului curent
							// avanseaza (este mai prioritar)

		// daca pozitia prioritatii threadului curent depaseste nr total de 
		// pozitii al prioritatii respective
		if(prio_pos[curr_prio] >= vector_get_size(&order[curr_prio]))
			prio_pos[curr_prio] = 0; // se reinitializeaza

		// daca in urma round_robinului s-a schimbat threadul curent sau s-a
		// reinitializat cuanta threadului curent
		if(r_robin(curr_prio, -1, exchange) == -1)
			return; // si-a terminat treaba
	}
}

// functie de creare si initializare a scheduler-ului
DECL_PREFIX int so_init(unsigned _quantum, unsigned _num_devices) {

	// verificare parametrii si daca s-a initializat deja
	if((_quantum == 0 && _num_devices == 0) || 
	   (_quantum == 1 && (int)_num_devices == -1) || init == 1)

		return -1;

	// initializari
	num_threads = 0;
	num_devices = _num_devices;
	quantum = _quantum;

	// se aloca tabloul de device-uri I/O
	devices = (vector *) malloc(num_devices * sizeof(vector));
	// se initializeaza fiecare vector din cadrul devices
	unsigned i;	
	for(i=0; i<num_devices; i++)
		vector_init(&devices[i]);
	// se initializeaza fiecare vector din cadrul tabloului order
	for(i=0; i<SO_MAX_PRIO; i++)
		vector_init(&order[i]);

	init = 1; // a avut loc initializarea

	return 0;
}

// rutina de pornire a unui thread:
// se apeleaza handlerul acestuia si se pune in asteptare
void* start_thread(void * arg) {

	sem_wait(&threads[(int)arg].sem); // se inchide semaforul threadului

	// se apeleaza handlerul
	threads[(int)arg].handler( threads[(int)arg].prio );
	threads[(int)arg].waiting = 1; // se pune in asteptare threadul

	schedule(0); // se ruleaza scheduler-ul

	return NULL;
}

// functie de creare a unui nou thread
DECL_PREFIX tid_t so_fork(so_handler *_handler, unsigned _prio) {

	// verificare parametrii
	if(_handler == 0)
		return INVALID_TID;

	// initializari: handler, prioritate thread, semafor
	threads[num_threads].handler = _handler;
	threads[num_threads].prio = _prio;
	threads[num_threads].waiting = 0; // threadul nu este inca in asteptare

	sem_init(&threads[num_threads].sem, 0, 0); // initializare semafor thread

	// se adauga la sfarsitul listei cu prioritatea _prio, threadul de creat
	vector_push_back(&order[_prio], (void *)num_threads);

	// crearea unui nou thread
	pthread_t tid;
	pthread_create(&tid, NULL, start_thread, (void*)num_threads);

	threads[num_threads].tid = tid; // retinerea id-ului noului thread

	num_threads ++; // cresterea nr total de threaduri

	if(num_threads == 1) { // daca este vorba de primul thread creat

		curr_thread.pos = num_threads - 1; // se initalizeaza pozitia in cadrul
											// vectorului threads
		curr_thread.quantum = quantum; // se initializeaza cuanta de timp cu max

		sem_post(&threads[num_threads - 1].sem); // se poate deschide semaforul
	}
	else { // daca este vorba de unul din restul de threaduri

		curr_thread.quantum --; // se scade cuanta threadului curent

		schedule(1); // se ruleaza scheduler-ul
	}
	return tid; // se returneaza id-ul noului thread creat
}

// functie de punere in asteptare a threadurilor de pe un device I/O
DECL_PREFIX int so_wait(unsigned dev_index) {

	// verificare parametrii
	if(dev_index >= num_devices)
		return -1;

	// se adauga threadul curent la sfarsitul listei de threaduri a device-ului
	vector_push_back(&devices[dev_index], (void *)curr_thread.pos);
	threads[curr_thread.pos].waiting = 1; // se pune in asteptare threadul

	schedule(1); // se ruleaza scheduler-ul

	return 0;
}

// functie de semnalarea a threadurilor de pe un device I/O, pt reluarea lor
DECL_PREFIX int so_signal(unsigned dev_index) {

	// verificare parametrii
	if(dev_index >= num_devices)
		return -1;

	int i, size;
	// se retine numarul de threaduri ale device-ului
	size = vector_get_size(&devices[dev_index]);

	// se scot threaduri pe rand din lista de threaduri a device-ului
	// si se scot si din asteptare
	for(i=size-1; i>=0; i--) {
		threads[(int)vector_get(&devices[dev_index], i)].waiting = 0;
		vector_remove(&devices[dev_index], i);
	}
	curr_thread.quantum --; // se scade cuanta threadului curent

	schedule(1); // se ruleaza scheduler-ul

	return size;
}

// functie de executie a unei operatii oarecare
DECL_PREFIX void so_exec() {

	curr_thread.quantum --; // se scade cuanta threadului curent

	schedule(1); // se ruleaza scheduler-ul
}

// functie de distrugere a scheduler-ului
DECL_PREFIX void so_end() {

	int i;
	// se face join la threaduri
	for(i=0; i<num_threads; i++)
		pthread_join(threads[i].tid, NULL);

	// se distruge vectorul de threaduri
	for(i=0; i<num_threads; i++)
		sem_destroy(&threads[i].sem);

	// se distruge tabloul order
	unsigned j;
	for(j=0; j<SO_MAX_PRIO; j++)
		vector_destroy(&order[j]);

	// se distruge tabloul devices
	for(i=0; i<num_devices; i++)
		vector_destroy(&devices[i]);
	free(devices);

	init = 0;
}

