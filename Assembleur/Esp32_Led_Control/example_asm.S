# Exercice 0
# Faire une addition dans un registre
.globl increment

.text

increment:
    addi sp, sp, -4 # Allouer 4 octets sur la pile
    sw ra, 0(sp) # Sauvegarder l'adresse de retour à sp
    # Corps de la fonction

    addi a0, a0, 1 # Effectuer l'incrementation

    # Épilogue (dépiler)
    lw ra, 0(sp) # Restaurer l'adresse de retour
    addi sp, sp, 4 # Rétablir le pointeur de pile
    jalr x0, 0(ra) # Retourner à l'adresse de retour
