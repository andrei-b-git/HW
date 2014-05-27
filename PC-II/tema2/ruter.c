/*	Tema 2 PC
 *
 *	nume: Bojin Andrei Florian
 *  grupa/seria: 333 CA
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "helpers.h"

int out, in, nod_id;
int timp = -1, seq_no;
int gata = FALSE;
// matrice in care retin topologia retelei de rutere si costurile drumurilor
int topologie[KIDS][KIDS];
// buffer de mesaje de tip LSA, tip M1
msg LSABuff[KIDS];
// cozi ce retin mesajele de procesat de tipul 6
msg coada_noua[BUFSIZE], coada_veche[BUFSIZE];
int cn_size, cv_size;
// structura suport in interiorul payloadului unui mesaj
struct load{
	int costuri[KIDS];
	int dest;
};

/* ATENTIE!!!
 * > 1. NU trebuie sa va preocupati voi de declansarea primului eveniment.
 * >> Acest lucru a fost deja implementat
 * > 2. NU trebuie sa va preocupati cand este triggered un eveniment
 * >> (esta gata implementat).
 * >> Trebuie doar sa le tratati: implementati functia procesare_eveniment()
 * > 3. NU trebuie sa va preocupe cronologia evenimentelor
 * >> Doar completati sectiunile pentru fiecare tip de mesaj primit
 * >> (acolo unde este indicat ca mai este ceva de implementat). */

// nu modificati numele, modalitatea de alocare si initializare a tabelei de rutare
// se foloseste la mesajele de tip 8/10 (deja implementate aici) si la logare (implementata in simulation_messages.c)
int tab_rutare [KIDS][2];

// Algoritmul Dijkstra de obtinere a drumului minim
void calcul_tabela_rutare() {
	// calculati tabela de rutare pornind de la topologie
	// folosind un algoritm de drum minim
    int vizitat[KIDS];
    int i, j, k;
	// Initializarea costurilor minime de la sursa la vecinii
    for(i=0; i<KIDS; i++) {
        vizitat[i] = FALSE;
        tab_rutare[i][0] = topologie[nod_id][i];
        if(tab_rutare[i][0] < DRUMAX)
            tab_rutare[i][1] = i;
        else
            tab_rutare[i][1] = -1;
    }
	// se marcheaza ca vizitat nodul sursa
    vizitat[nod_id] = TRUE;
    tab_rutare[nod_id][0] = 0;
    // se cauta nodul nevizitat cu costul minim
    for(i=1; i<KIDS; i++) {
        k = -1;
        for(j=0; j<KIDS; j++)
            if(!vizitat[j] && (k == -1 || tab_rutare[j][0] < tab_rutare[k][0]))
                k = j;
        
        vizitat[k] = TRUE;
		// se actualizeaza tabela de rutare in urma recalcularii costurilor
        for(j=0; j<KIDS; j++) {
            if(!vizitat[j] &&
            	tab_rutare[j][0] > tab_rutare[k][0] + topologie[k][j]) {

                tab_rutare[j][0] = tab_rutare[k][0] + topologie[k][j];
                tab_rutare[j][1] = tab_rutare[k][1];
            }
        }
    }
}

void procesare_eveniment(msg mevent) {

	if (mevent.add == TRUE) {
		printf ("Nod %d, msg tip eveniment - am aderat la topologie la pasul %d\n", nod_id, timp); 
	}
	else
		printf ("Timp %d, Nod %d, procesare eveniment\n", timp, nod_id);

	// aveti de implementat tratarea evenimentelor si trimiterea mesajelor ce tin de protocolul de rutare
	// in campul payload al mesajului de tip 7 e linia de fisier corespunzatoare respectivului eveniment
	// (optional) vezi simulation_messages.c, functia announceNewEvent() si io_lib.c, functia readEventsFromFile()

	// rutere direct implicate in evenimente, care vor primi mesaje de tip 7 de la simulatorul central:
	// eveniment tip E1: ruterul nou adaugat la retea  (event_t.router1  - vezi functiile indicate)
	// eveniment tip E2: capetele noului link (event.router1 si event_t.router2)
	// eveniment tip E3: capetele linkului suprimat (event_t.router1 si event_t.router2)
	// eveniment tip E4:  ruterul sursa al pachetului (event_t.router1)

	int i, ev_type;
	msg t;
	struct load aux_load;
	// se initializeaza costurile drumurilor dintre vecini din structura din payload
	for(i=0; i<KIDS; i++)
		aux_load.costuri[i] = DRUMAX;	
	// se extrage tipul evenimentului
	ev_type = atoi(strtok(mevent.payload, " "));
	// se trateaza fiecare tip de eveniment
	switch(ev_type) {
		// adaugare ruter
		case E1: {
			int id_ruter_nou, nrvec, vecin, cost;
			// se parseaza inputul
			id_ruter_nou = atoi(strtok(NULL, " "));
			nrvec = atoi(strtok(NULL, " "));
			// se creaza un mesaj de tip Database Request
			t.msg_type = 2;
			t.creator = id_ruter_nou;
			t.sender = id_ruter_nou;
			t.time = timp;
			t.add = FALSE;
			t.length = sizeof(struct load);	
			// se trimite mesajul creat cu payloadul constituit din costurile vecinilor
			for(i=0; i<nrvec; i++) {

				t.seq_no = seq_no ++;

				vecin = atoi(strtok(NULL, " "));
				cost = atoi(strtok(NULL, " "));

				t.next_hop = vecin;
				aux_load.costuri[vecin] = cost;
			
				memcpy(t.payload, &aux_load, t.length);
				write(out, &t, sizeof(msg));
			}
			break;
		} 
		// adaugare link intre 2 rutere
		case E2: {
			int ruter1, ruter2, cost;
			// se parseaza inputul
			ruter1 = atoi(strtok(NULL, " "));
			ruter2 = atoi(strtok(NULL, " "));
			cost = atoi(strtok(NULL, " "));
			// se actualizeaza topologia cu noul cost
			topologie[ruter1][ruter2] = cost;
			topologie[ruter2][ruter1] = cost;
			// se creaza un mesaj de tip Database Request
			t.msg_type = 2;
			t.seq_no = seq_no ++;
			t.time = timp;
			t.add = FALSE;
			t.length = sizeof(struct load);	
			// se trateaza cazul in care nodul curent este ruter1
			if(nod_id == ruter1) {
				t.creator = ruter1;
				t.sender = ruter1;
				t.next_hop = ruter2;
				aux_load.costuri[ruter2] = cost;
			}
			// se trateaza cazul in care nodul curent este ruter2
			if(nod_id == ruter2) {
				t.creator = ruter2;
				t.sender = ruter2;
				t.next_hop = ruter1;
				aux_load.costuri[ruter1] = cost;
			}
			// se trimite mesajul creat cu payloadul constituit din costurile vecinilor
			memcpy(t.payload, &aux_load, t.length);
			write(out, &t, sizeof(msg));

			break;
		}
		// suprimare link intre 2 rutere
		case E3: {
			int ruter1, ruter2;
			// se parseaza inputul
			ruter1 = atoi(strtok(NULL, " "));
			ruter2 = atoi(strtok(NULL, " "));
			// se actualizeaza topologia, stergandu-se link-ul dintre cele 2 rutere
			topologie[ruter1][ruter2] = DRUMAX;
			topologie[ruter2][ruter1] = DRUMAX;
			// se creaza un mesaj de tip LSA
			t.msg_type = 1;
			t.seq_no = seq_no ++;
			t.creator = nod_id;
			t.sender = nod_id;
			t.time = timp;
			t.add = FALSE;
			t.length = sizeof(struct load);	
			// se retine topologia actualizata
			for(i=0; i<KIDS; i++)
				aux_load.costuri[i] = topologie[nod_id][i];

			memcpy(t.payload, &aux_load, t.length);
			// se trimite mesajul LSA doar vecinilor nodului curent
			for(i=0; i<KIDS; i++)
				if(topologie[nod_id][i] != DRUMAX) {
					t.next_hop = i;
					write(out, &t, sizeof(msg));
				}
			// se buffereaza mesajul LSA trimis
			LSABuff[nod_id] = t;

			break;
		}
		// dirijare pachet
		case E4: {
			int sursa, destinatie;
			// se parseaza inputul
			sursa = atoi(strtok(NULL, " "));
			destinatie = atoi(strtok(NULL, " "));
			// se creaza un mesaj de tip Pachet de Date
			t.msg_type = 4;
			t.seq_no = seq_no ++;
			t.creator = sursa;
			t.sender = nod_id;
			t.next_hop = tab_rutare[destinatie][1];
			t.time = timp;
			t.add = FALSE;
			t.length = sizeof(struct load);
			// se trimite mai departe mesajul creat
			aux_load.dest = destinatie;
			memcpy(t.payload, &aux_load, t.length);
			if(t.next_hop != -1)
				write(out, &t, sizeof(msg));
			break;
		}
	}
}

int main (int argc, char ** argv) {
	msg mesaj, mesaj_event;
	int fd_status, k, i, j;
	int event = FALSE;

	out = atoi(argv[1]);  // legatura pe care se trimit mesaje catre simulatorul central (toate mesajele se trimit pe aici)
	in = atoi(argv[2]);   // legatura de pe care se citesc mesajele

	nod_id = atoi(argv[3]); // procesul curent participa la simulare numai dupa ce nodul cu ID-ul lui este adaugat in topologie

	// tab_rutare[k][0] reprezinta costul drumului minim de la ruterul curent (nod_id) la ruterul k
	// tab_rutare[k][1] reprezinta next_hop pe drumul minim de la ruterul curent (nod_id) la ruterul k
	for (k = 0; k < KIDS; k++) {
		tab_rutare[k][0] = DRUMAX;  // drum =DRUMAX daca ruterul k nu e in retea sau informatiile despre el nu au ajuns la ruterul curent
		tab_rutare[k][1] = -1; // in cadrul protocolului(pe care il veti implementa), next_hop =-1 inseamna ca ruterul k nu e (inca) cunoscut de ruterul nod_id (vezi mai sus)
		if (k == nod_id)
			tab_rutare [k][0] = 0;
		// se initializeaza topologia
		for(i=0; i<KIDS; i++)
			topologie[k][i] = DRUMAX;
		topologie[k][k] = 0;
		// se initializeaza bufferul de mesaje de tip LSA
		LSABuff[k].msg_type = -1;
		LSABuff[k].seq_no = -1;
		LSABuff[k].time = -1;
	}

	printf ("Nod %d, pid %u alive & kicking\n", nod_id, getpid());

	/* nodul/ruterul 0 declanseaza toate evenimentele */
	/* !!! nu este nimic de facut aici !!! */
	if (nod_id == 0) { // sunt deja in topologie
		timp = -1; // la momentul 0 are loc primul eveniment
		mesaj.msg_type = 5; // finish procesare mesaje timp -1
		mesaj.sender = nod_id;
		write (out, &mesaj, sizeof(msg)); 
		printf ("TRIMIS Timp %d, Nod %d, msg tip 5 - terminare procesare mesaje vechi din coada\n", timp, nod_id);

	}

	while (!gata) {
		fd_status = read(in, &mesaj, sizeof(msg));

		if (fd_status <= 0) {
			printf ("Adio, lume cruda. Timp %d, Nod %d, msg tip %d cit %d\n", timp, nod_id, mesaj.msg_type, fd_status);
			exit (-1);
		}

		switch (mesaj.msg_type) {

		// 1,2,3,4 sunt mesaje din protocolul link state;
		// actiunea imediata corecta la primirea unui pachet de tip 1,2,3,4 este buffer-area
		// (punerea in coada /coada new daca sunt 2 cozi - vezi enunt)

		case M1:
			//printf ("Timp %d, Nod %d, msg tip 1 - LSA\n", timp, nod_id);
			coada_noua[cn_size ++] = mesaj; // se pune in coada mesajul pentru mai tarziu
			break;

		case M2:
			//printf ("Timp %d, Nod %d, msg tip 2 - Database Request\n", timp, nod_id);
			coada_noua[cn_size ++] = mesaj; // se pune in coada mesajul pentru mai tarziu
			break;

		case M3:
			//printf ("Timp %d, Nod %d, msg tip 3 - Database Reply\n", timp, nod_id);
			coada_noua[cn_size ++] = mesaj; // se pune in coada mesajul pentru mai tarziu
			break;

		case M4:
			//printf ("Timp %d, Nod %d, msg tip 4 - pachet de date (de rutat)\n", timp, nod_id);
			coada_noua[cn_size ++] = mesaj; // se pune in coada mesajul pentru mai tarziu
			break;

		case M6:
			// complet in ceea ce priveste partea cu mesajele de control
			// puteti inlocui conditia de coada goala/mesaje_vechi, ca sa corespunda cu implementarea personala
			// aveti de implementat procesarea mesajelor ce tin de protocolul de rutare
		{

			// printf ("Timp %d, Nod %d, msg tip 6 - incepe procesarea mesajelor puse din coada la timpul anterior (%d)\n", timp, nod_id, timp-1);

			// in scheletul de cod nu exista (inca) o coada
			int mesaje_vechi = TRUE;

			msg t, r, t1;
			struct load aux_load;
			// se copie coada noua in cea veche
			for(i=0; i<cn_size; i++)
				coada_veche[i] = coada_noua[i];

			cv_size = cn_size;
			cn_size = 0;
			i = 0;

			// cat timp mai exista mesaje venite la timpul anterior
			while (mesaje_vechi && i<cv_size) {
				// procesez toate mesajele venite la timpul anterior
				// (sau toate mesajele primite inainte de inceperea timpului curent - marcata de mesaj de tip 6)
				// la acest pas/timp NU se vor procesa mesaje venite DUPA inceperea timpului curent
				// cand trimiteti mesaje de tip 4 nu uitati sa setati (inclusiv) campurile, necesare pt logare:
				// mesaj.time, mesaj.creator,mesaj.seq_no, mesaj.sender, mesaj.next_hop
				// la tip 4 - creator este sursa initiala a pachetului rutat

				t = coada_veche[i]; // se extrage un mesaj din coada veche

				switch (t.msg_type) {	// se proceseaza mesajul dupa tip
					// mesaj de tip LSA
					case 1:
						// daca mesajul LSA de procesat este mai nou decat cel din bufferul LSA
						if(t.seq_no > LSABuff[t.creator].seq_no) {
							// actualizez bufferul de mesaje LSA
							LSABuff[t.creator] = t;
							// actualizez topologia
							memcpy(&aux_load, t.payload, sizeof(struct load));
							for(j=0; j<KIDS; j++) {
								topologie [t.creator][j] = aux_load.costuri[j];
								topologie [j][t.creator] = aux_load.costuri[j];
							}
							// trimit mesajul tuturor vecinilor mai putin senderului
							for(j=0; j<KIDS; j++) {
								if(topologie[nod_id][j] != DRUMAX && j != t.sender) {

									t.sender = nod_id;
									t.next_hop = j;
									write(out, &t, sizeof (msg));
								}
							}
						}
						break;
					// mesaj de tip Database Request
					case 2:

						r.seq_no = seq_no ++;
						// creez un mesaj de tip LSA
						t1.msg_type = 1;
						t1.seq_no = seq_no ++;
						t1.creator = nod_id;						
						t1.sender = nod_id;
						t1.time = timp;
						t1.add = FALSE;
						t1.length = sizeof(struct load);
						// creez un mesaj tip Database Reply
						for(j=0; j<KIDS; j++) {

							if (LSABuff[j].msg_type != -1) {

								r = LSABuff[j];
								
								r.msg_type = 3;								
								r.sender = nod_id;
								// trimit mesajul senderului
								r.next_hop = t.sender;								
								r.time = timp;
								r.add = FALSE;
								
								write(out, &r, sizeof(msg));
							}
							// actualizez lista costurilor de trimis in payload
							aux_load.costuri[j] = topologie[nod_id][j];
						}

						memcpy(t1.payload, &aux_load, t1.length);
						// trimit mesajul LSA tuturor vecinilor
						for(j=0; j<KIDS; j++) {
							if(topologie[nod_id][j] != DRUMAX) {
								t1.next_hop = j;
								write(out, &t1, sizeof(msg));
							}
						}
						// actualizez bufferul de mesaje LSA
						LSABuff[nod_id] = t1;
						// actualizez topologia
						memcpy(&aux_load, t.payload, sizeof(struct load));
						topologie[t.creator][nod_id] = aux_load.costuri[nod_id];
						topologie[nod_id][t.creator] = aux_load.costuri[nod_id];

						break;
					// mesaj de tip Database Reply
					case 3:
						// daca mesajul LSA de procesat este mai nou decat cel din bufferul LSA
						if(t.seq_no > LSABuff[t.creator].seq_no) {
							// actualizez bufferul de mesaje LSA
							t.msg_type = 1;
							LSABuff[t.creator] = t;

							memcpy(&aux_load, t.payload, sizeof(struct load));
							// actualizez topologia
							for(j=0; j<KIDS; j++) {
								topologie[t.creator][j] = aux_load.costuri[j];
								topologie[j][t.creator] = aux_load.costuri[j];
							}
						}
						break;
					// mesaj de tip Pachet de Date
					case 4:

						memcpy(&aux_load, t.payload, sizeof(struct load));
						// daca destinatia nu este nodul curent
						if(aux_load.dest != nod_id) {
							// daca exista drum intre nodul curent si destinatie
							if(topologie[nod_id][aux_load.dest] != DRUMAX) {
								// trimit mesajul mai departe
								t.sender = nod_id;
								t.next_hop = tab_rutare[aux_load.dest][1];
								t.time = timp;
								write(out, &t, sizeof(msg));
							}
						}
						break;
				}
				// cand coada devine goala sau urmatorul mesaj are timpul de trimitere == pasul curent de timp:
				mesaje_vechi = FALSE;
				i ++; // se trece la urmatorul mesaj din coada veche
			}
			// se reinitializeaza dimensiunea cozii vechi				
			cv_size = 0;

			// procesez mesaj eveniment
			if (event) {
				procesare_eveniment(mesaj_event);
				event = FALSE;
			}

			// calculez tabela de rutare
			calcul_tabela_rutare();

			// nu mai sunt mesaje vechi, am procesat evenimentul(daca exista), am calculat tabela de rutare(in aceasta ordine)
			// trimit mesaj de tip 5 (terminare pas de timp)
			mesaj.msg_type = 5;
			mesaj.sender = nod_id;
			write (out, &mesaj, sizeof(msg));
		}
		break;

		case M7:
			// complet implementat - nu modificati! (exceptie afisari on/off)
			// aici se trateaza numai salvarea mesajului de tip eveniment(acest mesaj nu se pune in coada), pentru a fi procesat la finalul acestui pas de timp
			// procesarea o veti implementa in functia procesare_eveniment(), apelata in case 6
		{
			event = TRUE;
			memcpy (&mesaj_event, &mesaj, sizeof(msg));

			if (mesaj.add == TRUE)
				timp = mesaj.time+1; // initializam timpul pentru un ruter nou
		}
		break;

		case M8:
			// complet implementat - nu modificati! (exceptie afisari on/off)
		{
			// printf ("Timp %d, Nod %d, msg tip 8 - cerere tabela de rutare\n", timp, nod_id);

			mesaj.msg_type = 10;  // trimitere tabela de rutare
			mesaj.sender = nod_id;
			mesaj.time = timp;
			memcpy (mesaj.payload, &tab_rutare, sizeof (tab_rutare));
			// Observati ca acest tip de mesaj (8) se proceseaza imediat - nu se pune in nicio coada (vezi enunt)
			write (out, &mesaj, sizeof(msg));
			timp++;
		}
		break;

		case M9:
			// complet implementat - nu modificati! (exceptie afisari on/off)
		{
			// Aici poate sa apara timp -1 la unele "noduri"
			// E ok, e vorba de procesele care nu reprezentau rutere in retea, deci nu au de unde sa ia valoarea corecta de timp
			// Alternativa ar fi fost ca procesele neparticipante la simularea propriu-zisa sa ramana blocate intr-un apel de read()
			printf ("Timp %d, Nod %d, msg tip 9 - terminare simulare\n", timp, nod_id);
			gata = TRUE;
		}
		break;


		default:
			printf ("\nEROARE: Timp %d, Nod %d, msg tip %d - NU PROCESEZ ACEST TIP DE MESAJ\n", timp, nod_id, mesaj.msg_type);
			exit (-1);
		}			
	}

	return 0;
}
