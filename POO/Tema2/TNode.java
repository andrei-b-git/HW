/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

import javax.swing.tree.DefaultMutableTreeNode;

//clasa nod de arbore binar subclasa a clasei DefaultMutableTreeNode
public class TNode extends DefaultMutableTreeNode implements IBNode {
	public TNode(Object x) {   //creare nod frunza cu valoarea x
		super(x);	//apelarea constructorului clasei DefaultMutableTreeNode
	}
	public TNode(Object x,IBNode left,IBNode right) {	//creare nod cu valoarea x
		super(x);										//si fii left, right
		add((TNode)left);	//crearea elementului 0 din vectorul children al nodului
		add((TNode)right);	//crearea elementului 1 din vectorul children al nodului
	}
	public IBNode getLeft() {	//obtinere fiu stang
		return (IBNode)getChildAt(0);	//elementul 0 din vectorul children
	}
	public IBNode getRight() {	//obtinere fiu drept
		return (IBNode)getChildAt(1);	//elementul 1 din vectorul children
	}
	public Object getData() {	//obtinere camp data din nod
		return getUserObject();
	}
	public void setLeft(IBNode st) {	//setare fiu stang
		TNode y=(TNode)getChildAt(1);	//retinere fiu drept
		removeAllChildren();	//stergerea tuturor fiilor
		add((TNode)st);	  //creare fiu stang
		add(y);	  //creare fiu drept retinut anterior
	}
	public void setRight(IBNode dr) {	//setare fiu drept
		TNode y=(TNode)getChildAt(0);	//retinere fiu stang
		removeAllChildren();	//stergerea tuturor fiilor
		add(y);	  //creare fiu stang retinut anterior
		add((TNode)dr);	  //creare fiu drept
	}
	public boolean isLeaf() {	//daca nodul este nod frunza
		return super.isLeaf();	//apelarea aceleasi metode din superclasa
	}
}