
;*******************************************************
;
;	Stage2.asm
;		Stage2 Bootloader
;
;	OS Development Series
;*******************************************************

bits	16

org 0x500

jmp	main				; go to start

;*******************************************************
;	Preprocessor directives
;*******************************************************

%include "stdio.inc"			; basic i/o routines
%include "Gdt.inc"			; Gdt routines
%include "A20.inc"			; A20 enabling
%include "Fat12.inc"			; FAT12 driver. Kinda :)
%include "Common.inc"
%include "bootinfo.inc"
%include "memory.inc"
%include "GuestVM.inc"

;*******************************************************
;	Data Section
;*******************************************************

LoadingMsg db 0x0D, 0x0A, "Searching for Operating System...", 0x00
msgFailure db 0x0D, 0x0A, "*** FATAL: Missing or corrupt KRNL32.EXE. Press Any Key to Reboot.", 0x0D, 0x0A, 0x0A, 0x00

BadImage db "*** FATAL: Invalid or corrupt kernel image. Halting system.", 0
NoCPUIDMsg db "*** FATAL: no cpuid ***", 0
NoLongModeMsg db "*** FATAL: no long mode ***", 0
EnteredRealMode db "*** Entered The Dragon (long mode) ***", 0x0A, 0

boot_info:
istruc multiboot_info
	at multiboot_info.flags,			dd 0
	at multiboot_info.memoryLo,			dd 0
	at multiboot_info.memoryHi,			dd 0
	at multiboot_info.bootDevice,			dd 0
	at multiboot_info.cmdLine,			dd 0
	at multiboot_info.mods_count,			dd 0
	at multiboot_info.mods_addr,			dd 0
	at multiboot_info.syms0,			dd 0
	at multiboot_info.syms1,			dd 0
	at multiboot_info.syms2,			dd 0
	at multiboot_info.mmap_length,			dd 0
	at multiboot_info.mmap_addr,			dd 0
	at multiboot_info.drives_length,		dd 0
	at multiboot_info.drives_addr,			dd 0
	at multiboot_info.config_table,			dd 0
	at multiboot_info.bootloader_name,		dd 0
	at multiboot_info.apm_table,			dd 0
	at multiboot_info.vbe_control_info,		dd 0
	at multiboot_info.vbe_mode_info,		dw 0
	at multiboot_info.vbe_interface_seg,		dw 0
	at multiboot_info.vbe_interface_off,		dw 0
	at multiboot_info.vbe_interface_len,		dw 0
iend

GDT64:
	.Null: equ $ - GDT64
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0
	.Code: equ $ - GDT64
	dw 0
	dw 0
	db 0
	db 10011010b
	db 00100000b
	db 0
	.Data: equ $ - GDT64
	dw 0
	dw 0
	db 0
	db 10010010b
	db 00000000b
	db 0
	.Pointer:
	dw $ - GDT64 - 1
	dq GDT64

main:

	;-------------------------------;
	;   Setup segments and stack	;
	;-------------------------------;

	cli	                   ; clear interrupts
	xor		ax, ax             ; null segments
	mov		ds, ax
	mov		es, ax
	mov		ax, 0x0000         ; stack begins at 0x9000-0xffff
	mov		ss, ax
	mov		sp, 0xFFFF
	sti	                   ; enable interrupts

	mov     	[boot_info+multiboot_info.bootDevice], dl

	call		_EnableA20
	call		InstallGDT
	sti

	xor		eax, eax
	xor		ebx, ebx
	call		BiosGetMemorySize64MB

	push		eax
	mov		eax, 64
	mul		ebx
	mov		ecx, eax
	pop		eax
	add		eax, ecx
	add		eax, 1024		; the routine doesnt add the KB between 0-1MB; add it

	mov		dword [boot_info+multiboot_info.memoryHi], 0
	mov		dword [boot_info+multiboot_info.memoryLo], eax

	mov		eax, 0x0
	mov		ds, ax
	mov		di, 0xC000
	call		BiosGetMemoryMap

	call		LoadRoot
   	mov    		ebx, 0
   	mov		ebp, IMAGE_RMODE_BASE
   	mov 	   	esi, ImageName
	call		LoadFile		; load our file
   	mov   		dword [ImageSize], ecx
	cmp		ax, 0
	je		EnterStage3
	mov		si, msgFailure
	call   		Puts16
	mov		ah, 0
;	int     	0x16                    ; await keypress
;	int     	0x19                    ; warm boot computer

	;-------------------------------;
	;   Go into pmode               ;
	;-------------------------------;

EnterStage3:

	cli	                           ; clear interrupts
	mov	eax, cr0                   ; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax
	xchg bx, bx 
	jmp	CODE_DESC:Stage3                ; far jump to fix CS. Remember that the code selector is 0x8!

	; Note: Do NOT re-enable interrupts! Doing so will triple fault!
	; We will fix this in Stage 3.

;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************

bits 32

%include "Paging.inc"

CheckCPUID:
	pushad
	;-------------------------------
	;	Check if CPUID exists
	;-------------------------------
	pushfd
	pop eax

	mov eax, eax

	xor eax, 1 << 21

	push eax
	popfd

	pushfd
	pop eax

	push ecx
	popfd

	xor eax, ecx
	jz .NoCPUID
	popad
	ret
.NoCPUID:
	mov ebx, NoCPUIDMsg
	call Puts32
	hlt

CheckLongMode:
	pushad
	mov		eax, 0x80000000
	cpuid
	cmp		eax, 0x80000001
	jb	.NoLongMode
	mov		eax, 0x80000001
	cpuid
	test 	edx, 1 << 29
	jz  .NoLongMode
	popad
	ret
.NoLongMode:
	mov ebx, NoLongModeMsg
	call	Puts32
	hlt

Stage3:

	;-------------------------------;
	;   Set registers				;
	;-------------------------------;

	mov	ax, DATA_DESC		; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h

	call	ClrScr32
	xchg 	bx, bx
	call 	CheckCPUID
	
	call 	CheckLongMode
	xchg    bx, bx
	call	EnablePaging

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax
	xchg bx, bx

	lgdt[GDT64.Pointer]
	jmp GDT64.Code:Realm64 

%include "vmx.inc"

[BITS 64]

Realm64:
	cli
	mov ax, GDT64.Data
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov rsp, 0x200000
	mov ebx, EnteredRealMode
	xchg bx, bx
	call Puts64
	
	call CheckVMX
	call TurnVMXON
	call EnableEPT
	xchg bx, bx
	call PrepareVMLaunch

	mov rbx, rsp
	mov rax, 006C14h
	vmwrite rax, rbx
	xchg bx, bx
	vmlaunch

	hlt

;-- header information format for PE files -------------------

;typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
;    USHORT e_magic;         // Magic number (Should be MZ
;    USHORT e_cblp;          // Bytes on last page of file
;    USHORT e_cp;            // Pages in file
;    USHORT e_crlc;          // Relocations
;    USHORT e_cparhdr;       // Size of header in paragraphs
;    USHORT e_minalloc;      // Minimum extra paragraphs needed
;    USHORT e_maxalloc;      // Maximum extra paragraphs needed
;    USHORT e_ss;            // Initial (relative) SS value
;    USHORT e_sp;            // Initial SP value
;    USHORT e_csum;          // Checksum
;    USHORT e_ip;            // Initial IP value
;    USHORT e_cs;            // Initial (relative) CS value
;    USHORT e_lfarlc;        // File address of relocation table
;    USHORT e_ovno;          // Overlay number
;    USHORT e_res[4];        // Reserved words
;    USHORT e_oemid;         // OEM identifier (for e_oeminfo)
;    USHORT e_oeminfo;       // OEM information; e_oemid specific
;    USHORT e_res2[10];      // Reserved words
;    LONG   e_lfanew;        // File address of new exe header
;  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

;<<------ Real mode stub program -------->>

;<<------ Here is the file signiture, such as PE00 for NT --->>

;typedef struct _IMAGE_FILE_HEADER {
;    USHORT  Machine;
;    USHORT  NumberOfSections;
;    ULONG   TimeDateStamp;
;    ULONG   PointerToSymbolTable;
;    ULONG   NumberOfSymbols;
;    USHORT  SizeOfOptionalHeader;
;    USHORT  Characteristics;
;} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

;struct _IMAGE_OPTIONAL_HEADER {
;    //
;    // Standard fields.
;    //
;    USHORT  Magic;
;    UCHAR   MajorLinkerVersion;
;    UCHAR   MinorLinkerVersion;
;    ULONG   SizeOfCode;
;    ULONG   SizeOfInitializedData;
;    ULONG   SizeOfUninitializedData;
;    ULONG   AddressOfEntryPoint;			<< IMPORTANT!
;    ULONG   BaseOfCode;
;    ULONG   BaseOfData;
;    //
;    // NT additional fields.
;    //
;    ULONG   ImageBase;
;    ULONG   SectionAlignment;
;    ULONG   FileAlignment;
;    USHORT  MajorOperatingSystemVersion;
;    USHORT  MinorOperatingSystemVersion;
;    USHORT  MajorImageVersion;
;    USHORT  MinorImageVersion;
;    USHORT  MajorSubsystemVersion;
;    USHORT  MinorSubsystemVersion;
;    ULONG   Reserved1;
;    ULONG   SizeOfImage;
;    ULONG   SizeOfHeaders;
;    ULONG   CheckSum;
;    USHORT  Subsystem;
;    USHORT  DllCharacteristics;
;    ULONG   SizeOfStackReserve;
;    ULONG   SizeOfStackCommit;
;    ULONG   SizeOfHeapReserve;
;    ULONG   SizeOfHeapCommit;
;    ULONG   LoaderFlags;
;    ULONG   NumberOfRvaAndSizes;
;    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
;} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

