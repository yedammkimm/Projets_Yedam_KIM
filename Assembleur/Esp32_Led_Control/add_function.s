.globl add

.text
add: 

    addi sp,sp,-12
    sw ra,0(sp)
    sw a0,4(sp)
    sw a1,8(sp)

    add a0,a1,a0

    lw a1,8(sp)
    lw ra,0(sp)
    addi sp,sp,12
    jalr x0,0(ra)
