MOV r0,#1
MOV r1,#5
Loop:
ADD r2,r2,r0
ADD r0,r0,#1
CMP r0,r1
ADD r3,r2,r2
SUB r4,r3,r2
SUB r5,r4,r1
LDR r6,[r5]
MOV r7,r6
BNE Loop
ADD r7,r7,r1
SUB r8,r7,r5
