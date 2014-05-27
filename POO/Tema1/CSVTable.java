/* Tema1: Utilitar pentru operatii cu tabele si fisiere CSV
 * nume/prenume: Bojin Andrei Florian
 * grupa/seria: 323 CA
 */

package CSVTable;

import java.io.*;
import java.util.*;

public class CSVTable {
	private Object a[][];
	public CSVTable () throws IOException {	// tabelul va fi creat cu date dintr-un fisier
		readCSV("in.csv");
	}
	public CSVTable (Object t[][]) {	// creare tabel cu datele din matricea t
		a = t;
		for(int i=0;i<t.length;i++)
			for(int j=0;j<t[0].length;j++)
				a[i][j]=t[i][j];	//copii matricea t in tabelul a
	}
	public void show () {	//afisare tabel (in mod text)
		int i,j,lung=0;
		int max[] = new int[a[0].length];	//vector in care retin latimea fiecarei coloane
		for(j=0;j<a[0].length;j++){
			max[j]=0;
			for(i=0;i<a.length;i++)
				if(a[i][j].toString().length()>max[j])
					max[j]=a[i][j].toString().length();	//gasesc cel mai lung cuvant de pe fiecare coloana
			lung+=max[j];	//calculez lungimea unei linii
		}
		if((lung+a[0].length+1) <= 80){	//daca o linie are lungimea mai mica de 80 caractere
			char[] charArray = new char[lung+a[0].length+1];
			Arrays.fill(charArray,'-');	//creez un string de "-" de lungimea liniei
			String sep = new String(charArray);
			System.out.println(sep);	//afisez stringul de "-"
			for(i=0;i<a.length;i++){
				for(j=0;j<a[0].length;j++)	//afisez cuvintele centrate in tabel
					System.out.print("|"+centrare(a[i][j].toString(),max[j]));
				System.out.println("|");
				System.out.println(sep);	//afisez stringul de "-"
			}
		}
		else{	//daca o linie are lungimea mai mare de 80 caractere, inclusiv "|"
			int crop[] = new int[a[0].length];	//vector in care retin noua latime a fiecarei coloane
			int med=(80-a[0].length-1)/a[0].length;	//lungimea medie a unei coloane pt o linie de 80
			int rest=(80-a[0].length-1)%a[0].length;	//restul de loc dupa medierea fiecarei coloane
			int k=0;	//numarul de coloane cu lungimea peste medie
			for(j=0;j<a[0].length;j++)
				if(max[j]<=med){	//daca lungimea coloanei j este mai mica decat media
					rest+=med-max[j]; //adaug spatiul economisit de la coloane mai mici ca media la rest
					crop[j]=max[j];	//pastrez lungimea initiala a coloanei j
				}
				else{	//daca lungimea coloanei j nu este mai mica decat media
					crop[j]=med;	//noua lungime a coloanei este egala cu media
					k++;	//cresc numarul de coloane cu lungimea peste medie
				}
			int y=rest%k;	//distribui spatiul economisit la coloanele care erau mai lungi ca media
			for(j=0;j<a[0].length;j++)
				if(max[j]>med){	////daca lungimea coloanei j este mai mare decat media
					crop[j]+=rest/k+((y>0)?1:0);	//noua lungime a coloanei este marita unde trebuie
					y--;	//scade spatiul economisit
				}
			char[] charArray = new char[80];
			Arrays.fill(charArray,'-');	//creez un string de "-" de lungimea liniei
			String sep = new String(charArray);
			System.out.println(sep);	//afisez stringul de "-"
			for(i=0;i<a.length;i++){
				for(j=0;j<a[0].length;j++)
					if(a[i][j].toString().length() <= crop[j])	//daca textul este mai mic decat coloana
						System.out.print("|"+centrare(a[i][j].toString(),crop[j]));	//afisez textul centrat
					else
						System.out.print("|"+a[i][j].toString().substring(0,crop[j]));	//afisez inceputul
				System.out.println("|");			//textului atat cat imi da voie noua lungime a coloanei
				System.out.println(sep);	//afisez stringul de "-"
			}
		}
	}
	public String centrare (String s, int l) {	//centreaza sir pe lungime data
		char[] charArray = new char[(l-s.length())/2];
		Arrays.fill(charArray,' ');	//creez un string de " " de lungime (l-s.length())/2
		String sp = new String(charArray);
		s=s.concat(sp); //adaug stringul de " " la sfarsitul textului meu
		sp=sp.concat(s);	//adaug stringul de " " la inceputul textului meu
		if((l-sp.length()) == 0)
			return sp;
		else
			return sp.concat(" ");	//mai adaug un " " la sfarsit in caz ca este centrat inegal
	}
	public void writeCSV (String csvfile) throws IOException {	//scrie tabel in fisier CSV
		PrintStream ps = new PrintStream(new FileOutputStream(csvfile));
		int i,j;
		for(i=0;i<a.length;i++){
			for(j=0;j<a[0].length;j++){
				Object s = a[i][j];
				ps.print(s);
				if(a[0].length-j > 1)	//pun virgula pana inainte de ultimul cuvant de pe o linie
					ps.print(",");
			}
			ps.println();
		}
		ps.close();
	}
	public void readCSV (String csvfile) throws IOException {	//citire din fisier CSV in tabel
		File f = new File(csvfile);
		BufferedReader br = new BufferedReader(new FileReader(f));
		String line = null;
		ArrayList<String[]> mylist = new ArrayList<String[]>(); //lista de vectori string
		while((line = br.readLine()) != null) {	//cat timp fisierul nu e gol, citeste cate o linie
			String sir[] = line.split(",");	//imparte o linie citita intr-un vector de stringuri
			mylist.add(sir); //adauga vectorul de stringuri la lista de vectori string
		}
		br.close();
		a = mylist.toArray(new Object[mylist.size()][]); //convertesc lista de vectori string la 
	}													 //o matrice de tipul Object
	public void setValueAt (Object x, int r, int c) {	//pune x in linia r si coloana c
		a[r-1][c-1] = x;
	}
	public static void main (String args[]) throws IOException {
		if(args[0].equals("1")) {	//daca sunt in cazul 1 de verificare
			Object t[][] = {{"nume" ,"lab","lucr","exam"},
							{"unu"  ,"20" ," "   ,"30"},
							{"doi"  ," "  ,"10"  ,"20"},
							{"trei" ,"20" ," "   ," "},
							{"patru","20" ,"30"  ,"50"}};
			CSVTable x = new CSVTable(t);
			x.show();	//afisez tabelul pe ecran
			x.writeCSV("out.csv");	//salvez tabelul in fisierul out.csv
		}
		else 
			if(args[0].equals("2")) {	//daca sunt in cazul 2 de verificare
				CSVTable x = new CSVTable();
				x.show();	//afisez tabelul pe ecran
				x.setValueAt("13",3,4);	//modific elementul de pe linia 3, coloana 4 la stringul "13"
				x.writeCSV("out.csv");	//salvez tabelul in fisierul out.csv
			}
			else	//daca nu sunt in nici un caz de verificare
				System.out.println("Introduceti 1 pentru: \n\t'Creare tabel cu "+
					"date constante, afisare pe ecran si salvare in fisierul out.CSV.'"+
					"\nIntroduceti 2 pentru: \n\t'Citire tabel dintr-un fisier "+
					"in.CSV, afisare tabel pe ecran, modificare tabel in \n\t memorie "+
					"si salvare tabel in alt fisier out.CSV.'");
	}
}