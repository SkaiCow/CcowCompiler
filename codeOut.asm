section .data
a dd 0
Bob dd 0
Jane dd 0
b dd 0
c dd 0
Lit20 dd 20
Lit26 dd 26
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

section .text
Pmg1: nop
mov ax,[Lit20]
mov [Bob],ax
mov ax,[Lit26]
mov [Jane],ax
call PrintString
call GetAnInteger
mov [a],eax
call PrintString
call GetAnInteger
mov [b],eax
call PrintString
call GetAnInteger
mov [c],eax
mov ax,[Jane]
add ax,[Bob]
mov [T1],ax
mov ax,[T1]
sub ax,[Lit10]
mov [T2], ax
mov dx, 0
mov ax, [T2]
mov bx, [Lit2]
div bx
mov [T1], ax
mov ax,[Lit4]
mul byte[T1]
mov [T2], ax
mov ax,[T2]
mul byte[a]
mov [T1], ax
mov ax,[c]
add ax,[b]
mov [T3],ax
mov dx, 0
mov ax, [T1]
mov bx, [T3]
div bx
mov [T4], ax
mov ax,[T4]
mov [ans], ax
mov ax, [ans]
call ConvertIntegerToString
mov eax, 4
mov ebx, 1
mov ecx, Result
mov edx, ResultEnd
int 80h
call fini
