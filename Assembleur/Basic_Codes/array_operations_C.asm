.data 
array: .word 8,3,7,1,0,9,2,6,4,5,10

emptyArray: .space 44

.text 
li s0,10
la s1,array
addi t1,s0,1
la s2,array
lw t3,0(s2)
mv t4,t3			#Plus Grand
mv t5,t3 			#Plus Petit
addi s2,s2,4


loop_small_big:
lw t3,0(s2)

j min

max: 

ble t3,t4,ELSE
mv t4,t3

j fin_loop

min: 
ble t5,t3,ELSE_1
mv t5,t3

j max

ELSE_1:
j max

ELSE: 
j fin_loop

fin_loop: 

addi t1,t1,-1
addi s2,s2,4
bnez t1,loop_small_big
j END

END: 
mv a0,t5
li a7,1
ecall

li a0,32
li a7,11
ecall

mv a0,t4
li a7,1
ecall





