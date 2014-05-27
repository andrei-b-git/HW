/*
 *				Elemente de grafica pe calculator (EGC)
 *
 * 					TEMA 3: ATLETISM
 *
 * nume:				Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 	333 CA
 */
 
 1. Cerinta:
	Implementarea unei curse de atletism care sa poate fi urmarita din mai multe perspective.
	
2. Utilizare:
	Nu necesita fisiere de intrare sau de iesire.
		
	Mentionez ca jocul a fost rulat si testat pe rezolutia de 1920x1080. Rularea pe o rezolutie 
mai mica de atat va duce la efecte nedorite. Rularea se va face cu setarile de 1920x1080 implicite.
	
	Tastele de CONTROL ale jocului sunt:
- ESC ... iesire;
- SPACE ... reincarca shadere;
- ENTER ... toggle wireframe;
- o ... reseteaza camera;
- w ... translatie camera in fata;
- s ... translatie camera inapoi;
- a ... translatie camera in stanga;
- d ... translatie camera in dreapta;
- r ... translatie camera in sus;
- f ... translatie camera jos;
- q ... rotatie camera FPS pe Oy, plus;
- e ... rotatie camera FPS pe Oy, minus;
- z ... rotatie camera FPS pe Oz, plus;
- c ... rotatie camera FPS pe Oz, minus;
- t ... rotatie camera FPS pe Ox, plus;
- g ... rotatie camera FPS pe Ox, minus;
- UP, DOWN, LEFT, RIGHT ... controlul jucatorului principal;
- F1 ... activare FullScreen;
- F2 ... dezactivarea FullScreen;
- F3 ... activare shader Gouraud;
- F4 ... activare shader Phong;
- 1 ... schimbarea camera anterioara;
- 2 ... schimbarea camera urmatoare;
- 3 ... impartirea viewportului in 4;

3. Implementare:
	Tema a fost realizata in Visual Studio 2012 pe Windows 7 Ultimate.
	
	Am profitat de libertate de implementare si am ales sa realizez o simulare a unei curse de
masini. Circuitul respecta forma stadionului din enunt si este realizat manual. Masinile si camera
de filmat sunt modele .obj care sunt loadate. Toate insa sunt retinute si manipulate prin vao-uri.
	Cea mai complexa a fost realizarea miscarii masinilor si a camerelor, implementata in functia
NotifyBeginFrame(), unde a trebuit sa impart circuitul in 6 parti: banda dreapta din dreapta,
curba de sus din dreapta, curba de sus din stanga, banda dreapta din stanga, curba de jos din
stanga si curba de jos din dreapta. Pe toate aceste 6 parti a trebuit sa implementez miscarea
separata a fiecarui obiect, plus miscarea camerelor care trebuiau sa urmeze obiectele perfect.
	Ca shader am folosit 6 shadere: vertex si fragment pentru iluminare Gouraud si vertex si fragment
pentru iluminare Phong. Acestea le folosesc la incarcarea tuturor obiectelor, mai putin steagul care
flutura si apare la sfarsit. Acesta este incarcat folosind un shader special pentru el, unde se pastreaza
pentru fiecare punct coord x si y a acestuia si se variaza doar coordonata z printr-o functie de sinus
care depinde de o variabila uniforma trimisa si actualizata in main la fiecare refresh al functiei
NotifyDisplayFrame(). Steagul in sine este o matrice de patrate in planul xoy.
	Fisierul de modificare a vitezelor masinilor se gaseste in folderul Tema 3\resurse.
	
	M-am folosit de Framework-ul  de la laboratorul lab_gl_3_student de EGC. Modificarile 
pe care le-am  adus au constat in modificarea urmatoarelor surse/headere:

- main.cpp;

	In sursa main.cpp am majoritatea implementarii.
	Aceasta a constituit din:
1 - definirea unor parametrii necesari in configurarea jocului si initializarea acestora in constructor;
2 - crearea circuitului compus din mai multe semicercuri si dreptunghiuri suprapuse;
3 - crearea masinilor si a camerelor asociate;
4 - crearea camerei de filmat;
5 - crearea steagului care flutura si apare la sfarsit;
6 - functie care verifica apartenenta masinii jucatorului principal la interiorul pistei;
7 - functia NotifyBeginFrame() unde am realizat toata miscarea obiectelor din cursa;
8 - functia NotifyDisplayFrame() unde testez sfarsirea jocului si trimit la shader matricile de modelare
si toate celelalte variabile si apoi fac bind la vao-urile asociate fiecarui obiect;
9 - functia NotifyEndFrame() unde regasesc locatiile variabilelor din shaderul folosit;
10 - functiile in care fac legatura intre butoane si functia lor asociata;
11 - mainul in care am setat dimensiunile ecranului la 1920x1080.

	Am explicat mai in detaliu in sursa main.cpp si la punctul 7. Functionalitati.

- lab_glut_support.hpp;
- lab_glut.hpp;

	In lab_glut.hpp si lab_glut_support.hpp am adaugat 2 vectori bool keyStates si keySpecialStates 
de dimensiune 256 care retin starea fiecarei taste apasate:
- false: tasta eliberata;
- true: tasta apasata;
	Sunt 2 vectori deoarece sunt 2 feluri de taste: taste normale si taste speciale.
	Acesti vectori sunt initializati in metodele _keyboardDownCallback cu true si _keyboardUpCallback 
cu false Analog pentru _specialDownCallback si _specialUpCallback.
	Mi-au trebuit acesti vectori pentru a permite recunoasterea interna a distinctiei dintre o tasta
apasat si una eliberata. Astfel, in timp ce tin apasata tasta UP, iar jucatorul se deplaseaza inainte,
pot sa apas tasta LEFT sau RIGHT pentru a ma roti, continuand sa ma deplasez inainte. Daca nu as
fi implementat acest lucru, ar fi trebuit sa ridic tasta UP dupa care sa ma rotesc pe loc, dupa care
sa apas din nou tasta UP, lucru neplacut si care impiedica fluiditatea jocului.
	
4. Testare:
	Tema a fost rulata si testata in Visual Studio 2012 pe Windows 7 Ultimate pe o rezolutie
de 1920x1080, in modul FullScreen.
	De asemenea, daca tema este testata pe un laptop cu 2 procesoare grafice, se recomanda
folosirea GPU-ului de la placa video in deschiderea si rularea Visual Studio pentru o performanta
marita.

5. Probleme aparute:
	Am avut probleme la atribuirea unei culori obiectelor de tip obj pe care le loadam, deoarece
intrau in calcul si normalele.
	Am avut probleme in realizarea miscarii obiectelor de pe circuit, in acelasi timp cu miscarea
camerelor din perspectiva carora se vedeau obiectele. De asemenea am avut probleme in realizarea
miscarii obiectelor in sine, mai ales pe portiunile de semicerc ale circuitului si in trecerea de la o
portiune de semicerc la una dreapta si invers.

6. Continutul arhivei:
	Frameworkul de la laboratorul lab_gl_3_student de unde am sters fisierul de tip SQL Server 
Compact Edition Database care ocupa peste 30MB pentru a putea uploada arhiva.
	Fisierul README.txt

7. Functionalitati:
	
	Functionalitati Standard (ca in enunt):
- crearea unui circuit cu 4 culoare ca in enunt;
- crearea masinii playerului si implementarea controalelor specificate in enunt;
- crearea masinilor celorlalti si implementarea miscarii automate a acestora;
- crearea unei camere de filmat care se misca odata cu jucatorul principal;
- crearea celor 4 tipuri de camere ca in enunt si schimbarea intre ele din butoane;
- se pot apasa mai multe taste in acelasi timp ca in enunt;
- citirea vitezelor masinilor din fisier ca in enunt;
- terminarea unui tur pentru ca jocul sa se termine;
- implementarea steagului care flutura si care apare odata ce s-a terminat cursa;
- toate obiectele fie ca sunt create, fie ca sunt loadate, sunt retinute in vao;
- neiesirea de pe pista a jucatorului principal;
	
	Functionalitati Bonus (ca in enunt):
- scena realista, construita cu atentie la detalii, formata din o multitudine de obiecte;
	
	Functionalitati Suplimentare:
- adaugarea efectului de iluminare a obiectelor din joc: exista doua tipuri de iluminare, Gouraud sau
Phong intre care se poate schimba in timpul jocului;
- adaugarea unei functionalitati de tip impartire a viewportului in 4, in fiecare parte vizualizandu-se o
camera;
- urmarire a masinii jucatorului principal de catre camera de filmat, indiferent pe ce culoar se afla;