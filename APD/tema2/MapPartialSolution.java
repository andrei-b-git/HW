/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

/**
 * Clasa ce reprezinta o solutie partiala pentru rezolvarea functionalitatii MAP. 
 * Aceste solutii partiale constituie task-uri care sunt introduse in workpoolul
 * MapWorkPool
 */
public class MapPartialSolution {

	public String fragment;		//un fragment al doc pe care se face indexarea

	public MapPartialSolution (String fragment) {
		this.fragment = fragment;
	}

	public String toString() {
		return fragment;
	}
}
