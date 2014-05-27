package Tema2_APD;

import java.util.LinkedList;

/**
 * Clasa ce implementeaza un "work pool" conform modelului "replicated workers",
 * necesar rezolvarii functionalitatii REDUCE. Task-urile introduse in work pool 
 * sunt obiecte de tipul ReducePartialSolution.
 */
public class ReduceWorkPool {

	int nThreads; // nr total de thread-uri ReduceWorker
	int nWaiting = 0; // nr de thread-uri ReduceWorker care sunt blocate asteptand
	public boolean ready = false; // daca s-a terminat rezolvarea problemei 

	//task-urile continand solutiile partiale ce treb rezolvate
	LinkedList<ReducePartialSolution> tasks =
									new LinkedList<ReducePartialSolution>();

	/**
	 * Constructor pentru clasa ReducepWorkPool.
	 * @param nThreads - numarul de thread-uri ReduceWorker
	 */
	public ReduceWorkPool(int nThreads) {
		this.nThreads = nThreads;
	}

	/**
	 * Functie care incearca obtinerea unui task din ReduceWorkpool.
	 * Daca nu sunt task-uri disponibile, functia se blocheaza pana cand 
	 * poate fi furnizat un task sau pana cand rezolvarea problemei este complet
	 * terminata
	 * @return Un task de rezolvat, sau null daca problema e rezolvata
	 */
	public synchronized ReducePartialSolution getWork() {

		if (tasks.size() == 0) { // ReduceWorkpool gol
			nWaiting++;
			/* condtitie de terminare:
			 * nu mai exista nici un task in ReduceWorkpool si nici un 
			 * ReduceWorker nu e activ
			 */
			if (nWaiting == nThreads) {
				ready = true;
				/* problema s-a terminat, anunt toti ceilalti ReduceWorkeri */
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
	 * Functie care introduce un task in ReduceWorkpool.
	 * @param sp - task-ul care trebuie introdus 
	 */
	synchronized void putWork(ReducePartialSolution sp) {

		tasks.add(sp);
		/* anuntam unul dintre ReduceWorkerii care asteptau */
		this.notify();
	}

}
