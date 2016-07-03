;*******************************************************
;   Memory.inc
;       -Basic memory routines
;
;   Hypervisor Development
;*******************************************************

%ifndef __MEMORY_INC_INCLUDED__
%define __MEMORY_INC_INCLUDED__

bits16

;----------------------------------------------
;   Memory map entry structure
;----------------------------------------------

struc   MemoryMapEntry
    .baseAddress        resq    1
    .length             resq    1
    .type               resd    1
    .acpi_null          resd    1
endstruc

;----------------------------------------------
;   Get memory map from bios
;   /in es:di->destination buffer for entries
;   /ret bp=entry count
;----------------------------------------------

BiosGetMemoryMap:

    xor     ebx, ebx
    xor     bp, bp
    mov     edx, 0x0534D4150
    mov     eax, 0xe820
    mov     [es:di + 20], dword 1
    mov     ecx, 24
    int     0x15
    jc      short .failed
    mov     edx, 0x0534D4150
    cmp     eax, edx
    jne     short .failed
    test    ebx, ebx
    je      short .failed
    jmp     short .jmpin
.e820lp:
    mov     eax, 0xe820
    mov     [es:di + 20], dword 1
    mov     ecx, 24
    int     0x15
    jc      short .e820f
    mov     edx, 0x0534D4150
.jmpin:
    jcxz    .skipent
    cmp     cl, 20
    jbe     short .notext
    test    byte [es:di + 20], 1
    je      short .skipent
.notext:
    mov     ecx, [es:di + 8]
    test    ecx, ecx
    jne     short .goodent
    mov     ecx, [es:di + 12]
    jecxz   .skipent
.goodent:
    inc     bp
    add     di, 24
.skipent:
    test    ebx, ebx
    jne     short .e820lp
.e820f:
    ret
.failed:
    stc
    ret

;--------------------------------------------------
;   Get memory size for >64M configurations (32 bit)
;   ret\ ax=KB between 1MB and 16MB
;   ret\ bx=number of 64K blocks above 16MB
;   ret\ bx=0 and ax=-1 on error
;--------------------------------------------------

BiosGetMemorySize64MB_32bit:
    push    ecx
    push    edx
    xor     ecx, ecx
    xor     edx, edx
    mov     eax, 0xe881
    int     0x15
    jc      .error
    jcxz    .use_ax
    mov     eax, ecx
    mov     ebx, edx

.use_ax:
    pop     edx
    pop     ecx
    ret

.error:
    mov     ax, -1
    mov     bx, 0
    pop     edx
    pop     ecx
    ret

;---------------------------------------------------
;   Get memory size for >64M configurations
;   ret\ ax=KB between 1MB and 16MB
;   ret\ bx=number of 64K blocks above 16MB
;   ret\ bx=0 and ax= -1 on error
;---------------------------------------------------

BiosGetMemorySize64MB:
    push    ecx
    push    edx
    xor     ecx, ecx
    xor     edx, edx
    mov     ax, 0xe801
    int     0x15
    jc      .error
    cmp     ah, 0x86
    je      .error
    cmp     ah, 0x80
    je      .error
    jcxz    .use_ax
    mov     ax, cx
    mov     bx, dx

.use_ax:
    pop     edx
    pop     ecx
    ret

.error:
    mov     ax, -1
    mov     bx, 0
    pop     edx
    pop     ecx
    ret

;--------------------------------------------------
;   Get amount of contigous KB from addr 0
;   ret\ ax=KB size from address 0
;--------------------------------------------------

BiosGetMemorySize:
    int 0x12
    ret

;---------------------------------------------------
;   Get contigous extended memory size
;   ret\ ax=KB size above 1MB; ax= -1 on error
;---------------------------------------------------

BiosGetExtendedMemorySize:
    mov     ax, 0x88
    int     0x15
    jc      .error
    test    ax, ax
    je      .error
    cmp     ah, 0x86
    je      .error
    cmp     ah, 0x80
    je      .error
    ret
.error:
    mov     ax, -1
    ret

%endif