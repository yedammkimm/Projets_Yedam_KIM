


let u0 = 241 ;;

(*Q1*)
let create_tab n =
  let tab = Array.make (n+1) u0 in
  for i = 1 to n do
    tab.(i) <- 15091 * tab.(i-1) mod 64007
  done;
  tab
;;


(*
  let q1_a = (create_tab 17).(17) ;;

let q1_b = (create_tab 42).(42) ;;

let q1_c = (create_tab 8000).(8000) ;;

*)
(*Q2*)

type sign =
  |Plus of int
  |Minus of int 
;;


let taille (p,q,r) =
  let tab = create_tab (q*r+r) in
  let clause = Array.make q [] in
  let m = ref 0 in
  for j = 0 to q-1 do
    for k = 0 to r-1 do
      begin
        let a = tab.(j*r+k) mod p in
        if tab.(j*r+k) < 32003 then
          begin
            if not(List.mem (Plus(a)) clause.(j)) then
              clause.(j) <- Plus(a)::clause.(j)
          end
        else
          begin
            if not(List.mem (Minus(a)) clause.(j)) then
              clause.(j) <- Minus(a)::clause.(j)
          end
      end
    done;
  done;
  for i = 0 to q-1 do
    m := (List.length clause.(i)) + !m
  done;
  !m,clause
;;

(* 	 
let q2_a = taille (4,15,2) ;;

let q2_b = taille (3,30,3) ;;

let q2_c = taille (8,60,3) ;;

let q2_d = taille (10,100,4) ;;


*)
(*Q3*)

(*Utile pour remove un element de la liste*)

let same_var a b =
  match a,b with
  |Plus(x),Plus(y) ->
      if x = y then true
      else false
  |Minus(x),Minus(y) ->
      if x = y then true
      else false 
  |_ -> false
;;

let rec remove_var l a =
  match l with
  |[] -> l
  |x::xs ->
      if same_var x a then
        xs
      else
        x::(remove_var xs a)
;;

 
let nb_clauses_trivial (p,q,r) =
  let (_,tab) = taille (p,q,r) in
  let count = ref 0 in
  let rec explore l n =
    if n = 1 then l  
    else
      match l with
      |[] -> l
      |Plus(x)::xs ->
          if (List.mem (Minus(x)) xs) then
            begin
              incr count;
              []
            end
          else
            explore (xs@[Plus(x)]) (n-1)
      |Minus(x)::xs ->
          if (List.mem (Plus(x)) xs) then
            begin
              incr count; 
              []
            end
          else
            explore (xs@[Minus(x)]) (n-1) 
  in
  for i = 0 to q-1 do
    match tab.(i) with
    |[] -> ()
    |_ ->
        let n = List.length tab.(i) in
        tab.(i) <- explore tab.(i) n
  done;
  !count,tab
;;


let q3_a = nb_clauses_trivial (4,15,2) ;;

let q3_b = nb_clauses_trivial (3,30,3) ;;

let q3_c = nb_clauses_trivial (8,60,3) ;;

let q3_d = nb_clauses_trivial (10,100,4) ;; 

    
(*Q4*) 

let var a = 
  match a with 
  |Plus(x) -> x 
  |Minus(x) -> x
;;


let rec replace_neg l a = 
  match l with 
  |[] -> [] 
  |x::xs -> 
      match x with 
      |Plus(y) -> x::(replace_neg xs a)
      |Minus(y) -> if (var a) = y then xs 
          else 
            x::(replace_neg xs a)
;; 


let rec replace_val l1 l2 a = 
  match l1 with 
  |[] -> l2 
  |x::xs -> 
      match x with 
      |Plus(y) -> if y = (var a) then 
            []
          else 
            (replace_val xs (l2@[x]) a)
      |Minus(y) ->
          (replace_val xs (l2@[x]) a)
;;



let reverse a = 
  match a with 
  |Plus(x) -> Minus(x) 
  |Minus(x) -> Plus(x) 

;;


let nb_variables_forces (p,q,r) =
  let (_,tab) = nb_clauses_trivial (p,q,r) in 
  let valeurs_forces = ref [] in 
  let m = ref 0 in 
  let rec find_valeurs_forces l i =
    match l with 
    |x::[] -> 
        begin
          incr m;
          if not(List.mem x !valeurs_forces) then 
            begin
              valeurs_forces := !valeurs_forces@[x];
            end;
          tab.(i) <- replace_val tab.(i) [] x ;
          tab.(i) <- replace_neg tab.(i) (reverse x);
          tab.(i) <- replace_neg tab.(i) x;
          tab.(i) <- replace_val tab.(i) [] (reverse x)
        end
    |_ -> ()
  in
  for i = 0 to q-1 do
    match tab.(i) with
    |[] -> ()
    |_ ->
        find_valeurs_forces tab.(i) i
  done;
  !m, !valeurs_forces
;;
  



let q4_a = nb_variables_forces (4,15,2) ;;

let q4_b = nb_variables_forces (3,30,3) ;;

let q4_c = nb_variables_forces (8,60,3) ;;

let q4_d = nb_variables_forces (10,100,4) ;;


(*Q5*)

exception No_val_force
  
let taille_apres_une_valeur_force (p,q,r) =
  let (_,tab) = nb_clauses_trivial (p,q,r) in
  let (_,valeurs_forces) = nb_variables_forces (p,q,r) in 
  let m = ref 0 in 
  let first_val_force l = 
    match l with 
    |[] -> raise No_val_force
    |x::xs -> x 
  in 
  let erase a = 
    for i = 0 to q-1 do 
      begin
        tab.(i) <- replace_val tab.(i) [] a ;
        tab.(i) <- replace_neg tab.(i) (reverse a);
        tab.(i) <- replace_neg tab.(i) a;
        tab.(i) <- replace_val tab.(i) [] (reverse a) ;
        m:= List.length(tab.(i))+ !m 
      end 
    done;
    !m,tab
  in
  try 
    erase (first_val_force valeurs_forces) with 
  |No_val_force -> 
      begin
        for j = 0 to q-1 do 
          m:= List.length(tab.(j))+ !m 
        done;
      end;
      !m,tab 
;; 

let q5_a = taille_apres_une_valeur_force (4,15,2) ;;

let q5_b = taille_apres_une_valeur_force (3,30,3) ;;

let q5_c = taille_apres_une_valeur_force (8,60,3) ;;

let q5_d = taille_apres_une_valeur_force (10,100,4) ;;
    
    
;;
  



(*Q6*)  

(* La fonction ne marche pas 
   Idee: Par la liste de valeur force, On remplace soit 1 ou soit 0 par rapport au signe 
   de cette variable, puis on trouve encore la valeur force, Jusqu'a on peut plus avoir 
   de valeurs forces*)

let taille_after_all_valeurs_forces (p,q,r) = 
  let (_,tab) = taille_apres_une_valeur_force (p,q,r) in 
  let (_,liste) = nb_variables_forces (p,q,r) in 
  let valeurs_forces = ref liste in 
  let m = ref 0 in 
  let rec replace_neg1 l1 l2 a i =
    match l1 with 
    |[] -> 
        tab.(i) <- l2
    |x::[] -> 
        if a = x then 
          begin 
            tab.(i) <- l2
          end 
        else 
          ()
    |x::xs -> 
        if a = x then 
          begin 
            tab.(i) <- xs@l2
          end
        else 
          replace_neg1 xs (l2@[x]) a i 
                     
  in 
  let rec replace_val1 l1 l2 a i = 
    match l1 with 
    |[] -> 
        tab.(i) <- l2
    |x::xs -> 
        if a = x then 
          tab.(i) <- xs@l2
        else 
          replace_val1 xs (l2@[x]) a i ; 
  in 
  let rec delete_from_list l a = 
    match l with 
    |[] -> [] 
    |x::xs -> 
        if x = a then xs
        else 
          x::(delete_from_list xs a) in 
  let erase a = 
    match a with 
    |Plus(x) ->
        begin
          for i = 0 to q-1 do 
            begin
              replace_val1 tab.(i) [] a i; 
              replace_neg1 tab.(i) [] (reverse a) i; 
            end 
          done; 
          valeurs_forces := delete_from_list !valeurs_forces a
        end
    |Minus(x) -> 
        begin
          for i = 0 to q-1 do 
            begin
              replace_neg1 tab.(i) [] a i; 
              replace_val1 tab.(i) [] (reverse a) i 
            end 
          done;
          valeurs_forces := delete_from_list !valeurs_forces a
        end
  in
  let explore_toutes_valeurs_forces l = 
    match l with 
    |[] -> () 
    |x::xs -> 
        begin 
          erase x;
          List.iter erase xs 
        end 
  in
  let find_valeurs_forces l i = 
    match l with 
    |x::[] -> 
        if not(List.mem x !valeurs_forces) then 
          valeurs_forces := !valeurs_forces@[x] 
    |_ ->() 
  in
  while !valeurs_forces <> [] do 
    explore_toutes_valeurs_forces !valeurs_forces;
    for i = 0 to q-1 do 
      begin 
        find_valeurs_forces tab.(i) i 
      end
    done;
  done;
  for i = 0 to q-1 do 
    m:= List.length tab.(i) + !m
  done;
  !m,tab
      
    
  
        


;;

    
let q6_a = taille_after_all_valeurs_forces (4,15,2) ;;

let q6_b = taille_after_all_valeurs_forces (3,30,3) ;;

let q6_c = taille_after_all_valeurs_forces (8,60,3) ;;

let q6_d = taille_after_all_valeurs_forces (10,100,4) ;;
    
    
;;
 
 









