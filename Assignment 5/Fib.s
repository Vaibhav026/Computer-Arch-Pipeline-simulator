ADD r0,r0,#0
ADD r1,r1,#1
loop:
add r2,r1,r0
mov r0,r1
MOV r1,r2
ADD r3,r3,#1
CMP r3,#5
BNE loop
ADD r10,r10,r10
MUL r11,r1,r2