MOV r0,#1
MOV r1,#100
Loop:
ADD r2,r2,r0
ADD r0,r0,#1
CMP r0,r1
BNE Loop
ADD r3,r2,r1
SUB r4,r3,r2
MUL r6,r3,r4

