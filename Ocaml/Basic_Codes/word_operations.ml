(* Ceci est un éditeur pour OCaml
   Entrez votre programme ici, et envoyez-le au toplevel en utilisant le
   bouton "Évaluer le code" ci-dessous ou [Ctrl-e]. *)

let rec test_prefixe(m:char list)(t:char list): bool =
  match m, t with
  |[], _ -> true
  |_, [] -> false
  |x::xs, y::ys -> if x = y then
        test_prefixe xs ys
      else false
;;

let ex1 = ['c';'o';'m';'p';'u';'t';'e';'r']
let ex2 = ['o';'m']
let ex3 = ['t';'e';'r']

let rec test_suffixe(m: char list)(t:char list):bool =
  test_prefixe (List.rev m) (List.rev t)
;;

  
let rec test_facteur(m: char list)(t:char list):bool =
  if (test_prefixe m t) || (test_suffixe m t) then true
  else
    match m with
    |[] -> true
    |x::xs -> test_facteur xs t
;;

let rec test_sous_mot(m: char list)(t:char list): bool=
  match m, t with
  |_,[] -> false
  |x::xs , y::ys -> if x = y then
        test_sous_mot xs t
      else test_sous_mot m ys
  |[],_ -> true
 
;;
let rec prefixe(t:char list): char list list = 
  match t with 
  |[] -> [[]]
  |x::xs -> 
      [] :: List.map(fun e -> x::e) (prefixe xs)

;;

let rec suffixe (t:char list): char list list =
  match t with
  |[] -> [[]]
  |_::xs -> t::(suffixe xs)
               
let rec facteur(t:char list): char list list = 
  match t with 
  |[] -> [[]]
  |x::xs -> 
      (prefixe t)@(facteur xs)
    
let rec union u v =
  List.fold_left (fun acc elt -> if List.mem elt acc then acc else elt :: acc) v u
    
    
let rec sous_mots t = 
  match t with 
  |[] -> [[]]
  |x::xs -> 
      let s = sous_mots xs in 
      union s (List.map(fun e -> x::e)s) 

      
let rec est_dans_mot_bis(s:char list)(m:char list):bool = 
  let rec commence m s = 
    match m, s with 
    |[], _ -> true 
    |_,[] -> false 
    |x::xs, y::ys when x = y -> 
        commence xs ys 
    |_ -> false 
  in 
  match s with 
  |[] -> m = [] 
  |x::xs -> 
      commence m s || est_dans_mot_bis m xs 
;;


 
 



