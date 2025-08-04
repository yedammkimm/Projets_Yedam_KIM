(* Taille du plateau *)
let n = 5

let noms_joueurs = [|"Horace"; "Vera"|]

(** Fonctions de jeux **)

(* Joueur, positions du joueur, positions de l'autre joueur *)
type etat = int * int array * int array
(* Indice de ligne ou de colonne et déplacement *)
type coup = int * int
(* Stratégies *)
type strategie = etat -> coup

let configuration_initiale =
  let ini = Array.make n 0 in
  ini.(0) <- n - 1;
  ini.(n - 1) <- n - 1;
  0, ini, ini

let tour_suivant (joueur, moi, lui) =
  (1 - joueur, lui, moi)

let terminal (joueur, moi, lui) =
  if moi = Array.make n (n - 1) then
    Some joueur
  else if lui = Array.make n (n - 1) then
    Some (1 - joueur)
  else
    None

let joue (joueur, moi, lui) (i, d) =
  (* On fait bien une copie pour ne pas modifier le tableau `moi` *)
  let moi = Array.copy moi in
  (* On joue le coup et on passe à l'autre joueur *)
  moi.(i) <- moi.(i) + d;
  tour_suivant (joueur, moi, lui)

let coups_possibles (_, moi, lui) =
  let res = ref [] in
  for i = 1 to n - 2 do
    let j = moi.(i) in
    if j < n - 1 then begin
      if lui.(j + 1) != i then res := (i, 1) :: !res
      else if lui.(j + 1) = i && lui.(j + 2) != i then res := (i, 2) :: !res
    end
  done;
  if !res = [] then [(1, 0)] else !res

(** Fonction d'affichage **)

let affiche (joueur, moi, lui) =

  let hpos, vpos =
    match joueur with
    | 0 -> moi, lui
    | 1 -> lui, moi
    | _ -> failwith "Le joueur doit être 0 ou 1"
  in

  print_string "    ";
  for i = 1 to n - 2 do
    Printf.printf "%i " i
  done;
  print_newline ();

  let p = Array.make_matrix n n ' ' in
  p.(0).(0) <- 'x';
  p.(0).(n - 1) <- 'x';
  p.(n - 1).(0) <- 'x';
  p.(n - 1).(n - 1) <- 'x';
  for i = 1 to n - 2 do
    p.(i).(hpos.(i)) <- '>';
    p.(vpos.(i)).(i) <- 'v';
  done;

  for i = 0 to n - 1 do
    if i = 0 || i = n - 1 then print_char ' ' else print_int i;
    print_char ' ';
    for j = 0 to n - 1 do
      Printf.printf "%c " p.(i).(j);
    done;
    print_newline ()
  done;
  print_newline ()

(** Fonction de partie **)

let partie etat s1 s2 affichage =
  let rec jouer etat s1 s2 =
    if affichage then affiche etat;
    let coup = s1 etat in
    let joueur, _, _ = etat in
    if affichage then begin
        if snd coup = 0 then
          Printf.printf "%s passe.\n\n" noms_joueurs.(joueur)
        else
          Printf.printf "%s joue en %d.\n\n" noms_joueurs.(joueur) (fst coup)
    end;
    let etat = joue etat coup in
    match terminal etat with
    | Some joueur ->
       if affichage then begin
         affiche etat;
         Printf.printf "%s a gagné !\n" noms_joueurs.(joueur)
       end;
       joueur
    | None -> jouer etat s2 s1
  in
  jouer etat s1 s2

let parties etat s1 s2 nb_parties =
  let nb_gagnes = Array.make 2 0 in
  for i = 1 to nb_parties do
    let g = partie etat s1 s2 false in
    nb_gagnes.(g) <- 1 + nb_gagnes.(g)
  done;
  nb_gagnes

;;





(** Stratégies **)

let lire_entier () =
  try read_int ()
  with Failure _ -> 0

;;

let coup_aleatoire etat =
  let cou = coups_possibles etat in 
  match cou with
  |[] -> (1,0)
  |[(1,0)] ->  (1,0)
  |x::xs -> 
    let len = List.length cou in 
    List.nth cou (Random.int len) 
;;


let coup_interactif etat =
  let j, _, _ = etat in
  let coups = coups_possibles etat in
  if coups = [(1, 0)] then begin
    Printf.printf "%s : vous ne pouvez pas jouer. Passe.\n" noms_joueurs.(j);
    1, 0
  end else begin
    Printf.printf "%s : quel coup voulez-vous jouer (indice de ligne ou de colonne) ? " noms_joueurs.(j);
    match j with 
    |0 ->
      begin 
        let (y,_) = coup_aleatoire etat in
        let i = ref y in
        while not (1 <= !i && !i <= n - 2 && List.mem_assoc !i coups) do
          Printf.printf "Veuillez entrer un coup valide : ";
          let (z,_) = coup_aleatoire etat in
          i := z
        done;
      !i, List.assoc !i coups
    end;
    |_ -> begin
      let i = ref (lire_entier()) in
      while not (1 <= !i && !i <= n - 2 && List.mem_assoc !i coups) do
        Printf.printf "Veuillez entrer un coup valide : ";
        i := lire_entier()
      done;
    !i, List.assoc !i coups
  end
  end
;;




let _ = partie configuration_initiale coup_interactif coup_interactif true

;;

let min_max etat = 
  let j,posJ,posO = etat in 
  let coup_possible_pour_1 = coups_possibles etat in
  let coup_possible_pour_2 = coups_possibles ((1-j),posO,posJ) in 
  match j with 
  |0 -> 
    begin
      match coup_possible_pour_1 with 
      |(x,y)::xs -> 

      |_ -> 

let rec min_max1 l = 
  match l with 
  |(x,y)::xs -> 
    if y = n-1 then
      true && min_max1 xs 
    else
      false
  |_ -> 
    match terminal etat with 
    |Some joueur -> true
    |None -> false  
;;