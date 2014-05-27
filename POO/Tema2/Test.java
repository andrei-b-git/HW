/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

public class Test {   //clasa pt teste
	public static void main(String args[]) {
		ExpTree t=new ExpTree("7 50 40 + 60 30 - / +");	  //creare arbore expresie
		System.out.println("Afisare arbore expresie:\n"+t.toString());	//afisare arbore
		System.out.println("Numarul de noduri din arbore este:\n"+t.size());	//numar noduri din arbore
		System.out.println("Radacina arborelui este:\n"+t.getRoot());	  //radacina arborelui
		System.out.println("Expresia infixata cu paranteze complete este:\n"+t.infix());
		System.out.println("Rezultatul expresiei este:\n"+t.eval());	//rezultat expresie
		BSTree b=new BSTree("7 50 40 + 60 30 - / +");	//creare arbore binar de cautare tip expresie
		System.out.println("Afisare arbore binar de cautare tip expresie:\n"+b.toString());
		System.out.println("Elementul 7 se gaseste in expresie?\n"+(b.find("7").getData()!=null ? "Da." : "Nu."));
	}
}

/*
OUTPUT:

Afisare arbore expresie:
+
    7
    /
        +
            50
            40
        -
            60
            30
Numarul de noduri din arbore este:
9
Radacina arborelui este:
+
Expresia infixata cu paranteze complete este:
(7+((50+40)/(60-30)))
Rezultatul expresiei este:
10.0
Afisare arbore binar de cautare tip expresie:
+
    7
    /
        +
            50
            40
        -
            60
            30
Elementul 7 se gaseste in expresie?
Da.
*/