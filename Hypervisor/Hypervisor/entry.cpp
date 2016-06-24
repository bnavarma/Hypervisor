/*
==============================================
    entry.cpp
        -This is the kernel entry point. This is called
        from the boot loader
==============================================
*/

extern void __cdecl main();
extern void __cdecl InitializeConstructors();
extern void __cdecl Exit();

void __cdecl kernel_entry() {

#ifdef ARCH_X86
    _asm {
        cli
        mov ax, 10h
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov esp, 0x90000
        mov ebp, esp
        push ebp
    }
#endif

    InitializeConstructors();

    main();

    Exit();

#ifdef ARCH_X86
    _asm cli
#endif // ARCH_X86
    for (;;);
}