/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

import java.io.IOException;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 * Clasa ce reprezinta un thread MapWorker.
 */
public class MapWorker extends Thread {

	MapWorkPool mwp;
	HashMap<String, Integer> hm;

	public MapWorker(MapWorkPool workpool) {
		this.mwp = workpool;
		hm = new HashMap<String, Integer>();
	}

	/**
	 * Procesarea unei solutii partiale. Aceasta poate implica generarea unor
	 * noi solutii partiale care se adauga in workpool folosind putWork().
	 * Daca s-a ajuns la o solutie finala, aceasta va fi afisata.
	 * @throws IOException 
	 */
	void processMapPartialSolution(MapPartialSolution mps) throws IOException {

		String fragment, token;
		//salvez fragmentul
		fragment = mps.fragment;
		//prelucrez fragmentul
		StringTokenizer stk = new StringTokenizer(fragment, 
												",.-+*~`'\"\\!?;:() \t\n\r\f");
		while (stk.hasMoreTokens()) {
			token = stk.nextToken().toLowerCase();
			if (hm.containsKey(token) == true) {
				hm.put(token, hm.get(token)+1);
			}
			else {
				hm.put(token, 1);
			}
		}
	}

	public void run() {
		while (true) {
			MapPartialSolution mps = mwp.getWork();
			if (mps == null)
				break;
			
			try {
				processMapPartialSolution(mps);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}