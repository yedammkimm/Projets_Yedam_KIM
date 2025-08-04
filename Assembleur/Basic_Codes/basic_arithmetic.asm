


addi s0, x0, 10 # s0 = N = 10, le nombre de termes de la série de Fibonacci à calculer
addi t0, x0, 0 # t0 = Terme actuel de Fibonacci initialisé à 0 (F0)
addi t1, x0, 1 # t1 = Terme suivant de Fibonacci initialisé à 1 (F1)
addi t2, x0, 1 # t2 = Compteur de boucle initialisé à 1, car F0 est déjà calculé

# Vérifier si N est 0; si oui, sauter à la fin
beq s0, x0, end
 mv a0,t0
 li a7,1
 ecall
 
 li a0,32
 li a7,11
 ecall
# Afficher F0 (premier terme) et un espace
# COMLETER : afficher le premier terme suivi d’un espace
#On utilise a0 et a7 puis ecall(l'appel systeme) et 1 permet d'afficher un entier, 11 un espace
#Donc, a0: Valeur, a7:Code


# Vérifier si N est 1; si oui, sauter à la fin
li t3, 1
beq s0, t3, end

fib_loop: 
 #Boucle de Calcul des termes 
 mv t3,t1
 add t1,t0,t1 
 mv t0,t3
 addi s0,s0,-1
 mv a0,t0
 li a7,1
 ecall
 
 li a0,32
 li a7,11
 ecall
 bnez s0,fib_loop
 
 
end:
# Arrêter le programme
li a7, 10 # Code pour l'appel système "exit"
ecall # Exécuter l'appel système