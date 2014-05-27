/*
 *				Elemente de grafica pe calculator (EGC)
 *
 * 					TEMA 2: CRAZY DRIVER
 *
 * nume:				Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 	333 CA
 */
 
 1. Cerinta:
	Implementarea unui joc 3D wireframe, de curse auto cu obstacole, intr-o varianta simplificata.
	
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
- UP: va misca jucatorul inainte;
- LEFT: va roti jucatorul la stanga;
- RIGHT: va roti jucatorul la dreapta;
- DOWN: va opri jucatorul;
- ESC: va termina executia programului;

3. Implementare:
	Tema a fost realizata in Visual Studio 2012 pe Windows 7 Ultimate.
	
	M-am folosit de Framework-ul  de la laboratorul 3 de EGC, si de asemenea de destul de mult
cod de la prima tema. Modificarile pe care le-am  adus au constat in modificarea urmatoarelor 
surse/headere:

- main.cpp;

	In sursa main.cpp am majoritatea implementarii.
	Aceasta a constituit din:
1 - definirea unor parametrii necesari in configurarea jocului (variabile globale);
2 - crearea drumului compus din strada si mediul inconjurator, a benzilor intermitente, 
a corpului masinii playerului si a rotilor, a inamicilor si a bonusurilor;
3 - functia init in care imi creez restul de elemente prezente in fereastra (contexte vizuale,
text);
4 - crearea unor functii care aleg random ce functii de creare a inamicilor si bonusurilor se 
apeleaza;
5 - functie de actualizare a scorului si tratarea consecintelor acesteia;
6 - functii de miscare a inamicilor si a bonusurilor;
7 - functii de testare a coliziunilor pe baza inscrierii fiecarui corp intr-un cerc sau sfera si 
compararea sumei razelor celor 2 cercuri (sfere) cu distanta dintre centre;
8 - functia onIdle() unde testez apasarea tastelor UP, LEFT, RIGHT si DOWN, realizand deplasarea
benzilor intermitente si apelez functiile de miscare a inamicilor si bonusurilor;

	Am explicat mai in detaliu in sursa main.cpp si la punctul 7. Functionalitati.

- DrawingWindow.h;
- DrawingWindow.cpp;

	In clasa DrawingWindow am adaugat 2 vectori bool keyStates si keySpecialStates de dimensiune
256 care retin starea fiecarei taste apasate:
- false: tasta eliberata;
- true: tasta apasata;
	Sunt 2 vectori deoarece sunt 2 feluri de taste: taste normale si taste speciale.
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
un vector de Object3D care compun corpul masinii playerului, al inamicilor si al bonusurilor. 
Mi-a fost mai usor sa lucrez astfel pe ansamblurile respective decat pe fiecare obiect care 
compunea corpul unei entitati de pe teren.
	Metodele din aceasta clasa sunt:
- functia de adaugare a unui obiect la contextul vizual prin intermediul clasei Ansamblu;
- functia de stergere a unui obiect din contextul vizual prin intermediul clasei Ansamblu;
- functia de procesare a deplasarii unui Ansamblu in fereastra;
- functia de stergere a unui Ansamblu din contextul vizual
	
4. Testare:
	Tema a fost rulata si testata in Visual Studio 2012 pe Windows 7 Ultimate pe o rezolutie
de 1920x1080, in modul FullScreen.
	De asemenea, daca tema este testata pe un laptop cu 2 procesoare grafice, se recomanda
folosirea GPU-ului de la placa video in deschiderea si rularea Visual Studio pentru o performanta
marita.

	La inceputul sursei main.cpp se gasesc toate variabilele globale care se pot schimba de
catre utilizator pentru a configura parametrii jocului (ex: viteza jucatorului, bonusuri etc). 
Sunt destul de multi parametrii si de aceea am ales sa fac aceste variabile globale.

5. Probleme aparute:
	Am avut probleme la intelegerea felului in care se face proiectia si la stabilirea dimensiunilor
ferestrei in care implementez obiectele.
	Am implementat 3 bonusuri pe langa cerintele temei. As fi vrut sa fac si bonusul cu umplerea
poligoanelor cu culoare, dar obiectele nu se distingeau asa ca am renuntat. Am mai incercat sa
aplic efecte de lighting ca sa se poata deosebi fetele obiectelor 3D, dar nu am reusit.
	In rest, am implement multe alte "bonusuri" personale in felul in care se comporta inamicii,
afisaj, organizarea a temei pe care o sa le explic la 7. Functionalitati.
	
6. Continutul arhivei:
	Frameworkul de la laborator de unde am sters fisierul de tip SQL Server Compact Edition
Database care ocupa peste 30MB pentru a putea uploada arhiva.
	Fisierul README.txt

7. Functionalitati:
	
	Functionalitati Standard (ca in enunt):
- crearea unei sosele drepte cu 4 benzi, demarcate prin benzi mai mici, intermitente;
- crearea masinii playerului si implementarea controalelor specificate in enunt;
- efectul vizual de rotire a botului masinii la schimbarea unei benzi;
- crearea a 3 tipuri de inamici (obstacole), particularizati prin forma, culoare, mod de deplasare, 
punctaj etc. Acestia sunt de 2 tipuri: fixi si mobili si au formele: piramida, cub si octogon 3D;
- generarea aleatoare a obstacolelor, atat a tipului cat si a pozitiei;
- crearea a 2 contexte vizuale: titlu cu numele temei si al realizatorului, scorul, distanta parcursa,
timpul trecut si nr de vieti, si teren cu spatiul de joc in care se misca playerul si inamicii;
- coliziunile dintre player si inamici se realizeaza pe baza de coliziuni cerc-cerc, sau sfera-sfera 
(bounding spheres);
- respecarea mecanicilor jocului: parcurgerea unei anumite distante, acumuland puncte prin
evitarea obstacolelor, cu un numar maxim de coliziuni (vieti), dupa care jocul este pierdut.
	
	Functionalitati Bonus (ca in enunt):
- modele geometrice mai complexe pentru jucator: acesta este compus din 10 obiecte separate
printre care si cele 4 roti care au miscare individuala;
- diferite animatii si efecte aplicate masinii jucatorului si obstacolelor. Aici voi enumera animatia
rotilor care se invart atat in jurul propriului centru atunci cand masina porneste, cat si rotirea
rotilor din fata ale masinii la luarea unei curbe, simuland realitatea. De asemenea, obstacolele
se invart in jurul propriului centru iar un anumit tip de obstacol simuleaza mecanica de miscare
a unei "bouncing ball", cu urcare la o anumita altitudine cu deceleratie si apoi coborarea cu
acceleratie, in acest timp mergand cu o viteza proprie constanta;
- aparitia random periodica a unuia din 3 tipuri de obstacole-bonus pe o banda random;
			-cele 3 tipuri de bonusuri sunt:
								+1 viata;
								+ 1000 pct;
								invulnerabilitate pentru 4 sec (timp in care nu se mai testeaza coliziunile);
	
	Functionalitati Suplimentare:
- implementarea unui sistem de monitorizare a distantei "smecher" cu un segment de dreapta
reprezentand distanta de parcurs si un minicar reprezentand masina care se deplaseaza pe
segmentul reprezentat de distanta pe masura ce masina se deplaseaza in jocul real;
- se pot apasa mai multe taste in acelasi timp;
- afisarea si disparitia dupa 0.5 sec a diferitelor mesaje:
								- ciocnirea cu un inamic si pierderea unei vieti,
								- luarea unui bonus (mesaj specific fiecarui bonus);
								- terminarea jocului impreuna cu scorul final;
								- baterea jocului;
- mecanism de freeze a ecranului la terminarea jocului;
- mutarea textului de viata in caz ca se depasesc numere de 1 cifra;
- afisarea scorului in format special (ex: 00000, +100 pct -> 00100);