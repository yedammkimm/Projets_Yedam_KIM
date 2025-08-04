



(* Ceci est un éditeur pour OCaml
Entrez votre programme ici, et envoyez-le au toplevel en utilisant le
                              bouton "Évaluer le code" ci-dessous ou [Ctrl-e]. *)

type 'a abr =
  |Vide
  |Noeud of 'a abr * 'a * 'a abr
;;

let ex = Noeud(Noeud(Noeud(Vide,2,Vide),3,Noeud(Vide,5,Vide)),5,Noeud(Vide,7,Noeud(Vide,8,Vide)))
(*Q4 parcours infixe*)
 
;;


(*Q5*)
let rec liste(a:'a abr):'a list =
  match a with
  |Vide -> []
  |Noeud(x,y,z) -> (liste x)@(y :: liste z)
 
;;

(*une fonction quadratique*)


let liste(a:'a abr): 'a list = 
  let rec aux a acc = 
    match a with 
    |Vide -> acc
    |Noeud(x,y,z) -> aux x (y:: aux z acc)
  in aux a []
;;

(*une fonction lineaire*)

let est_abr1(a: 'a abr): bool =
  let rec aux(a1:'a list)(b:bool): bool =
    match a1 with
    |[] -> b
    |x::[] -> b
    |x::y::xs ->
        if x > y then false
        else aux xs b
  in
  aux (liste a) true
    
;;

(* on doit regarder que le max de arbre gauche doit etre inferieure a la racine 
et inversement le min de arbre droit *)


let est_abr(a:'a abr):bool =
	(* Verifie si abr et trouve le min et max*)
  let rec abr_mini_maxi (a:'a abr): (bool*'a*'a) =
    match a with
    |Vide -> failwith "Cas Impossible"
    |Noeud(Vide,x,Vide) -> (true,x,x)
    |Noeud(g,x,Vide)->
        let resg,minig,maxig = abr_mini_maxi g in
        (resg && maxig <= x,minig, x)
    |Noeud(Vide,x,d) -> 
        let resd,minid,maxid = abr_mini_maxi d in 
        (resd && minid >= x, x,maxid)
    |Noeud(g,x,d)-> 
        let resd,minid,maxid = abr_mini_maxi d in
        let resg,minig,maxig = abr_mini_maxi g in
        (resd && resg && maxig <= x && minid >= x, minig,maxid)
  in
  match a with
  |Vide -> true
  |_ -> let res,_,_ = abr_mini_maxi a in res
;;




(*Q8*)

type 'a ensemble = 'a abr ;;
let vide = Vide ;;

let rec cardinal(a: 'a ensemble):int=
  match a with
  |Vide -> 0
  |Noeud(x,_,z) -> 1 + (cardinal x) + (cardinal z)
;;

(*Q9*) 


let appartient_avec_liste(x:'a)(a:'a ensemble): bool =
  let rec aux (x:'a)(a1:'a list):bool=
    match x,a1 with
    |_,[] -> false
    |_,y::ys -> if x == y then true
        else (aux x ys)
  in
  aux x (liste a)
;;

let rec appartient(x:'a)(a:'a ensemble):bool = 
  match a with 
  |Vide -> false 
  |Noeud(_,e,_) when x = e -> true 
  |Noeud(g,e,_) when x < e -> appartient x g 
  |Noeud(_,_,d) -> appartient x d
;;


        
(*Q10*) 

let rec ajouter (x:'a)(a:'a ensemble):'a ensemble =
  match a with
  |Vide -> Noeud(Vide,x,Vide)
  |Noeud(_,e,_) when e = x -> a 
  |Noeud(g,e,d) when e < x -> Noeud((ajouter x g),e,d)
  |Noeud(g,e,d) -> Noeud(g,e,(ajouter x d))
           
  (*Q12*)

let rec min_elt a =
  match a with
  | Vide -> failwith "arbre vide"
  | Noeud (Vide, e, _) -> e
  | Noeud (g, _, _) -> min_elt g
;;


let rec supprime_min_elt(a:'a abr):'a *'a abr = 
  match a with 
  |Vide -> failwith "Can't Supprime" 
  |Noeud(Vide,e,d) -> (e,d) 
  |Noeud(g,e,d)  -> 
      let x, g_del = supprime_min_elt g in 
      (x,Noeud(g_del,e,d))
      
;;
      

let rec supprime(x:'a)(a:'a ensemble):'a ensemble = 
  match a with 
  |Vide -> failwith "arbre vide" 
  |Noeud(Vide,e,d) when e = x -> d 
  |Noeud(g,e,d) when e = x -> 
      let m,d_del = supprime_min_elt d
      in Noeud(g,m,d_del) 
  |Noeud(g,e,d) when x < e -> 
      Noeud((supprime x g),e,d) 
  |Noeud(g,e,d) -> 
      Noeud(g,e,(supprime x d))
 




 
 
 


