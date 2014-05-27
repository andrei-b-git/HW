/*
 *	Tema 2 - Algoritmi paraleli si distribuiti
 *
 *	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 333 CA
*/


1. Continut arhiva:

	-MapPartialSolution.java =  Clasa ce reprezinta o solutie partiala pentru rezolvarea functionalitatii 
				MAP. Aceste solutii partiale constituie task-uri care sunt introduse in workpoolul 
				MapWorkPool;
	-MapWorkPool.java = Clasa ce implementeaza un "work pool" conform modelului "replicated 
				workers", necesar rezolvarii functionalitatii MAP. Task-urile introduse in work pool 
				sunt obiecte de tipul MapPartialSolution;
	-MapWorker.java = Clasa ce reprezinta un thread MapWorker;
	-ReducePartialSolution.java = Clasa ce reprezinta o solutie partiala pentru rezolvarea functionalitatii
				REDUCE. Aceste solutii partiale constituie task-uri care sunt introduse in workpoolul 
				ReduceWorkPool;
	-ReduceWorkPool.java = Clasa ce implementeaza un "work pool" conform modelului "replicated 
				workers", necesar rezolvarii functionalitatii REDUCE. Task-urile introduse in work pool 
				sunt obiecte de tipul ReducePartialSolution;
	-ReduceWorker.java = Clasa ce reprezinta un thread ReduceWorker;
	-ValueComparator.java = Clasa comparator necesara pt ordonarea gradelor de similaritate;
	-Tema2.java = Clasa care contine main-ul si in care realizez implementarea propriu-zisa;
	-README 	


2. Utilizare:

	Proiectul constituit cu sursele din arhiva se va rula cu parametrii in linie de comanda:
	-NT = numarul de threaduri;
	-input = fisier de intrare;
	-output = fisier de iesire;

	
3. Detalii implementare:

	In realizarea temei m-am folosit de bucati de cod din laboratorul 5. Replicated Workers, de unde
am luat modelul claselor tip WorkPool, Worker si PartialSolution.

	In sursa Tema2.java, in functia main din clasa Tema2, am realizat implementarea functionalitatii
temei. Aceasta incepe prin citirea parametrilor din linia de comanda, urmata de citirea si parsarea
fisierului de intrare.

	Pentru functionalitatea MAP, folosesc un HashMap< String, HashMap<String, Integer> > care retine
perechi de tipul (nume_doc, vector_partial), vectorul partial fiind la randul sau un alt HashMap cu
perechi de tipul (cuvant, nr de aparitii in doc). Pentru fiecare document, creez un MapWorkPool cu 
nThreads NT si citesc fragmentelor din fisier cu readFully(byte[], off, len) afiliata lui RandomAccessFile.
In citirea fragmentelor, citesc cate D octeti. Daca pointerul curent al fisierului este la mijlocul unui
cuvant, mai citesc atati octeti pana dau de un delimitator. In caz ca am ajuns la ultimul fragment,
citesc atatia octeti cati au mai ramas. Pun cate un fragment in MapPartialSolution si pe urma pun MPS
in MapWorkPool. Dupa citirea si punerea in MapWorkPool a fragmentelor, creez NT MapWorkeri pt 
ca am NT Threaduri si pentru fiecare MapWorker ii dau un MapWorkPool, dupa care ii dau start().
Apoi astept cu join ca toti Workerii sa ajunga la final. Urmeaza combinarea rezultatelor celor NT 
threaduri in acel HashMap< String, HashMap<String, Integer> >  declarat la inceput.

	Pentru functionalitatea REDUCE, folosesc o structura care retine gradele de similaritate pt fiecare 
doc de tipul HashMap<String, Double>. In cadrul lui REDUCE, calculez gradul de similaritate intre DOC 
si restul documentelor utilizand vectorii partiali de la MAP. Incep prin a calcula numarului de cuvinte 
din DOC si apoi, pentru fiecare document diferit de DOC, calculez, de asemenea, nr de cuvinte din el,
creez un ReduceWorkPool cu nThreads NT, calculez frecventele de aparitie a cuvintelor comune doc
si pun frecventele in ReducePartialSolutions, dupa care le pun in ReduceWorkPool. Creez NT 
ReduceWorkeri pt ca am NT Threaduri si pentru fiecare ii dau un ReduceWorkPool si ii dau start(),
dupa care astept ca toti sa termine cu join(). La urma combin gradele de similarite calculate de fiecare 
thread si le retin in structura de tipul HashMap<String, Double> declarata la inceput, dupa ce le
trunchiez la 3 zecimale.

	In final, ordonez gradele de similaritate si le retin intr-un TreeMap, dupa care realizea afisarea
celor cu grad de similaritate mai mare ca X, impreuna cu documentele afiliate, in fisier.	

	De asemenea, tot in clasa Tema2 am o functie care realizeaza trunchierea unui double la un anumit
nr de zecimale.
