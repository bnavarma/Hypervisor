;*******************************************
;   stdio.inc
;       -Input/Output routines
;
;   Hypervisor Development
;*******************************************

%ifndef __STDIO_INC_INCLUDED__
%define __STDIO_INC_INCLUDED__

;===========================================
;
;   16 Bit Real Mode Routines
;===========================================

;*******************************************
;   Puts16()
;       -Prints a null terminated string
;   DS=>SI: 0 terminated string
;*******************************************

bits    16

Puts16:
        pusha
.Loop1:
        lodsb
        or  al, al
        jz  Puts16Done
        mov ah, 0eh
        int 10h
        jmp .Loop1
Puts16Done:
        popa
        ret

;===========================================
;
;   32 Bit Protected Mode Routines
;===========================================

bits    32

%define     VIDMEM  0xB8000
%define     COLS    80
%define     LINES   25
%define     CHAR_ATTRIB 14

_CurX   db 0
_CurY   db 0

;*******************************************
;   Putch32 ()
;       - Prints a character to screen
;   BL => Character to print
;*******************************************

Putch32:

    pusha
    mov edi, VIDMEM

    xor eax, eax

    ; y * screen width

    mov ecx, COLS*2
    mov al, byte [_CurY]
    mul ecx
    push   eax

    ; now add _CurX * 2

    mov al, byte [_CurX]
    mov cl, 2
    mul cl
    pop ecx
    add eax, ecx

    ; add the position to draw to the base of vid memory

    xor ecx, ecx
    add edi, eax

    ; watch for new line

    cmp bl, 0x0A
    je  .Row

    ; print the character

    mov dl, bl
    mov dh, CHAR_ATTRIB
    mov word [edi], dx

    ; go to next location

    inc byte [_CurX]
    jmp .done

.Row:
    mov byte[_CurX], 0
    inc byte[_CurY]

.done:
    popa
    ret

;******************************************************
;   Puts32 ()
;       - Prints a null terminated string
;   parm\ EBX = address of string to print
;******************************************************

Puts32:

    pusha
    push    ebx
    pop     edi

.loop:

    ;--------------------------------------
    ;   Get character
    ;--------------------------------------

    mov bl, byte[edi]
    cmp bl, 0
    je .done

    ;-------------------------------------
    ;   Print the character
    ;-------------------------------------
    call    Putch32

    ;-------------------------------------
    ;   Go to next character
    ;-------------------------------------
    inc     edi
    jmp     .loop

.done:

    ;-------------------------------------
    ;   Update hardware cursor
    ;-------------------------------------

    mov bh, byte[_CurY]
    mov bl, byte[_CurX]
    call    MovCur

    popa
    ret

;******************************************************
;   MovCur ()
;       - Update hardware cursor
;   parm/ bh = Y pos
;   parm/ bl = x pos
;******************************************************

bits    32

MovCur:

    pusha

    ;------------------------------------
    ;   Get current position
    ;------------------------------------

    ; location = _CurX + _CurY * COLS

    xor eax, eax
    mov ecx, COLS
    mov al, bh
    mul ecx
    add al, bl
    mov ebx, eax

    ;------------------------------------
    ;   Set low byte index to VGA register
    ;------------------------------------

    mov al, 0x0f
    mov dx, 0x03D4
    out dx, al

    ;--------------------------------------
    ;   Set high byte index to VGA register
    ;--------------------------------------

    xor eax, eax

    mov al, 0x0e
    mov dx, 0x03D4
    out dx, al

    mov al, bh
    mov dx, 0x03D5
    out dx, al

    popa
    ret

;**********************************************************
;   ClrScr32 ()
;       - Clears screen
;**********************************************************

bits 32

ClrScr32:

    pusha
    cld
    mov edi, VIDMEM
    mov cx, 2000
    mov ah, CHAR_ATTRIB
    mov al, ' '
    rep stosw
    mov byte [_CurX], 0
    mov byte [_CurY], 0
    popa
    ret

;*********************************************************
;   GotoXY ()
;       - Set current X/Y location
;   parm\   AL=X position
;   parm\   AH=Y position
;*********************************************************

bits 32

GotoXY:
    pusha
    mov [_CurX], al
    mov [_curY], ah
    popa
    ret

%endif ; __STDIO_INC_INCLUDED__