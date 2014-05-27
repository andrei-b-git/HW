/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

import java.util.LinkedList;

/**
 * Clasa ce implementeaza un "work pool" conform modelului "replicated workers",
 * necesar rezolvarii functionalitatii MAP. Task-urile introduse in work pool 
 * sunt obiecte de tipul MapPartialSolution.
 */
public class MapWorkPool {

	int nThreads; // nr total de thread-uri MapWorker
	int nWaiting = 0; // nr de thread-uri MapWorker care sunt blocate asteptand
	public boolean ready = false; // daca s-a terminat rezolvarea problemei 

	//task-urile continand solutiile partiale ce treb rezolvate
	LinkedList<MapPartialSolution> tasks = new LinkedList<MapPartialSolution>();

	/**
	 * Constructor pentru clasa MapWorkPool.
	 * @param nThreads - numarul de thread-uri MapWorker
	 */
	public MapWorkPool(int nThreads) {
		this.nThreads = nThreads;
	}

	/**
	 * Functie care incearca obtinerea unui task din MapWorkpool.
	 * Daca nu sunt task-uri disponibile, functia se blocheaza pana cand 
	 * poate fi furnizat un task sau pana cand rezolvarea problemei este complet
	 * terminata
	 * @return Un task de rezolvat, sau null daca problema e rezolvata
	 */
	public synchronized MapPartialSolution getWork() {

		if (tasks.size() == 0) { // MapWorkpool gol
			nWaiting++;
			/* condtitie de terminare:
			 * nu mai exista nici un task in MapWorkpool si nici un MapWorker
			 * nu e activ
			 */
			if (nWaiting == nThreads) {
				ready = true;
				/* problema s-a terminat, anunt toti ceilalti MapWorkeri */
				notifyAll();
				return null;
			}
			else {
				while (!ready && tasks.size() == 0) {
					try {
						this.wait();
					} catch(Exception e) {e.printStackTrace();}
				}

				if (ready)
					/* s-a terminat prelucrarea */
					return null;

				nWaiting--;

			}
		}
		return tasks.remove();
	}

	/**
	 * Functie care introduce un task in MapWorkpool.
	 * @param sp - task-ul care trebuie introdus 
	 */
	synchronized void putWork(MapPartialSolution sp) {

		tasks.add(sp);
		/* anuntam unul dintre MapWorkerii care asteptau */
		this.notify();
	}

}
