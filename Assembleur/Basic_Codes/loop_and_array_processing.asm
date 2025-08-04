
main:
# Appel de la fonction pour ajouter 5 et 3
li a7,5
ecall

mv a3,a0         #a3 = a
li a7,5
ecall

mv a2,a0         #a2 = b
li a7,5
ecall

mv a1,a0         #a1 = c
li a7,5		 #a0 = d
ecall

jal ra, Add # Appeler la fonction add
jal ra, Sub
jal ra, Mul
j out 

Add: 

addi sp,sp,-16
sw ra,0(sp)
sw a2,4(sp)
sw a3,8(sp)

add a2,a2,a3

lw a3,8(sp)
lw ra,0(sp)
addi sp,sp,12
jalr x0,0(ra)

Sub: 

addi sp,sp,-16
sw ra,0(sp)
sw a0,4(sp)
sw a1,8(sp)

sub a0,a1,a0

lw a1,8(sp)
lw ra,0(sp)
addi sp,sp,12
jalr x0,0(ra)

Mul: 

addi sp,sp,-16
sw ra,0(sp)
sw a0,4(sp)
sw a2,8(sp)

mul a0,a2,a0

lw a2,8(sp)
lw ra,0(sp)
addi sp,sp,12
jalr x0,0(ra)

out: 
li a7,1
ecall
li a7,10
ecall







