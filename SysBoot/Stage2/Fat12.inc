;****************************************************
;
;       Fat12.inc
;           FAT12 filesystem for 3-1/2 floppies
;
;       Hypervisor Development Series
;****************************************************

%ifndef __FAT12_INC_INCLUDED__
%define __FAT12_INC_INCLUDED__

bits    16

%include "Floppy16.inc"

%define ROOT_OFFSET 0x2e00
%define FAT_SEG 0x2c0
%define ROOT_SEG 0x2e0

;*****************************************************
;   LoadRoot ()
;       - Load Root Directory Table to 0x7e00
;*****************************************************

LoadRoot:

    pusha
    push    es

    ; compute size of root directory and store in "cx"

    xor     cx, cx
    xor     dx, dx
    mov     ax, 32
    mul     WORD [bpbRootEntries]
    div     WORD [bpbBytesPerSector]
    xchg    ax, cx

    ; compute location of root directory and store in "ax"

    mov     al, BYTE[bpbNumberOfFATs]
    mul     WORD [bpbSectorsPerFAT]
    add     ax, WORD[bpbReservedSectors]
    mov     WORD [datasector], ax
    mov     WORD [datasector], cx

    ; read root directory into 0x7e00

    push    word ROOT_SEG
    pop     es
    mov     bx, 0
    call    ReadSectors
    pop     es
    popa
    ret

;*******************************************************
;   LoadFAT ()
;       - Loads FAT table to 0x7C00
;
;   Parm/ ES:DI => Root Directory Table
;*******************************************************

LoadFAT:

    pusha
    push    es

    ; compute size of FAT and store in "cx"

    xor     ax, ax
    mov     al, BYTE [bpbNumberOfFATs]
    mul     WORD [bpbSectorsPerFAT]
    mov     cx, ax

    ; compute location of FAT and store in "ax"

    mov     ax, WORD [bpbReservedSectors]

    ; read FAT into memory (Overwrite out bootloader at 0x7c00)

    push    word FAT_SEG
    pop     es
    xor     bx, bx
    call    ReadSectors
    pop     es
    popa
    ret

;*******************************************************
;   FindFile ()
;       - Search for filename in root table
;
;   parm/ DS:SI => File name
;   ret/ AX => File index number in directory table. -1 if error
;********************************************************

FindFile:

    push    cx
    push    dx
    push    bx
    mov     bx, si

    ; browse root directory for binary image

    mov     cx, WORD[bpbRootEntries]
    mov     di, ROOT_OFFSET
    cld

.LOOP:
    push    cx
    mov     cx, 11
    mov     si, bx
    push    di
    rep     cmpsb
    pop     di
    je      .Found
    pop     cx
    add     di, 32
    loop    .LOOP

.NotFound:
    pop     bx
    pop     dx
    pop     cx
    mov     ax, -1
    ret

.Found:
    pop     ax
    pop     bx
    pop     dx
    pop     cx
    ret

;******************************************************
;   LoadFile()
;       -Load file
;   parm/ ES:SI => File to load
;   parm/ EBX:BP => Buffer to load file to
;   ret/ AX => -1 on error, 0 on success
;******************************************************

LoadFile:

    xor     ecx, ecx
    push    ecx

.FIND_FILE:

    push    bx
    push    bp
    call    FindFile
    cmp     ax, -1
    jne     .LOAD_IMAGE_PRE
    pop     bp
    pop     bx
    pop     ecx
    mov     ax, -1
    ret

.LOAD_IMAGE_PRE:

    sub     edi, ROOT_OFFSET
    sub     eax, ROOT_OFFSET

    ; get starting cluster

    push    word ROOT_SEG
    pop     es
    mov     dx, WORD [es:di + 0x001A]
    mov     WORD[cluster], dx
    pop     bx
    pop     es
    push    bx
    push    es
    call    LoadFAT

.LOAD_IMAGE:

    ; load the cluster

    mov     ax, WORD [cluster]
    pop     es
    pop     bx
    call    ClusterLBA
    xor     cx, cx
    mov     cl, BYTE [bpbSectorsPerCluster]

    call    ReadSectors

    pop     ecx
    inc     ecx
    push    ecx

    push    bx
    push    es

    mov     ax, FAT_SEG
    mov     es, ax
    xor     bx, bx

    ; get next cluster

    mov     ax, WORD[cluster]
    mov     cx, ax
    mov     dx, ax
    shr     dx, 0x0001
    add     cx, dx

    mov     bx, 0
    add     bx, cx
    mov     dx, WORD [es:bx]
    test    ax, 0x0001
    jnz     .ODD_CLUSTER

.EVEN_CLUSTER:

    and     dx, 0000111111111111b
    jmp     .DONE

.ODD_CLUSTER:

    shr     dx, 0x0004

.DONE:

    mov     WORD [cluster], dx
    cmp     dx, 0x0ff0
    jb      .LOAD_IMAGE

.SUCCESS:
    pop     es
    pop     bx
    pop     ecx
    xor     ax, ax
    ret

%endif      ;__FAT12_INC_INCLUDED__