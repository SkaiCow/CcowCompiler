section .data
M dd 0
N dd 0
Y dd 0
X dd 0
Lit2 dd 2
Lit5 dd 5
Lit18 dd 18
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
global Pgm1
section .text
Pgm1: nop
mov ax,[Lit2]
mov [N],ax
mov ax,[Lit5]
mov [Y],ax
call GetAnInteger
mov [M],eax
mov ax,[N]
mul byte[M]
mov [T1], ax
mov ax,[Lit18]
add ax,[T1]
mov [T2],ax
mov ax,[T2]
sub ax,[Y]
mov [T1], ax
mov ax,[T1]
mov [X],ax
mov ax, [X]
call ConvertIntegerToString
mov ax, [X]
mov eax, 4
mov ebx, 1
mov ecx, Result
mov ecx, ResultEnd
int 80h
call GetAnInteger
mov [X],eax

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