
;*******************************************************
;
;	Paging.inc
;		Basic paging for bootloader
;
;	OS Development Series
;*******************************************************

; Added sence Tutorial 17

%ifndef __PAGING_INC_67343546FDCC56AAB872_INCLUDED__
%define __PAGING_INC_67343546FDCC56AAB872_INCLUDED__

bits	32

;****************************************
;	Enable Paging
;****************************************

EnablePaging:
	pushad										; save stack frame

	mov edi, 0x4000
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	xchg bx, bx
	mov edi, cr3

	mov DWORD[edi], 0x5003
	add edi, 0x1000
	mov DWORD[edi], 0x6003
	add edi, 0x1000
	mov DWORD[edi], 0x7003
	add edi, 0x1000

	mov ebx, 0x00000003
	mov ecx, 512

.SetEntry:
	mov DWORD[edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop .SetEntry

	popad
	ret

;*****************************************
;	Enabling EPT
;*****************************************

Bits 64

EnableEPT:
	push rax
	push rbx
	push rcx
	push rdi
	push rsi

	mov eax, 080000008
	cpuid
	and rax, 0xFF
	mov rbx, rax

	xor rdi, rdi
	add rdi, 0014000h

	xor rsi, rsi
	add rsi, 0015000h

	; Clear everything
	push rdi
	xor rax, rax
	mov ecx, 2048
	rep stosq 
	pop rdi

	xor rax, rax
	mov rax, rsi
	shl rax, 12
	shr rax, 12
	or rax, 7
	mov [rdi], rax

	xor rax, rax
	mov rax, 0016000h
	shl rax, 12
	shr rax, 12
	or rax, 7
	mov [rsi], rax

	mov rsi, 0016000h
	xor rax, rax
	shl rax, 30
	shr rax, 12
	or rax, 7
	bts rax, 7
	mov [rsi], rax

	pop rsi
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret

bits 32

%endif
