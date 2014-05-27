/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/
package Tema2_APD;

/**
 * Clasa ce reprezinta o solutie partiala pentru rezolvarea functionalitatii
 * REDUCE. Aceste solutii partiale constituie task-uri care sunt introduse in 
 * workpoolul ReduceWorkPool
 */
public class ReducePartialSolution {

	public float f1;	//frecventa de aparitie a unui termen in primul doc
	public float f2;	//frecventa de aparitie a unui termen in al doilea doc

	public ReducePartialSolution (float f1, float f2) {
		this.f1 = f1;
		this.f2 = f2;
	}

	public String toString() {
		return Float.toString(f1)+" "+Float.toString(f2);
	}
}
