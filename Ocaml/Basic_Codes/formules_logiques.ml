type formule =
 | Var of int
 | Neg of formule
 | Et of formule * formule
 | Ou of formule * formule
 | Implique of formule * formule


type litteral = int
type clause = int list  (* triée *)




let f1 = Implique(Implique(Var(1),Var(2)),Var(3));;


let f2 = Ou(Implique(Neg(Var(1)),Neg(Var(2))),Implique(Var(3),Var(1)));;


(*On suppose que n >= 1*)
let rec make_f3 (n:int) =
 match n with
 |1 -> Ou(Var(1),Var(2))
 |_ ->
   Ou((make_f3 (n-1)),Et(Var(n+1),Var(n)))


;;


let rec taille f =
 match f with
 |Var(x) -> 1
 |Neg(x) -> 1 + (taille x)
 |Et(x1,x2) -> (taille x1) + (taille x2)
 |Ou(x1,x2) -> (taille x1) + (taille x2)
 |Implique(x1,x2) -> (taille x1) + (taille x2)


;;


let rec sans_implique f =
 match f with
 |Var(x) -> Var(x)
 |Implique(x1,x2) -> Ou(Neg(x1),x2)
 |Et(x1,x2) -> Et((sans_implique x1),(sans_implique x2))
 |Ou(x1,x2) -> Ou((sans_implique x1),(sans_implique x2))
 |Neg(x1) -> Neg((sans_implique x1))
;;




let rec descendre_non f =
 let rec in_neg f1 =
   match f1 with
   |Var(x) -> Neg(Var(x))
   |Neg(x) -> x
   |Et(x1,x2) -> Ou((in_neg x1),(in_neg x2))
   |Ou(x1,x2) -> Et((in_neg x1),(in_neg x2))
   |Implique(x1,x2) -> in_neg(sans_implique f1)
 in
 match f with
 |Var(_) -> f
 |Implique(x1,x2) -> descendre_non (sans_implique f)
 |Et(x1,x2) -> Et((descendre_non x1),(descendre_non x2))
 |Ou(x1,x2) -> Ou((descendre_non x1),(descendre_non x2))
 |Neg(x) -> in_neg x
 ;;




(* Étape 3 : on fait remonter le connecteur `et` et descendre le connecteur `ou` *)
let rec descendre_ou = function
 | Ou (f1, f2) -> begin
     match descendre_ou f1, descendre_ou f2 with
     | f1, Et (f2, f3) | Et (f2, f3), f1 ->
        descendre_ou (Et (Ou (f1, f2), Ou (f1, f3)))
     | f1, f2 -> Ou (f1, f2)
   end
 | Et (f1, f2) -> Et (descendre_ou f1, descendre_ou f2)
 | f -> f




;;


let rec fnc f =
 descendre_ou (descendre_non(sans_implique f))
;;




let est_litteral = function
 | Var _ | Neg (Var _) -> true
 | _ -> false


let rec est_clause = function
 | Ou (f1, f2) ->
    (est_litteral f1 || est_clause f1) && (est_litteral f2 || est_clause f2)
 | f -> est_litteral f


let rec est_fnc = function
 | Et (f1, f2) -> est_fnc f1 && est_fnc f2
 | f -> est_clause f


let rec nb_clauses = function
 | Et (f1, f2) -> nb_clauses f1 + nb_clauses f2
 | _ -> 1






;;




let f3 = make_f3 20
let fnc_f1 = (fnc f1)
let fnc_f2 = (fnc f2)
let fnc_f3 = (fnc f3)


let test_fnc_f3 = (est_fnc fnc_f3)


let compare_taille_f3 =
 taille fnc_f3, nb_clauses fnc_f3
;;


let compare_clauses_f3 =
 nb_clauses f3, nb_clauses fnc_f3




;;




let rec forme_clausale f =
 let rec count_clause f1 =
   match f1 with
   |Var(x) -> [[x]]
   |Neg(x) ->
     match x with
     |Var(y) -> [[-y]]
     |_ -> count_clause (descendre_non(Neg(x)))
   |Et(x1,x2) -> [(count_clause x1)]@[(count_clause x2)]
   |Ou(x1,x2) -> [(count_clause x1)@(count_clause x2)]
   |Implique(x1,x2) -> [count_clause(sans_implique(f1))]
 in
 count_clause (fnc f)
;;











