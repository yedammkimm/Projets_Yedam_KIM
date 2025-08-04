


type regexp =
|Empty 
|Epsilon 
|Lettre of char 
|Union of regexp * regexp
|Concat of regexp * regexp
|Kleene of regexp



(*Etape 1 *)

(*Pour linearliser l'expression, On peut utiliser un tableau de caracteres tels que les indices signifient les numerotations *)

let list_to_tab l1 =
let n = List.length l1 in  
let tab = Array.make n ' ' in 
let l = ref l1 in
for i = 0 to n-1 do 
  match l1 with 
  |[] -> ()
  |x::xs -> 
      begin
        tab.(i) <- x;
        l := xs 
      end
done;
tab 
;;

(*C'est plus facile de transformer expression reguiliers a liste de caracteres avant de mettre au tableau*)
let linearliser e = 
let rec regexp_to_list e =
  match e with 
  |Empty |Epsilon -> [] 
  |Lettre a -> [a]
  |Union(e1,e2)|Concat(e1,e2) -> 
      (regexp_to_list e1)@(regexp_to_list e2)
  |Kleene e -> 
      regexp_to_list e 
in 
list_to_tab (regexp_to_list e)

;;

(*Etape 2*)

(*Pour representer les prefixes, suffixes, facteurs, en utilisant les numerotations de lettres dans le tableau, on cree liste des indices de prefixes,suffixes,facteurs *)


(*On change le type de regexp, mais explin car ce qu'on interesse c'est la numeration plutot que le lettre*)
type explin =
|Empty 
|Epsilon 
|Number of int
|Union of explin * explin
|Concat of explin * explin
|Kleene of explin

let rec regexp_to_explin (e:regexp): explin = 
let id = ref 0 in
match e with 
|Empty -> Empty 
|Epsilon -> Epsilon
|Lettre a -> 
    incr id;
    Number(!id)
|Union(e1,e2) ->
    Union(regexp_to_explin e1, regexp_to_explin e2)
|Concat(e1,e2) -> 
    Concat(regexp_to_explin e1, regexp_to_explin e2) 
|Kleene e -> 
    Kleene(regexp_to_explin e)

;;



let rec empty_word e =
match e with 
|Empty -> true 
|Epsilon |Number _ -> false 
|Union (e1,e2) -> empty_word e1 || empty_word e2 
|Concat (e1,e2) -> empty_word e1 && empty_word e2 
|Kleene e -> false

;;

let rec have_eps e = 
match e with 
|Epsilon |Kleene _  -> true 
|Union (e1,e2) -> have_eps e1 || have_eps e2 
|Concat(e1,e2) -> have_eps e1 && have_eps e2 
|Number _ |Empty -> false 

;;


let rec union l1 l2 = 
match l1 with 
|[] -> l2 
|x::xs -> 
    if List.mem x l2 then 
      union xs l2 
    else 
      x::union xs l2 
;;


let rec suffixe e =
match e with 
|Empty |Epsilon -> []
|Number a -> [a]
|Union(e1,e2) -> 
    union (suffixe e1) (suffixe e2)
|Concat(e1,e2) -> 
    if have_eps e2 then 
      union (suffixe e1) (suffixe e2)
    else 
      suffixe e2
|Kleene e -> suffixe e


;;

let rec prefixe e = 
match e with 
|Empty |Epsilon -> []
|Number a -> [a]
|Union(e1,e2) -> 
    union (prefixe e1) (prefixe e2)
|Concat(e1,e2)-> 
    if have_eps e1 then 
      union (prefixe e1) (prefixe e2)
    else 
      prefixe e1
|Kleene e -> prefixe e

;;  



let rec facteurs e =
let rec produit l1 l2 = 
  match l1 with 
  |[] -> []
  |x::xs -> 
      List.map (fun y -> (x,y)) l2
      @ produit xs l2 
in 
match e with 
|Empty -> [] 
|Epsilon -> [] 
|Number _ -> []
|Union(e1,e2) -> 
    union (facteurs e1) (facteurs e2)
|Concat(e1,e2) -> 
    if empty_word e1 || empty_word e2 then 
      union (facteurs e1) (facteurs e2)
    else 
      union (union (facteurs e1) (facteurs e2)) (produit (suffixe e1) (prefixe e2))
|Kleene e -> 
    union (facteurs e) (produit (prefixe e) (suffixe e))

;;






