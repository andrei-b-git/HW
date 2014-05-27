/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

import java.util.*;

public class ExpTree extends BTree {	//clasa pt arbori de expresii matematice
	private String postf;	//sir cu o expresie postfixata, cu spatii intre operanzi si operatori
	public ExpTree(String postf) {	//creare arbore dintr-o expresie postfixata
		this.postf=postf;
		StringTokenizer p=new StringTokenizer(postf," ");	//impartirea expresiei dupa spatii
		Stack st=new Stack();
		while(p.hasMoreElements()) {	//cat timp mai sunt tokenuri
			String s=p.nextElement().toString();	//retin tokenul curent
			if(isNumeric(s)==true)	//daca tokenul curent este un numar
				st.push(new TNode(s));	//introducere token in stiva ca nod de operand
			else {
				TNode l=(TNode)st.pop();	//scot ultimele 2 noduri de operanzi din stiva
				TNode r=(TNode)st.pop();
				st.push(new TNode(s,l,r));	//introduc un nod de operator in stiva cu fii
			}								//nodurile de operanzi scoase anterior
		}
		rad=(TNode)st.pop();	//setare radacina ca ultimul (si singurul) element din stiva
	}
	private boolean isNumeric(String str) {	  //daca un sir este numar sau nu
    	for (char c : str.toCharArray())	//parcurg sirul caracter cu caracter
        	if (!Character.isDigit(c))	//daca caracterul nu este cifra
        		return false;
    	return true;
	}
	public Double eval() {	//evaluare expresie dintr-un arbore binar
		return evalRec((TNode)getRoot());
	}
	private Double evalRec (TNode nod) {
		if(isNumeric(nod.getData().toString())==true)	//daca nodul contine un numar
			return Double.parseDouble(nod.getData().toString());	//rezultatul este numarul din nod
		else	//daca nodul contine un operator
			return calc(nod.getData().toString(),evalRec((TNode)nod.getRight()),evalRec((TNode)nod.getLeft()));
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