.data 
array: .word 8,3,7,1,0,9,2,6,4,5,10

.text 
li s0,10
la s1,array
addi t1,s0,1

loop: 
lw a0,0(s1)  #load word: comme *
li a7,1
ecall

li a0,32
li a7,11
ecall
 
addi s1,s1,4
addi t1,t1,-1
bnez t1,loop

 
end:  

 
