;***********************************************
;
;   Paging.inc
;       Basic paging for bootloader
;
;   Hypervisor Development
;***********************************************

%ifndef __PAGING_INC_INCLUDED__
%define __PAGING_INC_INCLUDED__

bits        32

%define     PAGE_DIR        0x9C000

%define     PAGE_TABLE_0    0x9D000

%define     PAGE_TABLE_768  0x9E000

%define     PAGE_TABLE_ENTRIES  1024

%define     PRIV    3

;*************************************************
;   Enable Paging
;*************************************************

EnablePaging:
    pusha

    ;-------------------------------------------------
    ;   identity map 1st page table (4MB)
    ;-------------------------------------------------

    mov     eax, PAGE_TABLE_0
    mov     ebx, 0x0 | PRIV
    mov     ecx, PAGE_TABLE_ENTRIES
.loop:
    mov     dword [eax], ebx
    add     eax, 4
    add     ebx, 4096
    loop    .loop

    ;--------------------------------------------------
    ;   set up the entries in the directory table
    ;--------------------------------------------------

    mov     eax, PAGE_TABLE_0 | PRIV
    mov     dword [PAGE_DIR], eax

    mov     eax, PAGE_TABLE_768 | PRIV
    mov     dword [PAGE_DIR + (768*4)], eax

    ;--------------------------------------------------
    ;   install directory table
    ;--------------------------------------------------

    mov     eax, PAGE_DIR
    mov     cr3, eax

    ;--------------------------------------------------
    ;   enable paging
    ;--------------------------------------------------

    mov     eax, cr0
    or      eax, 0x80000000
    mov     cr0, eax

    ;--------------------------------------------------
    ;   map the 768th table to physical addr 1MB
    ;   the 768th table starts the 3gb virtual address
    ;--------------------------------------------------

    mov     eax, PAGE_TABLE_768
    mov     ebx, 0x100000 | PRIV
    mov     ecx, PAGE_TABLE_ENTRIES
.loop2:
    mov     dword [eax], ebx
    add     eax, 4
    add     ebx, 4096
    loop    .loop2

    popa
    ret

%endif