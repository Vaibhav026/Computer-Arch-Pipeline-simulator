MOV r0,#1
MOV r1,#4
Loop:
ADD r2,r2,#4
ADD r0,r0,#1
ADD r3,r2,#1200
SUB r4,r3,r2
SUB r5,r4,r1
LDR r6,[r5]
MOV r7,r6
CMP r0,r1
BNE Loop
ADD r7,r7,r1
SUB r8,r7,r5
BL Loopp
ADD r5,r7,r8
ADD r9,r5,r5
Loopp:
ADD r10,r10,#1
SUB r10,r10,#1
SUB r11,r10,r13
CMP r10,r11
BNE Loopp
ADD r12,r10,r11
