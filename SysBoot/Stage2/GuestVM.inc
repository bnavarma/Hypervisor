;*****************************************************
;       Guest VM code
;
;       Hypervisor Development
;*****************************************************

%ifndef __GUEST_VM_INCLUDED__
%define __GUEST_VM_INCLUDED__

%include "stdio.inc"

bits 16

GuestMessage db "VM: Sooner or later you'll realize just as I did that there's a difference between knowing the path and walking the path.", 0

GuestVMEntry:
    mov si, GuestMessage
    call Puts16
    vmcall

%endif