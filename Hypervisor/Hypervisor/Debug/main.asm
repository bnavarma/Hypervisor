; Listing generated by Microsoft (R) Optimizing Compiler Version 19.00.23918.0 

	TITLE	c:\Users\varmaav\documents\visual studio 2015\Projects\Hypervisor\Hypervisor\main.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRT
INCLUDELIB OLDNAMES

PUBLIC	_main
EXTRN	?DebugClrScr@@YAXG@Z:PROC			; DebugClrScr
; Function compile flags: /Odtp
; File c:\users\varmaav\documents\visual studio 2015\projects\hypervisor\hypervisor\main.cpp
_TEXT	SEGMENT
_i$ = -4						; size = 4
_main	PROC

; 10   : void __cdecl main() {

	push	ebp
	mov	ebp, esp
	push	ecx

; 11   :     int i = 0x12;

	mov	DWORD PTR _i$[ebp], 18			; 00000012H

; 12   : 
; 13   :     DebugClrScr(0x18);

	push	24					; 00000018H
	call	?DebugClrScr@@YAXG@Z			; DebugClrScr
	add	esp, 4

; 14   : }

	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	0
_main	ENDP
_TEXT	ENDS
END
