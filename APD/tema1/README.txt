/*
 *	Tema 1 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/


1. Continut arhiva:

	-serial.cpp		 : sursa cu implementarea serial neoptimizata
	-serial_opt.cpp	 : sursa cu implementarea serial optimizata
	-paralel.cpp	 : sursa cu implementarea paralel neoptimizata
	-paralel_opt.cpp : sursa cu implementarea paralel optimizata
	-makefile
	-README 	


2. Utilizare:

	Fisierul makefile are reguli de compilare si clean. La rulare va crea 
4 executabile cu denumiri specifice fiecarei tip de implementare: serial, 
serial_opt, paralel si paralel_opt.
	Fiecare executabil se va rula cu comanda specifica:
	./nume_exe nr_sapt fisin fisout


3. Detalii implementare:

	In toate sursele realizez la inceput deschiderea fisierelor spre citire,
citirea efectiva din acestea impreuna cu alocarea memoriei pentru cele doua
matrici corespunzatoare celor doua configuratii, cea a saptamanii curente si
cea a celei urmatoare. La sfarsit eliberez memoria alocata si inchid fisierele.

	Toti timpii de rulare au fost obtinuti pe fisierul de input cu o matrice de
dimensiune 100, 20 de tipuri de culori si pe parcursul a 100 de saptamani.


	Varianta serial neoptimizata:

	Folosesc 3 vectori de dimensiunea numarului de culori, toti initial 0:
- cul_freq: care retine pentru fiecare culoare numarul de senatori din configuratia 
			curenta;
- cul_freq_1: care retine acele culori din configuratia curenta care au nr de 
			  senatori egal cu 1;
- disp: care retine acele culori care nu mai au senatori;
	Incep prin a parcurge numarul de saptamani, dupa care parcurg matricea initiala
de configurare config1 si apoi parcurg fiecare culoare. Pentru fiecare culoare
verific daca nu mai are senatori sau daca mai are un senator, caz in care distanta
minima va fi 0. Apoi calculez distanta minima de la elem pe care vreau sa-l inlocuiesc
la o culoare utilizand doua foruri si verificand daca am un element de culoarea
respectiva dar diferit ca pozitie fata de elementul pentru care caut. Calculez distanta
facand maximul dintre modulele diferentelor celor 2 coordonate, dupa care dintre toate 
aceste distante o obtin pe cea mai mica. In final am o multime de distante minime la
toate culorile dintre care o aleg pe cea mai mare, aceasta fiind cea care va inlocui
vechea culoare in noua matrice de configuratie, corespunzand saptamanii urmatoare.
De asemenea, odata cu obtinerea noi culori, incrementez in vectorul cul_freq numarul
de senatori ai culorii respective.
	In final parcurg si afisez nr de senatori ai fiecarei culori pentru saptamana
curenta, marcand in vectorul disp daca am o culoare care nu mai are senatori si in
vectorul cul_freq_1 acele culori care mai au 1 senator. Interschimb apoi matricile
corespunzatoare configuratiilor iar la sfarsit de tot afisez matricea configuratiei
corespunzatoare ultimei saptamani.
	
	Timp de rulare : 2m41.499s


	Varianta serial optimizata:

	Folosesc 4 vectori de dimensiunea numarului de culori, toti initial 0:
- culori: unde retin numarul de aparitii al fiecarei culori pornind din centru si
		  parcurgand cercurile concentrice;
- r: care retine pentru fiecare culoare intalnita raza minima pana la aceasta, fata
	 de centru;
- cul_freq: care retine pentru fiecare culoare numarul de senatori din configuratia 
			curenta;
- cul_freq_1: care retine acele culori din configuratia curenta care au nr de 
			  senatori egal cu 1;
	Cat timp nu am ajuns la ultima saptamana, parcurg matricea initiala de configurare
config1, fac reinitializarile care sunt necesare: raza cercului curent in care caut,
numarul de elemente nenule din vectorul culori, culoarea cautata care ia initial o
valoare foarte mare, vectorii de culori si raze si suma tuturor culorilor unice pe
care le intalnesc parcurgand cercurile. Urmeaza sa fac cautarea care consta intr-un
while (cat timp nu am gasit un Cmax). In interiorul acestui while incep prin a
stabili limitele intre care caut, dupa care incerc sa parcurg fiecare latura a cadrului
(cercului). Daca reusesc, verific daca am intalnit o culoare noua, caz in care cresc
numarul de elemente nenule din vectorul culori si tin minte raza la care am gasit culoarea
in vectorul r, dupa care adun culoarea la suma care retine toate culorile nou intalnite
parcurgand cercurile. Indiferent daca am intalnit o culoare noua sau nu, incrementez
in vectorul culori, elementul corespunzator culorii respective. Daca numarul de elemente 
nenule a ajuns cat numarul de culori existente, inseamna ca tot ceea ce mai trebuie sa fac 
pentru a gasi culoarea cautata este sa parcurg vectorul de raze si sa gasesc prima culoare 
cu raza cea mai mare (raza curenta la care s-a ajuns).
	In final dupa ce am parcurs fiecare latura a cadrului(cercului) de raza curenta, daca
tot nu am gasit culoarea verific mai intai daca culoarea curenta pe care vreau s-o
inlocuiesc are mai mult de 1 senator sau nu dupa care verific daca numarul de elemente
nenule a ajuns cat numarul de culori existente -1 (-2 in cazul unei culori cu 1 senator)
si daca da, atunci culoarea pe care o caut este cea care a mai ramas de negasit si pe care
o obtin facand diferenta dintre suma tuturor culorilor existente si suma culorilor pe care
le-am gasit pana atunci parcurgand cercurile.
	Daca in final tot nu am gasit culoarea atunci incrementez raza si reiau cautarea.
	Daca am gasit totusi culoarea, o inlocuiesc in noua configuratie si incrementez numarul
de senatori ai culorii respective in vectorul cul_freq.
	La finalul unei saptamani parcurg si afisez numarul de senatori ai fiecarei culori,
marcand in vectorul cul_freq_1 daca am o culoare cu 1 senator si crescand numarul de
culori fara senatori si eliminand culoarea respectiva din suma tuturor culorilor existente,
in cazul in care o culoare nu mai are senatori. Realizez apoi interschimbarea matricilor de
configuratie si incrementarea numarului de saptamani.
	In final afisez matricea configuratiei corespunzatoare ultimei saptamani.

	Timp de rulare : 11.548s


	Varianta paralel neoptimizata:

	Folosesc implementarea de la varianta serial neoptimizata, unde tot ce fac este sa
inserez un #pragma parallel for dupa forul care parcurge numarul de saptamani si
inainte de forul care parcurge liniile din matrice. Astfel fiecare thread se va
ocupa cu calculul unei linii din matrice. Am avut grija sa includ la privat toti
parametrii interni forului care sufereau modificari. De asemenea a fost nevoie de un
#pragma omp critical inainte de incrementarea nr de senatori ai culorii care a inlocuit
vechea culoare pentru a ma asigura ca nu scriu mai multe threaduri in acelasi timp
in acelasi spatiu de memorie.

	Timp de rulare : 1m16.236s	(4 threaduri, schedule-ing static, chuncksize 10)
	Speed-up fata de varianta serial neoptimizata : 2.11


	Varianta paralel optimizata:

	La fel ca la varianta paralel neoptimizata, ma folosesc de implementarea de la 
varianta serial optimizata, unde inserez un #pragma parallel for dupa forul care 
parcurge numarul de saptamani si inainte de forul care parcurge liniile din matrice.
Astfel fiecare thread se va ocupa cu calculul unei linii din matrice. La fel, am grija
sa includ la privat toti parametrii interni forului care sufereau modificari si sa
adaug un #pragma omp critical inainte de incrementarea nr de senatori ai culorii care a 
inlocuit vechea culoare pentru a ma asigura ca nu scriu mai multe threaduri in acelasi
timp in acelasi spatiu de memorie.

	Timp de rulare si speed-up : vezi tabelul de la punctul 5


4. Specificatiile sistemului pe care a fost rulata tema:

	Sistem de operare: 	Linux Mint 15: Olivia (x86-64 bit)
	Kernel/Build:		3.8.0-19-generic / #29-Ubuntu SMP Wed Apr 17 18:16:28
						UTC 2013
	Processor:			Intel Core i7-2630QM @ 2.00 Ghz x 4 
						4 Core-uri
						8 Thread-uri
						Max Turbo Frequency - 2.9 Ghz
	Cache Memory:		6 MB
	Compilator:			g++ (Ubuntu/Linaro 4.7.3-1ubuntu1) 4.7.3

	Mentionez ca tema a fost rulata pe calculatorul unui coleg deoarece sistemul
propriu nu dispune de o distributie Linux instalata direct, ci doar de una
instalata pe masina virtuala care nu permite rularea pe mai multe threaduri.

5. Tabel cu speed-up-ul si timpii scosi in urma testelor:

	Testele au fost efectuate pe varianta paralel optimizata. 
	Speed-up-ul a fost obtinut impartind timpul variantei serial optimizate
(Timp = 11.548s) la timpul variantei paralel optimizate.

	S	N	Nc	Nr.Thread-uri	Timp(s)	Speed-up	Tip		Chunksize	

	100	100	20		2			6.713	1.72		Static		10	
	100	100	20		4			4.407	2.62		Static		10	
	100	100	20		6			4.341	2.66		Static		10	
	100	100	20		8			4.292	2.69		Static		10	
								
	100	100	20		2			6.077	1.90		Dinamic		10	
	100	100	20		4			4.023	2.87		Dinamic		10	
	100	100	20		6			3.941	2.93		Dinamic		10	
	100	100	20		8			3.914	2.95		Dinamic		10	
								
	100	100	20		2			6.561	1.76		Static		25	
	100	100	20		4			4.230	2.73		Static		25	
	100	100	20		6			4.124	2.80		Static		25	
	100	100	20		8			4.109	2.81		Static		25	
								
	100	100	20		2			5.952	1.94		Dinamic		25	
	100	100	20		4			3.875	2.98		Dinamic		25	
	100	100	20		6			3.849	3.00		Dinamic		25	
	100	100	20		8			3.798	3.04		Dinamic		25

