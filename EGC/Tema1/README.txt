/*
 *				Elemente de grafica pe calculator (EGC)
 *
 * 					TEMA 1: GEOMETRY WARS
 *
 * nume:				Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 	333 CA
 */
 
 1. Cerinta:
	Implementarea unui joc de tipul Geometry Wars, folosind framework-ul de la laborator.

2. Utilizare:
	Nu necesita fisiere de intrare sau de iesire.
	
	In momentul rularii se vor cere in consola urmatorii parametrii:
- rezolutia ecranului pe care se vrea rularea in format:
			- width: 
			- height:
			
- daca se doreste rularea in FullScreen la care se poate raspunde cu y/n

	Mentionez ca jocul a fost rulat si testat pe rezolutia de 1920x1080. Rularea pe o rezolutie 
mai mica de atat va duce la efecte nedorite, gen text decalat. Acest lucru se datoreaza 
nescalabilitatii fontului.
	
	Tastele de CONTROL ale jocului sunt:
- UP: va muta jucatorul inainte;
- LEFT: va roti pe loc jucatorul la stanga;
- RIGHT: va roti pe loc jucatorul la dreapta;
- SPACE: va activa/dezactiva arma(burghiul) jucatorului;
- tasta B: va activa/dezactiva optiunea de a trage cu gloante pe directia burghiului;
- ESC: va termina executia programului;

3. Implementare:
	Tema a fost realizata in Visual Studio 2012 pe Windows 7 Ultimate.
	
	M-am folosit de Framework-ul de la primul exercitiu de la laboratorul 2 de EGC. Modificarile
pe care le-am adus au constat in modificarea urmatoarelor surse/headere:

- main.cpp;

	In sursa main.cpp am majoritatea implementarii.
	Aceasta a constituit din:
1 - definirea unor parametrii necesari in configurarea jocului (variabile globale);
2 - crearea corpului playerului si a burghiului, a gloantelor, a inamicilor si a bonusurilor;
3 - crearea unor functii care se apeleaza la un anumit interval de timp si care creaza 
unele din entitatile de la punctul 2;
4 - functia init in care imi creez restul de elemente prezente in fereastra (contexte vizuale,
text) si apelez functiile de la punctul 3;
5 - testarea faptului ca burghiul este in interiorul ferestrei;
6 - functii de miscare a gloantelor si a inamicilor;
7 - functii de testare a coliziunilor pe baza inscrierii fiecarui corp intr-un cerc si compararea
sumei razelor celor 2 cercuri cu distanta dintre centre;
8 - functie de actualizare a scorului si tratarea consecintelor acesteia;
9 - functia onIdle() unde testez apasarea tastelor UP, LEFT, RIGHT si apelez functiile de miscare
a inamicilor, gloantelor, bonusurilor;
10 - functia onKey() unde testez apasarea tastelor 'b' pentru burghiu si SPACE pentru tragerea
cu gloante;

	Am explicat mai in detaliu in sursa main.cpp si la punctul 7. Functionalitati.

- DrawingWindow.h;
- DrawingWindow.cpp;

	In clasa DrawingWindow am adaugat 2 vectori bool keyStates si keySpecialStates de dimensiune
256 care retin starea fiecarei taste apasate:
- false: tasta eliberata;
- true: tasta apasata;
	Sunt 2 vectori deoarece sunt 2 feluri de taste: taste normal si taste speciale.
	Acesti vectori sunt initializati in metodele keyboardFunction cu true si keyboardFunctionUp cu
false Analog pentru specialFunction si specialFunctionUp. De asemenea in aceste metode se apeleaza
functia onKey.
	Mi-au trebuit acesti vectori pentru a permite recunoasterea interna a distinctiei dintre o tasta
apasat si una eliberata. Astfel, in timp ce tin apasata tasta UP, iar jucatorul se deplaseaza inainte,
pot sa apas tasta LEFT sau RIGHT pentru a ma roti, continuand sa ma deplasez inainte. Daca nu as
fi implementat acest lucru, ar fi trebuit sa ridic tasta UP dupa care sa ma rotesc pe loc, dupa care
sa apas din nou tasta UP, lucru neplacut si care impiedica fluiditatea jocului.

- Ansamblu.h;
- Ansamblu.cpp;

	Pe langa frameworkul de la laborator, am mai avut nevoie de de crearea unui header si a
unei surse unde am implementat clasa Ansamblu cu metodele aferente. Aceasta clasa retine
un vector de Object2D care compun corpul navei playerului, al inamicilor, al gloantelor si al
bonusurilor. Mi-a fost mai usor sa lucrez astfel pe ansamblurile respective decat pe fiecare
obiect care compunea corpul unei entitati de pe teren.
	Metodele din aceasta clasa sunt:
- functia de adaugare a unui obiect la contextul vizual prin intermediul clasei Ansamblu;
- functia de stergere a unui obiect din contextul vizual prin intermediul clasei Ansamblu;
- functia de testare a apartenentei Ansamblului la interiorul ferestrei;
- functia de procesare a deplasarii unui Ansamblu in fereastra;
- functia de stergere a unui Ansamblu din contextul vizual
	
4. Testare:
	Tema a fost rulata si testata in Visual Studio 2012 pe Windows 7 Ultimate pe o rezolutie
de 1920x1080, in modul FullScreen.
	De asemenea, daca tema este testata pe un laptop cu 2 procesoare grafice, se recomanda
folosirea GPU-ului de la placa video in deschiderea si rularea Visual Studio pentru o performanta
marita.

	La inceputul sursei main.cpp se gasesc toate variabilele globale care se pot schimba de
catre utilizator pentru a configura parametrii jocului (ex: viteza jucatorului, timpul dintre
wave-urile de inamici etc). Sunt destul de multi parametrii si de aceea am ales sa fac aceste
variabile globale.

5. Probleme aparute:
	Am avut probleme la testarea apasarii simultane a mai mult de 3 taste, despre care am
citit apoi ca nu prea este posibila detectia acesteia in OpenGL.
	Am vrut sa aplic o textura de fundal si sunete dar am renuntat si din lipsa de timp dar
si pentru ca nu cunosteam indeajuns de bine.
	Am implementat toate bonusurile mai putin cel cu inamici animati cu structuri mai 
complexe pe care n-am mai avut timp sa-l fac. As fi vrut sa il fac ca un nivel special, cu un
boss. :D
	In rest, am implement multe alte "bonusuri" personale in felul in care se comporta inamicii,
afisaj, organizarea a temei pe care o sa le explic la 7. Functionalitati.
	
6. Continutul arhivei:
	Frameworkul de la laborator de unde am sters fisierul lab2 de tip SQL Server Compact Edition
Database care ocupa peste 30MB pentru a putea uploada arhiva.
	Fisierul README.txt

7. Functionalitati:
	
	Functionalitati Standard (ca in enunt):
- crearea a 1 player cu burghiul atasat si gloante;
- crearea a 4 tipuri de inamici, particularizati prin forma, culoare, mod de deplasare, punctaj etc.
- crearea a 2 contexte vizuale: titlu cu numele temei si al realizatorului, scorul, nivelul si nr de vieti
si teren cu spatiul de joc in care se misca playerul si inamicii;
- miscare player conform cerintei;
- miscare random a unor inamici;
- coliziunile dintre player, burghiu si inamici care se realizeaza pe baza de coliziuni cerc-cerc;
- neiesirea din plansa a entitatilor create;
	
	Functionalitati Bonus (ca in enunt):
- miscare inteligenta a unor inamici (se indreapta catre nava jucatorului);
- mai multe nivele de dificultate care se diferentiaza prin:
			- cresterea vitezei inamicilor;
			- scaderea intervalului de timp dintre 2 wave-uri consecutive a cate 4 inamici fiecare;
	Trecerea la un nou nivel de dificultate se face odata cu atingerea unui anumit scor.
- deplasarea cu acceleratie/franare a playerului atat cu burghiul activat cat si cu el dezactivat;
- abilitatea playerului de a trage cu gloante, pastrandu-si in acelasi timp arma(burghiul), aceasta
abilitate insa putand fi activa doar daca si burghiul este activ. De asemenea se realizeaza testarea
coliziunilor gloantelor cu inamicii;
- aparitia random periodica a unuia din 7 tipuri de bonusuri la o locatie random de pe teren;
			-cele 7 tipuri de bonusuri sunt:
								+1 viata;
								+ 100 pct;
								+ 200 pct;
								+ viteza de deplasare a playerului, impreuna cu inlaturarea penalizarii de deplasare
la activarea burghiului si cresterea acceleratiei, totul pentru 4 sec de la luarea bonusului;
								+ rata de tragere cu gloante, cresterea razei si vitezei acestora pentru 4 sec;
								time_stop: oprirea animatiei inamicilor pentru 4 sec;
								invulnerabilitate pentru 4 sec;
	
	Functionalitati Suplimentare:
- numarul mare de bonusuri si diversificarea acestora impreuna cu comportamentul lor;
- gruparea inamicilor de acelasi tip in wave-uri de cate 4 inamici, cu aparitia fiecarui wave la un 
anumit interval de timp;
- miscarea dupa un anumit tipar (bounce-off walls) a unor inamici;
- oferirea unui punctaj redus playerului la omorarea unui inamic cu un glont fata de oferirea unui
punctaj normal la folosirea burghiului. Astfel se incurajeaza lupta close-range;
- se pot apasa mai multe taste in acelasi timp;
- afisarea si disparitia dupa 3 sec a diferitelor mesaje:
								- la trecerea la un nou level;
								- ciocnirea cu un inamic si pierderea unei vieti,
								- luarea unui bonus (mesaj specific fiecarui bonus);
								- terminarea jocului impreuna cu scorul final;
								- baterea jocului (lucru ce nu se va intampla prea curand :p );
- mecanism de freeze a ecranului la terminarea jocului;
- mutarea textului de viata in caz ca se depasesc numere de 1 cifra;
- afisarea scorului in format special (ex: 00000, +100 pct -> 00100);
- distrugerea gloantelor odata ce parasesc terenul;