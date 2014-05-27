/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

import java.util.*;

public class BSTree extends BTree {	  //clasa pt arbori binari de cautare de expresii matematice
	private String postf;	//sir cu o expresie postfixata, cu spatii intre operanzi si operatori
	private Hashtable h1;
	public BSTree(String postf) {	//creare arbore binar de cautare dintr-o expresie postfixata
		this.postf=postf;
		h1=new Hashtable();
		Hashtable h2=new Hashtable();
		int k=0;	//numarul de operanzi/operatori si totodata cheia asociata
		StringTokenizer p=new StringTokenizer(postf," ");	//impartirea expresiei dupa spatii
		Stack st=new Stack();
		while(p.hasMoreElements()) {	//cat timp mai sunt tokenuri
			k++;
			String s=p.nextElement().toString();	//retin tokenul curent
			if(isNumeric(s)==true) 	//daca tokenul curent este un numar
				st.push(s+"|"+k);	  //introducere token in stiva ca operand + cheia asociata
			else {
				String r=(String)st.pop();	 //scot ultimii 2 operanzi din stiva
				String l=(String)st.pop();
				String exp=l+" "+s+"|"+k+" "+r;
				st.push(exp);	//introduc in stiva o expresie formata din operanzii scosi anterior,
			}					//operatorul curent si cheia asociata acestuia
		}
		StringTokenizer q=new StringTokenizer((String)st.pop()," ");	//impartirea expresiei dupa spatii
		int i=k;	//numarul de operanzi/operatori si totodata cheia asociata
		while(q.hasMoreElements()) {	//cat timp mai sunt tokenuri
			String s=q.nextElement().toString();	//retin tokenul curent
			StringTokenizer r=new StringTokenizer(s,"|");	//impartirea expresiei dupa |
			h1.put(i,r.nextElement());	//asocierea unor chei cu operatorii/operanzii expresiei
			h2.put(r.nextElement(),i);	//asocierea cheilor retinute prima data cu valori
			i--;
		}
		Object v[]=h2.values().toArray();	//convertirea colectiei de valori la un vector
		for(i=0;i<k;i++)
			add(v[i]);	//construirea unui arbore binar de cautare de chei
		convert((BNode)getRoot());	//parcurgerea arborelui si inlocuirea cheilor cu valorile
	}
	private void convert(BNode nod) {	//parcurgere inordine a arborelui
		if(nod.isLeaf()==false) {
			convert((BNode)nod.getLeft());
			nod.setData(h1.get(nod.getData()));
			convert((BNode)nod.getRight());
		}
		else
			nod.setData(h1.get(nod.getData()));
	}
	public void add(Object x) {	  //adauga un nod cu valoarea x la acest arbore
		rad=add(x,(BNode)getRoot());	//reinitializarea radacinii cu noul arbore
	}
	private BNode add(Object x,BNode nod) {
		if(nod==null)
			nod=new BNode(x);	//creare nod nou cu valoarea x
        else
        	if(x.toString().compareTo(nod.getData().toString())<0)	//x < valoarea din nod
            	nod.setLeft(add(x,(BNode)nod.getLeft()));	//nodul nou este in subarborele stang
        	else
        		if(x.toString().compareTo(nod.getData().toString())>0)	//x > valoarea din nod
            		nod.setRight(add(x,(BNode)nod.getRight()));	//nodul nou este in subarborele drept
        return nod;
	}
	public IBNode find(Object x) {	 //cauta valoarea x in acest arbore
		return (IBNode)find(x,(BNode)getRoot());
	}
	private BNode find(Object x,BNode nod) {
		while(nod!=null) {
			if(x.toString().compareTo(nod.getData().toString())<0)	//x < valoarea din nod
				nod=(BNode)nod.getLeft();	//caut in subarborele stang
			else
				if(x.toString().compareTo(nod.getData().toString())>0)	//x > valoarea din nod
					nod=(BNode)nod.getRight();	//caut in subarborele drept
				else
					return nod;
		}
		return null;
	}
	private boolean isNumeric(String str) {	  //daca un sir este numar sau nu
    	for (char c : str.toCharArray())	//parcurg sirul caracter cu caracter
        	if (!Character.isDigit(c))	//daca caracterul nu este cifra
        		return false;
    	return true;
	}
	public Double eval() {	//evaluare expresie dintr-un arbore binar
		return evalRec((BNode)getRoot());
	}
	public Double evalRec (BNode nod) {
		if(isNumeric(nod.getData().toString())==true)	//daca nodul contine un numar
			return Double.parseDouble(nod.getData().toString());	//rezultatul este numarul din nod
		else	//daca nodul contine un operator
			return calc(nod.getData().toString(),evalRec((BNode)nod.getRight()),evalRec((BNode)nod.getLeft()));
	}
	private Double calc(String o,Double l,Double r) {	//aplicarea operatorului (+,-,*,/) asupra operanzilor
		if(o.compareTo("+")==0)
			return l+r;
		if(o.compareTo("-")==0)
			return l-r;
		if(o.compareTo("*")==0)
			return l*r;
		if(o.compareTo("/")==0)
			return l/r;
		return 0.0;
	}
	public String infix() {	  //sir cu expresia complet parantezata (infixat cu paranteze complete)
		StringTokenizer p=new StringTokenizer(postf," ");	//impartirea expresiei dupa spatii
		Stack st=new Stack();
		while(p.hasMoreElements()) {	//cat timp mai sunt tokenuri
			String s=p.nextElement().toString();	//retin tokenul curent
			if(isNumeric(s)==true) 	//daca tokenul curent este un numar
				st.push(s);	  //introducere token in stiva ca operand
			else {
				String r=(String)st.pop();	 //scot ultimii 2 operanzi din stiva
				String l=(String)st.pop();
				String exp="("+l+s+r+")";
				st.push(exp);	//introduc in stiva o expresie formata din operanzii scosi anterior,
			}					//operatorul curent si parantezele asociate operatiei
		}
		return (String)st.pop();	//rezultatul este ultimul (si singurul) element din stiva
	}
}