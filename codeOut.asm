section .data
a dd 0
Bob dd 0
Jane dd 0
b dd 0
c dd 0
ans dd 0
Lit10 dd 10
Lit2 dd 2
Lit4 dd 4
T1 dd 0
T2 dd 0
T3 dd 0
T4 dd 0
T5 dd 0
T6 dd 0
section .bss
global _start

section .text

_start:
move ax,[Jane]
add ax,[Bob]
move [T1],ax
mov ax,[Lit10]
sub ax,[T1]
mov [T2], ax
mov dx, 0
mov ax, [T2]
mov bx, [Lit2]
div bx
mov [T1], ax
mov ax,[Lit4]
mul [T1]
mov [T2], ax
mov ax,[T2]
mul [a]
mov [T1], ax
move ax,[c]
add ax,[b]
move [T3],ax
mov dx, 0
mov ax, [T1]
mov bx, [T3]
div bx
mov [T4], ax
mov ax,[T4]
mov [ans], ax
