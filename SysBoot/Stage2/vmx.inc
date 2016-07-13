;*************************************************
;   vmx.inc
;       Basic VMX
;
;   Hypervisor Development
;*************************************************

%ifndef __VMX_INC_INCLUDED__
%define __VMX_INC_INCLUDED__

bits 64

Revision_Number dd 0
VMON_ADDRESS   dq 0x10000
VMCS_VM_ADDRESS   dq 0x11000

NoVMX db "*** FATAL: No VMX support ***", 0
ReturnMessage db "*** I'm back ***", 0

;*************************************************
;   CheckVMX
;*************************************************

CheckVMX:
    push rax
    push rcx
    mov eax, 1
    cpuid
    bt ecx, 5
    jc .VMX_SUPPORTED
    mov ebx, NoVMX
    call Puts64
    hlt
.VMX_SUPPORTED:
    xor eax, eax
    mov ecx, 0480h
    rdmsr
    mov [Revision_Number], eax
    pop rcx
    pop rax
    ret

;***************************************************
;   TurnVMXON
;***************************************************

TurnVMXON:
    push rax
    push rcx
    push rdx
    push rdi
    xor eax, eax
    mov ecx, 0480h
    rdmsr

    mov rax, cr4
    bts rax, 13
    mov cr4, rax

    xor rdi, rdi
    mov edi, [Revision_Number]
    xor rax, rax
    mov rax, [VMON_ADDRESS]
    mov [rax], edi
    mov rdi, VMON_ADDRESS

    mov rax, cr0
    bts rax, 5
    mov cr0, rax

    mov ecx, 03ah
    rdmsr
    test eax, 1
    or eax, 1
    mov rsi, rdi
    wrmsr
    VMXON [rdi]

    pop rdi
    pop rdx
    pop rcx
    pop rax
    ret

;***************************************************
;   PrepareVMLaunch
;       For more information on the offets and the
;       consequences please refer to INTEL manual
;       Volume 3 Chapter 24 and Appendix B
;***************************************************

PrepareVMLaunch:
    push rax
    push rbx
    push rcx
    push rdi

    xor rdi, rdi
    mov edi, [Revision_Number]
    xor rax, rax
    mov rax, [VMCS_VM_ADDRESS]
    mov [rax], edi
    mov rdi, VMCS_VM_ADDRESS
    xchg bx, bx
    vmclear [rdi]
    vmptrld [rdi]

    ; 16-bit control fields
    ;mov eax, 0
    ;xor rbx, rbx
    ;mov bx, 0x1
    ;vmwrite rax, rbx

    ; 16-bit Guest state fields
    xor rbx, rbx
    mov rax, 00800h
    vmwrite rax, rbx
    mov rax, 00802h
    vmwrite rax, rbx
    mov rax, 00804h
    vmwrite rax, rbx
    mov rax, 00806h
    vmwrite rax, rbx
    mov rax, 00808h
    vmwrite rax, rbx
    mov rax, 0080Ah
    vmwrite rax, rbx
    mov rax, 0080Ch
    vmwrite rax, rbx
    mov rax, 0080Eh
    vmwrite rax, rbx
    mov rax, 0080Eh
    vmwrite rax, rbx

    ; 16-Bit Host state fields
    mov rax, 00C00h
    mov bx, es
    vmwrite rax, rbx
    mov rax, 00C02h
    mov bx, cs
    vmwrite rax, rbx
    mov rax, 00C04h
    mov bx, ss
    vmwrite rax, rbx
    mov rax, 00C06h
    mov bx, ds
    vmwrite rax, rbx
    mov rax, 00C08h
    mov bx, fs
    vmwrite rax, rbx
    mov rax, 00C0Ah
    mov bx, gs
    vmwrite rax, rbx
    mov rax, 00C0Ch
    mov bx, 70h
    vmwrite rax, rbx

    ; 64-Bit Control fields

    ; EPT Pointer
    mov rax, 00201Ah
    xor rbx, rbx
    add rbx, 0x14000
    or rbx, 0
    or rbx, 0x18
    vmwrite rax, rbx

    ; 64-Bit Guest state fields
    mov rax, 002800h
    mov rbx, 0xFFFFFFFFFFFFFFFF
    vmwrite rax, rbx

    ; 64-Bit Host state fields

    ; EFER number
    mov ecx, 0c0000080h
    rdmsr
    mov rbx, 002C02h
    vmwrite rbx, rax

    ; 32-Bit Control fields

    mov rax, 004000h
    mov rbx, 0x1f
    vmwrite rax, rbx
    mov rax, 004002h
    mov rbx, 0x8401e9f2
    vmwrite rax, rbx
    mov rax, 00400Ah
    mov rbx, 0x4
    vmwrite rax, rbx
    mov rax, 00400Ch
    mov rbx, 0x36fff
    vmwrite rax, rbx
    mov rax, 00400Eh
    mov rbx, 0
    vmwrite rax, rbx
    mov rax, 004010h
    vmwrite rax, rbx
    mov rax, 004012h
    mov rbx, 0x11ff
    vmwrite rax, rbx
    mov rax, 004014h
    mov rbx, 0
    vmwrite rax, rbx
    mov rax, 004016h
    vmwrite rax, rbx
    mov rax, 00401Eh
    mov rbx, 0x82
    vmwrite rax, rbx

    ; 32-Bit Guest state fields
    mov rbx, 0xffff
    mov rax, 004800h
    vmwrite rax, rbx
    mov rax, 004802h
    vmwrite rax, rbx
    mov rax, 004804h
    vmwrite rax, rbx
    mov rax, 004806h
    vmwrite rax, rbx
    mov rax, 004808h
    vmwrite rax, rbx
    mov rax, 00480Ah
    vmwrite rax, rbx
    mov rax, 00480Ch
    vmwrite rax, rbx
    mov rax, 00480Eh
    vmwrite rax, rbx
    mov rax, 004810h
    vmwrite rax ,rbx
    mov rax, 004812h
    vmwrite rax, rbx

    mov rbx, 09fh
    mov rax, 004816h
    vmwrite rax, rbx

    mov rbx, 093h
    mov rax, 004814h
    vmwrite rax, rbx
    mov rax, 004818h
    vmwrite rax, rbx
    mov rax, 00481Ah
    vmwrite rax, rbx
    mov rax, 00481Ch
    vmwrite rax, rbx
    mov rax, 00481Eh
    vmwrite rax, rbx

    mov rbx, 010000h
    mov rax, 004820h
    vmwrite rax, rbx

    mov rbx, 08bh
    mov rax, 004822h
    vmwrite rax, rbx

    xor rbx, rbx
    mov rax, 004824h
    vmwrite rax, rbx
    mov rax, 004826h
    vmwrite rax, rbx

    ; Natural-width Guest state fields

    mov rbx, 0060000030h
    mov rax, 006800h
    vmwrite rax, rbx

    xor rbx, rbx
    mov rax, 006802h
    vmwrite rax, rbx
    mov rax, 006804h
    bts rbx, 13
    vmwrite rax, rbx
    xor rbx, rbx
    mov rax, 006806h
    vmwrite rax, rbx
    mov rax, 006808h
    vmwrite rax, rbx
    mov rax, 00680Ah
    vmwrite rax, rbx
    mov rax, 00680Ch
    vmwrite rax, rbx
    mov rax, 00680Eh
    vmwrite rax, rbx
    mov rax, 006810h
    vmwrite rax, rbx
    mov rax, 006812h
    vmwrite rax, rbx
    mov rax, 006814h
    vmwrite rax, rbx
    mov rax, 006816h
    vmwrite rax, rbx
    mov rax, 006818h
    vmwrite rax, rbx

    mov rbx, 0x400
    mov rax, 00681Ah
    vmwrite rax, rbx

    mov rbx, GuestVMEntry
    mov rax, 00681Eh
    vmwrite rax, rbx

    mov rbx, 2
    mov rax, 006820h
    vmwrite rax, rbx

    xor rbx, rbx
    mov rax, 006822h
    vmwrite rax, rbx
    mov rax, 006824h
    vmwrite rax, rbx
    mov rax, 006826h
    vmwrite rax, rbx

    ; Natural-width Host state fields

    mov rbx, cr0
    mov rax, 006C00h
    vmwrite rax, rbx

    mov rbx, cr3
    mov rax, 006C02h
    vmwrite rax, rbx

    mov rbx, cr4
    mov rax, 006C04h
    vmwrite rax, rbx

    mov rbx, 0
    mov rax, 006C06h
    vmwrite rax, rbx
    mov rax, 006C08h
    vmwrite rax, rbx
    mov rax, 006C10h
    vmwrite rax, rbx
    mov rax, 006C12h
    vmwrite rax, rbx
    mov rax, 006C0Ah
    vmwrite rax, rbx

    mov rbx, GDT64.Pointer
    mov rax, 006C0Ch
    vmwrite rax, rbx

    mov rbx, 0
    mov rax, 006C0Eh
    vmwrite rax, rbx

    mov rbx, VMEXIT_RETURN
    mov rax, 006C16h
    vmwrite rax, rbx

    pop rdi
    pop rcx
    pop rbx
    pop rax
    ret

VMEXIT_RETURN:
    push rbx
    mov rbx, ReturnMessage
    call Puts64
    pop rbx
    hlt

%endif