.globl main

main:
    addi sp,sp,-12
    sw ra,0(sp)
    sw a0,4(sp)
    sw a1,8(sp)

    jal ra, trie
    
    li a7,10
    ecall
# Fonction de tri par insertion
trie:
    # Arguments : a0 = Adresse du tableau, a1 = Taille du tableau

    beqz a1, .end_trie  # Si la taille du tableau est 0, terminer

    # i = 1
    li t0, 1

.outer_loop:
    # Si i >= taille, on a parcouru tout le tableau
    bge t0, a1, .end_trie

    # Récupérer l'élément à insérer dans la partie triée
    lw t1, (a0)      # t1 = tableau[i]
    mv t2, t0        # t2 = i

    .inner_loop:
        # Si t2 <= 0 ou tableau[t2-1] <= t1, sortir de la boucle interne
        blez t2, .inner_end
        lw t3, -4(a0)      # t3 = tableau[t2-1]
        bgt t3, t1, .inner_end

        # Décaler le tableau vers la droite
        sw t3, 0(a0)
        li t5, 1
        sub t2, t2, t5
        j .inner_loop

    .inner_end:
        # Insérer l'élément à sa position correcte
        sw t1, 0(a0)

    # Incrémenter i
    addi t0, t0, 1
    addi a0, a0, 4   # Pointeur vers le prochain élément du tableau
    j .outer_loop

.end_trie:
    ret

