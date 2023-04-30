sys_exit equ 1

section .data
a dd 1
Bob dd 1
Jane dd 1
b dd 1
c dd 1
ans dd 1
Lit10 dd 10
Lit2 dd 2
Lit4 dd 4
T1 dd 0
T2 dd 0
T3 dd 0
T4 dd 0
T5 dd 0
T6 dd 0
userMsg db 'Enter An integer: '
    lenUserMsg equ $-userMsg
    displayMsg db 'You entered: '
    lenDisplayMsg equ $-displayMsg
    newline db 0xA

    Ten dw 10

    printTempchar db 'Tempchar = : '
    lenprintTempchar equ $-printTempchar

    Result db 'Ans = '
    ResultValue db 'aaaaa'
    db 0xA
    ResultEnd equ $-Result

    num times 6 db 'ABCDEF'
    numEnd equ $-num
section .bss

    Tempchar RESB 1
    testchar RESB 1
    ReadInt RESW 1
    teampint RESW 1
    negflag RESB 1

global _start
section .text
_start: nop
mov ax,[Jane]
add ax,[Bob]
mov [T1],ax
mov ax,[Lit10]
sub ax,[T1]
mov [T2], ax
mov dx, 0
mov ax, [T2]
mov bx, [Lit2]
div bx
mov [T1], ax
mov ax,[Lit4]
mul word[T1]
mov [T2], ax
mov ax,[T2]
mul word[a]
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
call PrintString
call GetAnInteger
call fini

GetAnInteger:
    mov eax, 3
    mov ebx, 2
    mov ecx, num
    mov edx, 6
    int 0x80
    mov edx, eax
    mov eax, 4
    mov ebx, 1
    mov ecx, num
    int 80h

PrintString:
    push ax
    push dx
    mov eax, 4
    mov ebx, 1
    mov ecx, userMsg
    mov edx, lenUserMsg
    int 80h
    pop dx
    pop ax
    ret

fini:
    mov eax, sys_exit
    xor ebx,ebx
    int 80h
