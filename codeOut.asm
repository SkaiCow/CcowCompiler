section .data
number dd 0
results dd 0
Lit0 dd 0
Lit2 dd 2
Lit3 dd 3
Lit4 dd 4
Lit1 dd 1
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
main: nop
call PrintString
call GetAnInteger
mov [number],eax
mov ax,[Lit0]
mov [results],ax
mov ax,[Lit2]
cmp ax,[number]
JLE L1
mov ax,[Lit3]
cmp ax,[number]
JLE L2
mov ax,[Lit4]
cmp ax,[number]
JL L3
mov ax,[Lit1]
mov [results],ax
L3: nop
L2: nop
L1: nop
mov ax, [results]
call ConvertIntegerToString
mov eax, 4
mov ebx, 1
mov ecx, Result
mov edx, ResultEnd
int 80h
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