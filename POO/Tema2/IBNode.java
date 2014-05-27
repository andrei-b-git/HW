/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

public interface IBNode {	//interfata clasei de nod de arbore binar
	IBNode getLeft();	//obtinere fiu stang
	IBNode getRight();	//obtinere fiu drept
	Object getData();	//obtinere camp data din nod
	void setLeft(IBNode st);	//setare fiu stang
	void setRight(IBNode dr);	//setare fiu drept
	boolean isLeaf();	//daca nodul este nod frunza
}