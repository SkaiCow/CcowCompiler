section .data
factorial dd 0
n dd 0
Lit1 dd 1
results dd 0
Lit5 dd 5
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
_start:
pmg1: nop
call PrintString
call GetAnInteger
mov [factorial],eax
mov ax,[Lit1]
mov [n], ax
mov ax,[Lit1]
mov [results], ax
W1: nop
mov ax,[n]
cmp ax,[Lit5]
JG L1
mov ax,[n]
mul byte[results]
mov [T1], ax
mov ax,[T1]
mov [results],ax
mov ax, [results]
call ConvertIntegerToString
mov eax, 4
mov ebx, 1
mov ecx, Result
mov edx, ResultEnd
int 80h
mov ax,[Lit1]
add ax,[n]
mov [T2],ax
mov ax,[T2]
mov [n],ax
jmp W1
L1: nop
call fini

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

GetAnInteger:
    mov eax,3
    mov edx,2
    mov ecx,num
    mov edx,6
    int 80h
ConvertStringToInteger:
    mov ax,0
    mov [ReadInt],ax
    mov ecx,num
    mov bx,0
    mov bl, byte[ecx]
Next: sub bl, '0'
    mov ax,[ReadInt]
    mov dx,10
    mul dx
    add ax,bx
    mov [ReadInt], ax
    mov bx,0
    add ecx,1
    mov bl, byte[ecx]
    cmp bl,0xA
    jne Next
    ret

ConvertIntegerToString:
    mov ebx, ResultValue + 4
ConvertLoop: sub dx, dx
    mov cx,10
    div cx
    add dl,'0'
    mov [ebx],dl
    dec ebx
    cmp ebx,ResultValue
    jge ConvertLoop
    ret

fini:
    mov eax, 1
    xor ebx,ebx
    int 80h