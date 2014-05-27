
  nume: Bojin
  prenume: Andrei Florian
  grupa/seria: 323 CA

	In implementarea temei am urmat pasii:

- parcurg lista de patternuri/predicate si pentru fiecare din ele incerc sa fac match
  pe baza de date.

- folosesc o functie care primeste un pattern si o lista din baza de date si returneaza
  o lista de perechi (variabila, valoare).

- odata ce am obtinut o lista rezultata in urma apelurilor succesive ale acestei functii
  pe toata baza de date, o voi compara cu rezultatele anterioare.

- in urma compararii, daca sunt prezente perechi cu variabile noi, acestea se vor adauga
  in continuarea vechilor solutii, iar cele listele din rezultatele anterioare care nu
  au prezentat elemente comune nu vor mai fi adaugate la noile rezultate.

- daca este vorba de un predicat si nu un pattern, atunci voi inlocui in expresia din
  predicat toate variabilele cu valorile lor asociate din rezultatele anterioare, dupa
  care evaluez noua expresie si in functie de adevarul acesteia, adaug sau nu in noua
  lista de rezultate liste din vechile rezultate.

- in final voi filtra lista de rezultate in functie de variabilele care apar in lista de
  select

- afisez mesaje de eroare pentru cazurile in care prima lista din lista de SEARCH e un
  predicat sau variabilele din predicat nu au mai fost intalnite in rezultatele anterioare,
  sau in cazul in care variabilele din select nu au mai fost intalnite inainte.

	Am intarziat cu trimiterea temei din cauza zecilor de cazuri particulare pe care
a trebuit sa le acopar, progresul fiind greu si zbatandu-ma pentru fiecare test cu ore
intregi de debuguit.
	De asemenea a trebuit sa-mi schimb de multe ori modul de abordare al problemei,
mai ales al felului in care interclasam vechile rezultate cu noile.
	In final totusi, am ajuns sa ma descurc destul de bine in Scheme si acum regret ca
nu am avut aceasta indemanare si rapiditate in gandire de la inceputul temei, cand am stat
foarte mult sa "bajbai", nestiind macar cum sa incep.