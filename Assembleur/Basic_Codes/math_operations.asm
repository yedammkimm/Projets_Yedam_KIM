
main:
li a7,5
ecall

mv a1,a0         #a1 = a
li a7,5		#a0 = b
ecall

jal ra, euclide
 
euclide: 
mv a3,a0

j b_moins_a


b_moins_a: 

addi sp,sp,-16
sw ra,0(sp)
sw a0,4(sp)
sw a1,8(sp)

sub a0,a1,a0

lw a1,8(sp)
lw ra,0(sp)
addi sp,sp,12
jalr x0,0(ra)

mv a1,a0

bgtz a0,euclide

j END

END: 
mv a1,a0
li a7,1
ecall