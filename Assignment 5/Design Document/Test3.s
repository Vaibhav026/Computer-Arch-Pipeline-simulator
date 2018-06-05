MOV r1,#1200
MOV r0,#4
STR r0,[r1,#20]
LDR r2,[r1,#20]
ADD r4,r2,r0
SUB r8,r2,r1
ADD r9,r4,r2
