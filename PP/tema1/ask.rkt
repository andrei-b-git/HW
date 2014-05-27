#|
#   nume: Bojin
#   prenume: Andrei Florian
#   grupa/seria: 323 CA
#
#   TEMA 1, Paradigme de programare: Limbaj de interogare
|#

; Interogheaza o baza de date cu un anume query. Returneaza o lista de substitutii
(define ask
  (lambda (db query)
    (if (not (equal? 'if (caadar query)))   ;daca este pattern
        (if (and (not (null? (ask1 db (cddar query) (functie (cdar query) db))))   ;daca nu am o solutie nula
                 (null? (substract (cadadr query)   ;verific daca lista de select are variabile noi fata de ce
                                   (filter (lambda (x) (variable? x))    ;am in solutie
                                           (flatten (car (ask1 db (cddar query) (functie (cdar query) db))))))))
            (select (ask1 db (cddar query) (functie (cdar query) db))   ;selectez din solutie perechile cu aceleasi
                    (cadadr query)   ;variabile ca cele din lista de select
                    '())
            (error "Unbound variables" (substract (cadadr query)   ;arunc eroare daca lista de select are variabile noi fata de 
                                                  (filter (lambda (x) (variable? x))    ;ce aveam in rezultat
                                                          (flatten (car (ask1 db (cddar query) (functie (cdar query) db))))))))
        (error "Unbound variables" (filter (lambda (x) (variable? x))   ;daca prima lista din lista de search este predicat
                                           (flatten (cadar query)))))))   ;arunca eroare

; Functie auxiliara a lui ask, in care am algoritmul efectiv
(define ask1
  (lambda (db list_search rez_ant)
    (if (null? list_search)
        rez_ant
        (if (not (equal? 'if (caar list_search)))   ;daca este pattern
            (if (and (not (null? rez_ant))   ;daca nu am lista de rezultate anterioare nula
                     (null? (intersect (filter (lambda (x) (variable? x))   ;daca am cazul in care am un pattern numai cu 
                                               (flatten (car list_search)))    ;variabile noi fata de ce aveam pana acum
                                       (filter (lambda (x) (variable? x))      ;in lista de rezultate anterioare
                                               (flatten (car rez_ant))))))
                (ask1 db (cdr list_search) (merge1 rez_ant (functie list_search db) '()))   ;fac un merge mai special
                (ask1 db (cdr list_search) (merge rez_ant (functie list_search db) '())))   ;altfel fac merge normal
            (if (and (not (null? rez_ant))    ;daca este predicat si lista de rezultate anterioare nu e nula
                     (null? (substract (filter (lambda (x) (variable? x))    ;daca nu am in predicat variabile noi fata
                                               (flatten (car list_search)))  ;de ce aveam in rezultatele anterioare
                                       (filter (lambda (x) (variable? x)) 
                                               (flatten (car rez_ant))))))
                (ask1 db (cdr list_search) (predicat (car list_search) rez_ant '())) ;aplic functia de prelucrare si match a predicatului
                (error "Unbound variables" (substract (filter (lambda (x) (variable? x))   ;daca am variabile noi, arunc eroare
                                                              (flatten (car list_search))) 
                                                      (filter (lambda (x) (variable? x)) 
                                                              (flatten (car rez_ant))))))))))


;; utility functions

; Functie care aplica lista de select pe intreaga lista de rezultate, prin apeluri succesive de select_list
(define select
  (lambda (rez list_select sol)
    (if (null? rez)
        sol
        (select (cdr rez) 
                list_select 
                (append sol 
                        (list (select_list (car rez) list_select '())))))))

; Functie care selecteaza dintr-o lista din rezultat acele perechi (variabila, valoare) care au variabilele
; in lista de select
(define select_list
  (lambda (lista_rez list_select solutie)
    (if (null? list_select)
        solutie
        (select_list lista_rez 
                     (cdr list_select) 
                     (append solutie 
                             (filter (lambda (x) 
                                       (equal? (car x) (car list_select))) 
                                     lista_rez))))))

; Functie care primeste un predicat si rezultatele anterioare, inlocuieste variabilele din predicat
; si testeaza adevarul expresiei din predicat
(define predicat
  (lambda (pred rez_ant new_rez)
    (if (null? rez_ant)
        new_rez
        (if (eval (inloc (cadr pred) (car rez_ant)))
            (predicat pred (cdr rez_ant) (append new_rez (list (car rez_ant))))
            (predicat pred (cdr rez_ant) new_rez)))))

; Functie care inlocuieste in expresia din predicat variabilele dintr-o lista a rezultatului
(define inloc
  (lambda (expr list_rez)
         (inloc1 expr '() list_rez)))

; Functie auxiliara lui inloc, care aplica algoritmul efectiv care presupune parcurgerea element
; cu element a expresiei, copierea in expr_noua a elementelor care nu sunt variabile si inlocuirea
; variabilelor cu valoarea lor asociata din lista de rezultate
(define inloc1
  (lambda (expr expr_noua list_rez)
    (if (null? expr)
        expr_noua
          (if (list? (car expr))
              (if (and (equal? (caar expr) (string->symbol "quote")) 
                       (variable? (cadar expr)) 
                       (equal? (cadar expr) (caar (select_list list_rez (list (cadar expr)) '()))))
                  (inloc1 (cdr expr) 
                          (append expr_noua (list `',(cadar (select_list list_rez (list (cadar expr)) '())))) 
                          list_rez)
                  (inloc1 (cdr expr) 
                          (append expr_noua (list (inloc (car expr) list_rez))) 
                          list_rez))
              (if (and (variable? (car expr)) 
                       (equal? (car expr) (caar (select_list list_rez 
                                                             (list (car expr)) 
                                                             '()))))
                  (inloc1 (cdr expr) 
                          (append expr_noua (list `',(cadar (select_list list_rez (list (car expr)) '())))) 
                          list_rez)
                  (inloc1 (cdr expr) 
                          (append expr_noua (list (car expr))) 
                          list_rez))))))

; Functie care primeste o lista din rezultatele anterioare si o lista din rezultatele curente si returneaza
; o lista formata din vechea lista la care s-au concatenat perechile noi de (variabila, valoare) din lista 
; de rezultate curente
(define new_lst
  (lambda (lista_veche rez_cur lista_noua)
    (if (null? rez_cur)
        lista_noua
        (if (or (null? (intersect lista_veche (car rez_cur)))   ;daca intersectia e nula
                (not (equal? (length (intersect (map (lambda (x) (car x))   ;sau numarul de variabile comune
                                                     lista_veche)           ;al celor doua liste este diferit
                                                (map (lambda (x) (car x))   ;de numarul de perechi comune al
                                                     (car rez_cur))))       ;celor doua liste
                             (length (intersect lista_veche (car rez_cur))))))
            (new_lst lista_veche (cdr rez_cur) lista_noua)
            (if (not (null? lista_noua))   ;daca intersectia nu e nula
                (if (list? (caar lista_noua))
                    (new_lst lista_veche 
                             (cdr rez_cur)   ;face concatenarea de perechi noi la vechea lista
                             (append lista_noua (list (append lista_veche (substract (car rez_cur) lista_veche)))))
                    (new_lst lista_veche 
                             (cdr rez_cur) 
                             (append (list lista_noua) (list (append lista_veche (substract (car rez_cur) lista_veche))))))
                (new_lst lista_veche 
                         (cdr rez_cur) 
                         (append lista_noua lista_veche (substract (car rez_cur) lista_veche))))))))

; Functie de interclasare a rezultatelor anterioare cu cele curente si returnarea acestora intr-o noua lista.
; Se foloseste de apeluri succesive ale new_lst
(define merge
  (lambda (rez_ant rez_cur rez_noi)
    (if (null? rez_ant)
        rez_noi
        (merge (cdr rez_ant) 
               rez_cur
               (if (not (null? (new_lst (car rez_ant) rez_cur '())))
                   (if (list? (caar (new_lst (car rez_ant) rez_cur '())))
                       (filter (lambda (x) (not (equal? x '())))
                               (append rez_noi 
                                       (new_lst (car rez_ant) rez_cur '()))) 
                       (filter (lambda (x) (not (equal? x '())))
                               (append rez_noi 
                                       (list (new_lst (car rez_ant) rez_cur '())))))
                   (filter (lambda (x) (not (equal? x '())))
                           (append rez_noi 
                                   (list (new_lst (car rez_ant) rez_cur '()))))))))) 

; Functie folosita in cazul in care se aplica un pattern pe o lista de rezultate care nu are
; in comun nici o variabila cu patternul. Va concatena la fiecare lista din rezultatele anterioare
; fiecare din listele din rezultatele curente, obtinand astfel toate variantele posibile
(define merge1
  (lambda (rez_ant rez_cur rez_noi)
    (if (null? rez_ant)
        rez_noi
        (merge1 (cdr rez_ant) 
                rez_cur
                (append rez_noi 
                        (map (lambda (x)   ;mapeaza operatia de concatenare cu o lista din rez_ant
                               (append (car rez_ant) x))   ;pe toata lista de rezultate curente
                             rez_cur))))))    

; Functie care testeaza daca o lista rezultata in urma matchului unui pattern pe o lista din db, 
; are in ea mai multe perechi de tipul (variabila, valoare) cu aceeasi valoare, atunci si valorile
; asociate variabilelor egale, trebuie sa fie egale
(define test_lst_rez
  (lambda (lst)
    (if (not (null? (filter (lambda (x) 
                              (equal? (length x) 2)) 
                            (map (lambda (x) 
                                   (filter (lambda (y) (equal? (car y) (car x))) lst)) 
                                 lst))))
        (null? (filter (lambda (x) 
                         (not (equal? (cadar x) (cadadr x)))) 
                       (filter (lambda (x) 
                                 (equal? (length x) 2)) 
                               (map (lambda (x) 
                                      (filter (lambda (y) (equal? (car y) (car x))) lst)) 
                                    lst))))
        #t)))

; Functie care primeste un pattern si db si care prin apeluri repetate de functia partial, construieste
; o lista cu rezultatele aplicarii patternului pe intreaga baza de date. Returneaza o lista de perechi
; (variabila, valoare)
(define functie
  (lambda (pattern db)
    (if (not (null? db))
          (if (and (equal? (length (car pattern)) (length (car db)))   ;daca patternul si o lista din db au aceeasi lungime
                   (or (variable? (caddar pattern))
                       (and (list? (caddar pattern))   ;pentru a aplica length trebuie sa testez mai intai daca sunt liste
                            (list? (caddar db)) 
                            (equal? (length (caddar pattern)) (length (caddar db))))   ;mapul din functia partial necesita liste de aceeasi lungime
                       (and (not (list? (caddar pattern))) (not (list? (caddar db)))))   
                   (equal? (filter (lambda (x) (not (variable? x)))   ;ma asigur ca nu exista valori dupa ce inlatur din pattern tot ce are in comun
                                   (substract (flatten (car pattern)) (flatten (car db)))) '()))   ;cu o lista din db
              (let ([p (fct (flatten1 (partial (car pattern) (car db))))])   ;p este lista cu rezultate partiale in urma aplicarii patternului pe o lista din db
                (if (and (equal? (filter (lambda (x) 
                                           (not (variable? (car x))))   ;daca nu am perechi valoare, valoare in p
                                         p) '())
                         (test_lst_rez p))   ;vezi functia de deasupra
                    (cons p (functie pattern (cdr db)))   ;este o lista buna si o adaug la rezultatele finale
                    (functie pattern (cdr db))))   ;merg mai departe
              (functie pattern (cdr db)))
        '())))

; Functie care primeste un pattern si o lista din db de aceeasi lungime cu patternul, face 
; de tipul (variabila, valoare) si returneaza o lista cu astfel de perechi. Am folosit filter
; aplicat succesiv pe un map de 2 liste care lua cate un element din fiecare si daca nu erau
; egale, le punea intr-o lista. De asemenea am tratat cazul in care exista liste imbricate
(define partial 
  (lambda (l1 l2)
    (filter (lambda (x) 
              (not (equal? x '())))
            (map (lambda (x y)
                   (if (and (list? x) (list? y))
                       (if (equal? (length x) 2)
                           (partial x y)
                           (let partial1 ([x1 x] [y1 y])
                             (filter (lambda (x) 
                                       (variable? (car x)))
                                     (fct (filter (lambda (x) 
                                                    (not (equal? x '())))
                                                  (map (lambda (x y)
                                                         (if (and (list? x) (list? y))
                                                             (if (equal? (length x) 2)
                                                                 (partial x y)
                                                                 (partial1 x y))
                                                             (if (not (equal? x y)) 
                                                                 (list x y) 
                                                                 '())))
                                                       x1
                                                       y1))))))
                       (if (and (not (equal? x y))   ;daca nu sunt egale elementele
                                (not (or (and (not (list? x)) (list? y) (not (variable? x))) 
                                         (and (list? x) (not (list? y))))))   ;daca sunt ori ambele liste ori ambele neliste
                           (list x y)   ;le imperechez (var, val)             ;sau una e variabila si cealalta lista
                           '())))
                 l1
                 l2))))

; Functie asemanatoare lui flatten, pentru inlaturarea parantezelor la un anumit nivel
(define fct
  (lambda (lst)
    (if (null? lst)
        '()
        (if (list? (caar lst))
            (if (equal? (length (car lst)) 1)
                (append (list (caar lst)) (fct (cdr lst)))
                (append (car lst) (fct (cdr lst))))
            (append (list (car lst)) (fct (cdr lst)))))))

; Returneaza elementele comune si lui x si lui y
(define intersect
  (lambda (x y)
    (if (null? x)
        x
        (if (member (car x) y)
            (cons (car x) (intersect (cdr x) y))
            (intersect (cdr x) y)))))

; Extrage din x elementele comune si lui x si lui y
(define substract
  (lambda (x y)
    (cond ((null? x) x)
          ((member (car x) y) (substract (cdr x) y))
          (else (cons (car x) (substract (cdr x) y))))))

; Functia standard flatten, pentru inlaturarea parantezelor dintr-o lista
(define (flatten L)
  (flatten-iter L '()))

; Functie auxiliara a lui flatten, in care am algoritmul efectiv
(define (flatten-iter L result)
  (cond ((null? L)
         result)
        ((list? (car L))
         (flatten-iter (append (car L) (cdr L)) result))
        (else (flatten-iter (cdr L) (append result (list (car L)))))))

; Un flatten modificat astfel incat sa inlature parantezele numai pana la un anumit nivel
(define (flatten1 L)
  (flatten-iter1 L '()))

; Functie auxiliara a lui flatten1, in care am algoritmul efectiv
(define (flatten-iter1 L result)
  (cond ((null? L)
         result)
        ((list? (caar L))
         (flatten-iter1 (append (car L) (cdr L)) result))
        (else (flatten-iter1 (cdr L) (append result (list (car L)))))))

; Verifica daca parametrul dat este o variabila
(define (variable? sym)
  (and (symbol? sym) (equal? (string-ref (symbol->string sym) 0) #\?)))

