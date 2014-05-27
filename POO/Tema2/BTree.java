/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

public class BTree implements IBTree {	//clasa de arbore binar
	protected IBNode rad;	//radacina arborelui, accesibila in subclase
	public BTree() {
		rad=null;	//initializare radacina
	}
	@Override
	public int size() {	  //numarul de noduri din arbore
		return size(getRoot());
	}
	private int size(IBNode nod) {
		int c=1;	//contor
		if(nod.isLeaf()==false) {
			c+=size(nod.getLeft());
			c+=size(nod.getRight());
		}
		return c;
	}
	public IBNode getRoot() {	//radacina arborelui
		return rad;
	}
	@Override
	public String toString() {	//creare sir cu datele din arbore, cate o valoare pe linie,
		return toString(getRoot(),1);	//in ordine prefixata si cu indentare diferita pe nivel
	}
	private String toString(IBNode nod,int indentLevel) {
		String rez=nod.getData().toString();	//retinerea datei din nodul curent
		int i;
		if(nod.isLeaf()==false) {
			rez+="\n";	//trecerea la un nou nod
			for(i=0;i<indentLevel;i++)	//indentarea fiecarui nivel
				rez+="\t";
			rez+=toString(nod.getRight(),indentLevel+1);	//indentarea creste odata cu nivelul
			rez+="\n";	//trecerea la un nou nod
			for(i=0;i<indentLevel;i++)	//indentarea fiecarui nivel
				rez+="\t";
			rez+=toString(nod.getLeft(),indentLevel+1);	  //indentarea creste odata cu nivelul
		}
		return rez;
	}
}