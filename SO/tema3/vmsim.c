/*
 *	Tema 3 SO: Memorie virtuala
 *
 *	nume/prenume: 	Bojin Andrei Florian
 *	grupa/seria: 	333 CA
*/

#include <stdio.h>
#include <stdlib.h>
#include "vmsim.h"
#include "common.h"

// enumerari/structuri ajutatoare luate din enuntul temei:
enum page_state {
	STATE_IN_RAM,
	STATE_IN_SWAP,
	STATE_NOT_ALLOC
};

// handle pentru o pagina din memoria virtuala (page)
struct page_table_entry {
	enum page_state state;
	enum page_state prev_state;
	w_boolean_t dirty;
	w_prot_t protection;
	w_ptr_t start;
	struct frame *frame; // NULL in cazul in care pagina nu este mapata
};

// handle pentru un pagina din memoria fizica (frame)
struct frame {
	w_size_t index;
	struct page_table_entry *pte;
};

// abstractizare a unei zone de memorie
struct zone {
	vm_map_t map; // structura ce contine adresa de start a zonei mapate,
				// si handler-ele celor doua fisiere pentru RAM si SWAP
	struct page_table_entry *pages; // lista de pagini virtuale
	w_size_t num_pages; // nr de pagini virtuale
	struct frame *frames; // lista de pagini fizice
	w_size_t num_frames; // nr de pagini fizice
};

struct zone *zones; // vector de zone de memorie alocate prin vm_alloc
int num_zones = 0; // nr de zone de memorie
w_size_t page_size; // dimensiunea unei pagini virtuale

// functie de swap a unei pagini care nu mai are loc in RAM
void swap_page(int i, int page_from_swap, w_boolean_t swap_in) {

	char *buf;
	// luam prima pagina din RAM si o punem in SWAP
	int page_from_ram = zones[i].frames[0].index;

	// actualizam campurile zonei de memorie de unde vom lua pagina
	zones[i].pages[page_from_ram].state = STATE_IN_SWAP;
	zones[i].pages[page_from_ram].frame = NULL;

	// trecem continutul paginii din RAM in SWAP
	if(zones[i].pages[page_from_ram].dirty == TRUE || 
	   zones[i].pages[page_from_ram].prev_state == STATE_NOT_ALLOC) {

		buf = malloc(page_size*sizeof(char));
		// luam pagina din RAM
		w_set_file_pointer(zones[i].map.ram_handle, 0);
		w_read_file(zones[i].map.ram_handle, buf, page_size);
		// punem pagina in SWAP
		w_set_file_pointer(zones[i].map.swap_handle, page_from_ram*page_size);
		w_write_file(zones[i].map.swap_handle, buf, page_size);

		free(buf);
	}
	zones[i].pages[page_from_ram].prev_state = STATE_IN_RAM;

	mmap(zones[i].map.start + page_from_ram*page_size, page_size, 
		 PROT_NONE, MAP_FIXED | MAP_SHARED, 
		 zones[i].map.swap_handle, page_from_ram*page_size);

	// luam pagina din SWAP si o punem in RAM
	// actualizam campurile zonei de memorie unde vom pune pagina
	zones[i].pages[page_from_swap].state = STATE_IN_RAM;
	zones[i].pages[page_from_swap].dirty = FALSE;
	zones[i].pages[page_from_swap].protection = PROTECTION_READ;

	w_set_file_pointer(zones[i].map.ram_handle, 0);

	if(swap_in == TRUE) { // daca facem swap in

		buf = malloc(page_size * sizeof(char));

		w_set_file_pointer(zones[i].map.swap_handle, page_from_swap*page_size);
		// luam pagina din SWAP
		w_read_file(zones[i].map.swap_handle, buf, page_size);
		// punem pagina in RAM
		w_write_file(zones[i].map.ram_handle, buf, page_size);

		free(buf);
	} 
	else { // facem swap out
		buf = calloc(page_size, sizeof(char));
		// punem o pagina goala in RAM
		w_write_file(zones[i].map.ram_handle, buf, page_size);

		free(buf);
	}
	// actualizam campurile zonei de memorie unde vom pune pagina
	zones[i].frames[0].index = page_from_swap;
	zones[i].frames[0].pte = &(zones[i].pages[page_from_swap]);

	mmap(zones[i].map.start + page_from_swap*page_size, page_size, PROT_READ,
	MAP_FIXED | MAP_SHARED, zones[i].map.ram_handle, 0);
}

// functie pe post de handler de exceptie pentru tratarea semnalului SIGSEGV
void current_handler(int sig, siginfo_t *siginfo, void *aux) {

	int i, j, page;
	// identificam pagina si zona de memorie unde a avut loc SIGSEGV
	for(i=0; i<num_zones; i++) {
		page = ((w_ptr_t)siginfo->si_addr - zones[i].map.start) / page_size;
		if(page < zones[i].num_pages)
			break;
	}
	// vedem in ce stare se gaseste pagina de unde a avut loc SIGSEGV
	switch(zones[i].pages[page].state) {

		case STATE_IN_RAM: // daca este in RAM

			if(zones[i].pages[page].protection == PROTECTION_READ) {
				// schimbam drepturile paginii din READ in WRITE 
				zones[i].pages[page].protection = PROTECTION_WRITE;
				zones[i].pages[page].dirty = TRUE;	
			}
			w_protect_mapping(zones[i].map.start + page*page_size, 1, 
							  zones[i].pages[page].protection);

			break;

		case STATE_IN_SWAP: // daca este in SWAP

			swap_page(i, page, TRUE); // facem swap in

			break;

		case STATE_NOT_ALLOC: // daca zona nu a fost alocata

			// vedem daca putem gasi o zona libera in RAM pt pagina
			for(j=0; j<zones[i].num_frames; j++)
				if(zones[i].frames[j].pte == NULL)
					break;

			// daca a mai ramas loc in RAM se incearca plasarea paginii aici
			if(j != zones[i].num_frames) {

				// setam campurile zonei de memorie unde vom pune pagina
				zones[i].pages[page].state = STATE_IN_RAM;
				zones[i].pages[page].prev_state = STATE_NOT_ALLOC;
				zones[i].pages[page].dirty = FALSE;
				zones[i].pages[page].protection = PROTECTION_READ;
				zones[i].pages[page].frame = &(zones[i].frames[j]);

				zones[i].frames[j].index = page;
				zones[i].frames[j].pte = &(zones[i].pages[page]);

				w_set_file_pointer(zones[i].map.ram_handle, j*page_size);
				// punem pagina in RAM
				char *buf = calloc(page_size, sizeof(char));
				w_write_file(zones[i].map.ram_handle, buf, page_size);
				free(buf);

				mmap(zones[i].map.start + page*page_size, page_size, 
						 PROT_READ, MAP_FIXED | MAP_SHARED, 
						 zones[i].map.ram_handle, j*page_size);
			} else
				swap_page(i, page, FALSE); // facem swap out

			break;
	}
}

// functie de initializare a bibliotecii
w_boolean_t vmsim_init(void) {

	page_size = w_get_page_size(); // aflarea dimensiunii unei pagini virtuale

	// setarea handler-ului de exceptie pentru tratarea semnalului SIGSEGV
	return w_set_exception_handler(current_handler);
}

// functie de cleanup a bibliotecii
w_boolean_t vmsim_cleanup(void) {

	w_exception_handler_t previous_handler;
	// revenirea la handler-ul de exceptie initial
	w_get_previous_exception_handler(&previous_handler);

	// setarea handler-ului de exceptie pentru tratarea semnalului SIGSEGV
	return w_set_exception_handler(previous_handler);
}

// functie de alocare a unei zone de memorie
w_boolean_t vm_alloc(w_size_t num_pages, w_size_t num_frames, vm_map_t *map) {

	// verificam sa nu existe mai multe pagini fizice decat virtuale
	if(num_frames > num_pages)
		return FALSE;

	// generam un fisier temporar pe post de RAM
	char temp_ram[] = "ram_XXXXXX"; // numele fisierului temporar
	map->ram_handle = mkstemp(temp_ram);
	ftruncate(map->ram_handle, num_frames*page_size); // setam dimensiunea

	// generam un fisier temporar pe post de SWAP
	char temp_swap[] = "swap_XXXXXX"; // numele fisierului temporar
	map->swap_handle = mkstemp(temp_swap);	
	ftruncate(map->swap_handle, num_pages*page_size); // setam dimensiunea

	// cream adresa de start a zonei de memorie cu nr respectiv de pagini
	map->start = mmap(NULL, num_pages * page_size, 
					  PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	// alocam vectorul de zone de memorie pe masura ce sunt adaugate noi zone
	if(num_zones == 0)
		zones = malloc(sizeof(struct zone));
	else
		if(num_zones < 3)
			zones = realloc(zones, 2*(num_zones+1)*sizeof(struct zone));

	// copiem si setam campurile structurii noii zone de memorie
	zones[num_zones].map.start = map->start;
	zones[num_zones].map.ram_handle = map->ram_handle;
	zones[num_zones].map.swap_handle = map->swap_handle;

	zones[num_zones].num_pages = num_pages;
	zones[num_zones].pages = malloc(num_pages*sizeof(struct page_table_entry));

	int i;
	for(i=0; i<num_pages; i++) {
		// setam campurile structurii noii zone de memorie
		zones[num_zones].pages[i].state = STATE_NOT_ALLOC;
		zones[num_zones].pages[i].prev_state = STATE_NOT_ALLOC;
		zones[num_zones].pages[i].dirty = FALSE;
		zones[num_zones].pages[i].protection = PROTECTION_NONE;		
		zones[num_zones].pages[i].start = map->start + i*page_size;
		zones[num_zones].pages[i].frame = NULL;
	}
	// copiem si setam campurile structurii noii zone de memorie
	zones[num_zones].num_frames = num_frames;
	zones[num_zones].frames = malloc(num_frames * sizeof(struct frame));

	for(i=0; i<num_frames; i++)
		zones[num_zones].frames[i].pte = NULL;

	num_zones++; // incrementam nr de zone de memorie

	return TRUE;
}

// funtie de eliberare a unei zone de memorie
w_boolean_t vm_free(w_ptr_t start) {

	// verificam daca zona de memorie nu este deja eliberata
 	if (start == NULL)
 		return FALSE;

	int i;
	// cautam zona de memorie cu adresa de start respectiva
 	for(i=0; i<num_zones; i++)
		if(zones[i].map.start == start) {
			// se elibereaza zona respectiva de memorie
			munmap(start, zones[i].num_pages*page_size);
			// se inchid fisierele folosite pe post de RAM si SWAP
			w_close_file(zones[i].map.ram_handle);
			w_close_file(zones[i].map.swap_handle);	
 		}
 	return TRUE;
}

