.data
pmg1 dd ?
factorial dd ?
n dd ?
results dd ?
Lit1 dd 1
T1 dd ?
T2 dd ?
T3 dd ?
T4 dd ?
T5 dd ?
T6 dd ?

.code
pmg1: nop
mov ax,[5]
mov [factorial], ax
mov ax,[1]
mov [n], ax
mov ax,[1]
mov [results], ax
W1: nop
mov ax,[5]
cmp ax,[n]
JG L1
mov ax,[n]
mul [results]
mov [T1], ax
mov ax,[T1]
mov [results],ax
move ax,[1]
add ax,[n]
move [T2],ax
mov ax,[T2]
mov [n],ax
jmp W1
L1: nop
