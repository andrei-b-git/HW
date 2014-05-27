; nume: Bojin
; prenume: Andrei Florian
; grupa/seria: 323 CA

; Copie a situatiei de joc, necesara in testarea diferitelor alte
; situatii ce se pot ivi pe parcurs. Am adaugat in plus campul dir
; care contine directia pe care se deplaseaza mingea.
(deftemplate copy_world
	(multislot limit)
	(multislot ball)
	(multislot men)
	(slot id)
	(multislot moves)
	(multislot dir))

; Regula care creaza o copie a unei situatii de joc deja existente,
; si adauga pozitia actuala a mingii in lista de mutari si initializeaza
; directia pe care se deplaseaza mingea cu coordonatele 0, 0.
(defrule init_copy_world
	(declare (salience 4))
	(world (limit ?rows ?cols)
	       (ball ?x ?y)
	       (men $?men)
	       (id ?id)
	       (moves $?moves))
=>
	(assert (copy_world (limit ?rows ?cols)
			    (ball ?x ?y)
			    (men $?men)
			    (id ?id)
			    (moves $?moves ?x ?y -)
			    (dir 0 0))))

; Regula care filtreaza situatiile de joc, inlaturandu-le pe cele care
; s-au solutionat (au un fapt win cu id-ul lor).
(defrule filter_world
	(declare (salience 4))
	(win (id ?id))
	?w <- (world (id ?id))
=>
	(retract ?w))
	
; Regula care filtreaza copiile situatiilor de joc, inlaturandu-le pe 
; cele care s-au solutionat (au un fapt win cu id-ul lor).
(defrule filter_copy_world
	(declare (salience 4))
	(win (id ?id))
	?c <- (copy_world (id ?id))
=>
	(retract ?c))

; Regula care testeaza daca o anumita copie a unei situatii de joc s-a
; solutionat (daca mingea a ajuns sau a depasit ultima linie a terenului)
; si o marcheaza, adaugand un fapt win cu id-ul ei.	
(defrule test_goal
	(declare (salience 3))
	(copy_world (limit ?rows ?cols)
		    (ball ?x ?y)
		    (men $?men)
		    (id ?id)
		    (moves $?moves))
	(test (= ?rows (+ ?x 1)))
=>
	(assert (win (id ?id) (moves $?moves))))

; Regula care identifica acele situatii de joc unde mingea a ajuns pe
; aceeasi pozitie cu un jucator, inlatura jucatorul respectiv si muta
; mingea in continuare pe directia pe care se afla si inainte. Cum
; situatia de joc initiala s-a schimbat va trebui inlocuita cu retract/
; assert.
(defrule move_replace
	(declare (salience 2))
	?c <- (copy_world (limit ?rows ?cols)
			  (ball ?x ?y)
			  (men $?men1 ?x ?y - $?men2)
			  (id ?id)
			  (moves $?moves ?xm ?ym -)
			  (dir ?xd ?yd))
=>
	(retract ?c)
	(assert (copy_world (limit ?rows ?cols)
			    (ball (+ ?x ?xd) (+ ?y ?yd))
			    (men $?men1 $?men2)
			    (id ?id)
			    (moves $?moves (+ ?x ?xd) (+ ?y ?yd) -)
			    (dir ?xd ?yd))))

; Regula care testeaza daca vecinii pozitiei actuale a mingii sunt
; jucatori si indreapta mingea pe directia acestora, creand o noua
; situatie de joc.
(defrule test_neighbours
	(declare (salience 1))
	(copy_world (limit ?rows ?cols)
		    (ball ?x ?y)
		    (men $?men1 ?xm ?ym - $?men2)
		    (id ?id)
		    (moves $?moves)
		    (dir ?xd ?yd))
	(test (and (<= (abs (- ?xm ?x)) 1) 
		   (<= (abs (- ?ym ?y)) 1)))
=>
	(assert (copy_world (limit ?rows ?cols)
			    (ball ?xm ?ym)
			    (men $?men1 ?xm ?ym - $?men2)
			    (id ?id)
			    (moves $?moves ?xm ?ym -)
			    (dir (- ?xm ?x) (- ?ym ?y)))))

; Regula care inlatura toate situatiile de joc pentru a lasa in final
; doar faptele win.
(defrule remove_world
	?w <- (world (id ?id))
=>
	(retract ?w))

; Regula care inlatura toate copiile situatiilor de joc pentru a 
; lasa in final doar faptele win.
(defrule remove_copy_world
	?c <- (copy_world (id ?id))
=>
	(retract ?c))

