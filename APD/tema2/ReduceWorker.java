/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

import java.io.IOException;

/**
 * Clasa ce reprezinta un thread ReduceWorker.
 */
public class ReduceWorker extends Thread {

	ReduceWorkPool rwp;
	float sim;

	public ReduceWorker(ReduceWorkPool workpool) {
		this.rwp = workpool;
		sim = 0;
	}

	/**
	 * Procesarea unei solutii partiale. Aceasta poate implica generarea unor
	 * noi solutii partiale care se adauga in workpool folosind putWork().
	 * Daca s-a ajuns la o solutie finala, aceasta va fi afisata.
	 * @throws IOException 
	 */
	void processReducePartialSolution(ReducePartialSolution rps) 
														throws IOException {
		float f1, f2;

		//salvez floaturile
		f1 = rps.f1;
		f2 = rps.f2;

		//calculez gradul de similaritate
		sim += f1 * f2;
	}

	public void run() {
		while (true) {
			ReducePartialSolution rps = rwp.getWork();
			if (rps == null)
				break;
			
			try {
				processReducePartialSolution(rps);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}