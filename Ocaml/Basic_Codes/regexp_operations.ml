
   type regexp =
   | Empty
   | Epsilon
   | Lettre of char
   | Union of regexp * regexp
   | Concat of regexp * regexp
   | Kleene of regexp
     
     
 let rec est_vide(e: regexp):bool =
   match e with
   |Empty -> true
   |Union(e1,e2) -> est_vide e1 && est_vide e2
   |Concat(e1,e2) -> est_vide e1 || est_vide e2
   |Kleene(e) -> est_vide e
   |_ -> false
 ;;
 
 let rec a_eps(e:regexp):bool =
   match e with
   |Epsilon -> true
   |Union(e1,e2) -> a_eps e1 || a_eps e2
   |Concat(e1,e2) -> a_eps e1 && a_eps e2
   |Kleene _ -> true
   |_ -> false
 ;;
 
 let est_eps(e:regexp):bool =
   match e with
   |Vide -> false
   |Epsilon -> true
   |Union(e1,e2) -> 
    (est_eps expr1 && est_eps expr2) ||
       (est_eps expr1 && est_vide expr2) ||
         (est_vide expr1 && est_eps expr2)
   |Concat(e1,e2) ->
     est_eps e1 && est_eps e2
   |Kleene(e) -> est_eps e
   |_ -> false
 ;;
 
 let ex = Concat(Concat(Lettre('a'),Kleene(Lettre('b'))),Lettre('a'))
 ;;
 
 
 let rec residuel(c:char)(e:regexp):regexp =
   match e with
   |Empty |Epsilon -> Empty
   |Lettre(a) when a = c -> Epsilon
   |Union(e1,e2) -> Union(residuel c e1,residuel c e2)
   |Concat(e1,e2) -> Union(Concat((residuel c e1),e2),(residuel c e2))
   |Kleene(e) -> residuel c e
 ;;
 


 let rec appartient(c:char list)(r:regexp):bool =
 match c with
 |[] -> a_eps regexp
 |x::xs ->
  appartient suite (residuel x regexp)
 ;;


 


 
 
 
 
 
 
 


