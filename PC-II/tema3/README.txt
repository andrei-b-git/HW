nume: Bojin
prenume: Andrei Florian
grupa/seria: 323 CA

	Pentru a rezolva tema, am pornit de la scheletul din laboratorul 8 (de TCP). Informatiile despre fiecare client in parte le-am salvat intr-un numar de vectori: unul 
pentru socketul pe care clientul si serverul comunica, unul pentru numele clientului, unul pentru timpul de conectare, unul pentru portul pe care clientul asculta si un vector pentru adresa clientului (IP + port). Am considerat ca la un moment dat nu pot fi conectati mai mult de 10 clienti. Fiecare client pastreaza informatii despre fisierele pe care trebuie sa le
trimita (in vectori pentru filedescriptor, pentru nume si adresa clientului destinatar).
	Comenzile de la tastatura si mesajele dintre clienti/server sunt parsate pentru a determina actiunea de executat. Prin urmare, am tratat o serie de erori, spre exemplu
daca un client incearca sa se conecteze la server cu un nume care exista deja, sau daca se da alta comanda decat cele specificate in enunt.
	In bucla principala, clientul verifica daca mai are fisiere de transmis. Din primul fisier gasit in lista se citeste un pachet, si este trimis la destinatar, apoi apeleaza select. La fiecare pachet trimis deschid o conexiune noua (prin urmare un socket nou) si apoi o inchid.

Observatii:

	Cand am testat comportamentul pentru fisiere mari, am constatat ca dupa un numar de pachete, apelul connect da eroare, si am presupus ca acest lucru se intampla pentru ca sistemul a ramas fara socketi liberi.
	Am incercat sa apelez cu un timeout, dar din motive pe care nu le inteleg, acest lucru nu a functionat, si ca urmare la finalul buclei principale, am apelat sleep pentru
4 milisecunde. Implementand astfel, am reusit sa transmit fisiere mai mari fara probleme, cu toate ca dureaza destul de mult (80 Mb se transmit in aprox. 5 minute).

	Din pacate nu am mai avut timp sa implementez si functiile: broadcast si history la client si kick la server.

