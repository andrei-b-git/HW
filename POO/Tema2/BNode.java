/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

public class BNode implements IBNode {	//clasa nod de arbore binar
	private Object x;	//data din nod
	private IBNode left;	//legatura catre fiul stang
	private IBNode right;	//legatura catre fiul drept
	public BNode(Object x){   //creare nod frunza cu valoarea x
		this.x=x;
		this.left=null;
		this.right=null;
	}
	public BNode(Object x,IBNode left,IBNode right) {	//creare nod cu valoarea x
		this.x=x;										//si fii left, right
		this.left=left;
		this.right=right;
	}
	public IBNode getLeft() {	//obtinere fiu stang
		return left;
	}
	public IBNode getRight() {	//obtinere fiu drept
		return right;
	}
	public Object getData() {	//obtinere camp data din nod
		return x;
	}
	public void setLeft(IBNode st) {	//setare fiu stang
		left=st;
	}
	public void setRight(IBNode dr) {	//setare fiu drept
		right=dr;
	}
	public void setData(Object x) {   //setare camp data din nod
		this.x=x;
	}
	public boolean isLeaf() {	//daca nodul este nod frunza
		return (left==null && right==null);
	}
}