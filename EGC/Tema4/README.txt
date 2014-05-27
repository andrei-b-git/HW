/*
 *				Elemente de grafica pe calculator (EGC)
 *
 * 					TEMA 4: SLEEPING BEAUTY
 *
 * nume:				Bojin
 * prenume:		Andrei Florian
 * grupa/seria: 	333 CA
 */
 
 1. Cerinta:
	Implementarea unei sistem de animatie faciala ce reactioneaza la un sistem de particule.
	
2. Utilizare:
	Nu necesita fisiere de intrare sau de iesire.
			
	Tastele de CONTROL sunt:
- ESC ... iesire;
- SPACE ... reincarca shadere;
- W ... toggle wireframe;
- F1 ... activare FullScreen;
- F2 ... dezactivarea FullScreen;

3. Implementare:
	Tema a fost realizata in Visual Studio 2012 pe Windows 7 Ultimate.
	
	Am realizat doar prima parte a temei: animatia fetei, impreuna cu iluminarea acesteia.	
	M-am folosit de Framework-ul  de la laboratorul lab_gl_5_student de EGC. 
	Implementarea consta in:
- definirea la inceputul sursei main a unor variabile necesare in rezolvarea cerintei temei;
- constructorul clasei in care realizez toate initializarile necesare;
- functia setupFrame unde am obtinut atributele vao si num_indices ale fiecarei stari, pornind de
la vertices si indices a 2 stari: starea curenta pentru care vroiam sa obtin atributele si starea urmatoare.
Astfel, in vertex shader trimiteam doua seturi de vertices si indices, avand doua serii de locatii: una
pentru starea curenta si una pentru starea urmatoare;
- functia notifyBeginFrame unde calculam timpul trecut de la starea curenta pentru a-l trimite apoi
in functia notifyDisplayFrame la shader. De asemenea, calculam surplusul de timp la trecerea intre
doua stari, urmand sa-l scad din timpul cadrului urmator. Astfel la trecerea dintre doua stari, acest
surplus de timp nu intra in timpul cadrului curent, acesta trunchindu-se la timpul maxim necesar
trecerii in starea urmatoare, iar surplusul de timp va conta in calculul timpului cadrului urmator;
- functia notifyDisplayFrame unde trimit toate variabilele la shader;
- functia notifyEndFrame unde vad daca este necesara schimbarea starii curente si realizez initializarile
respective daca este cazul;
	
4. Testare:
	Tema a fost rulata si testata in Visual Studio 2012 pe Windows 7 Ultimate.
	De asemenea, daca tema este testata pe un laptop cu 2 procesoare grafice, se recomanda
folosirea GPU-ului de la placa video in deschiderea si rularea Visual Studio pentru o performanta
marita.

5. Probleme aparute:
	Am incercat putin rezolvarea si partii a doua a temei dar din cauza dificultatilor aparute pe
parcurs si a lipsei de timp si a frustrarii faptului ca nu mergea deloc, am renuntat si am sters
toata tentativa de rezolvare ce tinea de partea a doua a temei, lasand doar prima parte.

6. Continutul arhivei:
	Frameworkul de la laboratorul lab_gl_5_student de unde am sters fisierul de tip SQL Server 
Compact Edition Database care ocupa peste 30MB pentru a putea uploada arhiva.
	Fisierul README.txt

7. Functionalitati:
	
	Functionalitati Standard (ca in enunt):
- animatie de calitate
- trimiterea corecta de la CPU la GPU
- algoritmul realizarii animatiei
- iluminare Phong in fragment shader
- texturarea fetei
