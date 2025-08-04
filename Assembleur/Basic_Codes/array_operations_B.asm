.data 
array: .word 8,3,7,1,0,9,2,6,4,5,10

emptyArray: .space 44

.text 
li s0,10
la s1,array
addi t1,s0,1
la s2,emptyArray


loop: 
lw t2,0(s1)
mul a0,t2,t2
sw a0,0(s2)     #Store word dans 0(s2) de la valeur a0
li a7,1
ecall

li a0,32
li a7,11
ecall
 
addi s1,s1,4
addi s2,s2,4
addi t1,t1,-1
bnez t1,loop

