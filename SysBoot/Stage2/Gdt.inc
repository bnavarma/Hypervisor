;********************************************
;   Gdt.inc
;       -GDT Routines
;
;   Hypervisor Development
;********************************************

%ifndef __GDT_INC_INCLUDED__
%define __GDT_INC_INCLUDED__

bits        16

;********************************************
;   InstallGDT()
;       - Install out GDT
;********************************************

InstallGDT:

    cli
    pusha
    lgdt    [toc]
    sti
    popa
    ret

;********************************************
;   Global Descriptor Table (GDT)
;********************************************

gdt_data:
    dd 0
    dd 0

; gdt code:
    dw 0FFFFh
    dw 0
    dw 0
    db 10011010b
    db 11001111b
    db 0

; gdt data:
    dw 0FFFFh
    dw 0
    dw 0
    db 10010010b
    db 11001111b
    db 0

end_of_gdt:
toc:
    dw end_of_gdt - gdt_data -1
    dd gdt_data

; give the descriptor offsets names

%define NULL_DESC 0
%define CODE_DESC 0x8
%define DATA_DESC 0x10

%endif ; __GDT_INC_INCLUDED__