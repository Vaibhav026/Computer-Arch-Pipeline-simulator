mov,cmp,add,sub,mul commands all versions work,
for add,cmp,sub work like this:
add r1,r2,#2
sub r1,r2,r3
But spaces after # are not allowed
For labels one limitation is that there should not be any other command on the same line of that of label.
ELSE: (works)
ELSE: add r1,r2  (will work but will ignore add)

It allows written things which does not mean anything b/w two commands but will simply ignore it still show error in that line,nothing to worry about as nothing other gets affected.
For eg:
abc c cmn sm (simulator will ignore this and move to the next instruction)

Ldr and str also work
ldr r1,[r0]
ldr r1,[r0,#12] but no space after 2 in this example is allowed
ldr r1,[r0],#12
ldr r1,=ABC

where ABC is starting  memory address

ALL Branch statements work fine where r[14] represents link register and r[15] pc.   

All errors are dealt line by line and most errors will also show reason behind the error like if I write s instead of r for register it will display an error.

