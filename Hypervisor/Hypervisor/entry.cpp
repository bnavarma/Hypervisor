/*
==============================================
    entry.cpp
        -This is the kernel entry point. This is called
        from the boot loader
==============================================
*/


#include <bootinfo.h>
#include "DebugDisplay.h"

extern void __cdecl InitializeConstructors();
extern void __cdecl Exit();

extern int __cdecl kmain(multiboot_info* bootinfo);

void __cdecl kernel_entry(multiboot_info* bootinfo) {

#ifdef ARCH_X86
    _asm {
        cli
        mov ax, 10h
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
    }
#endif

    InitializeConstructors();

    kmain(bootinfo);

    Exit();

    DebugPrintf("\nkernel_entry: shutdown complete. Halting system");

#ifdef ARCH_X86
    _asm cli
    _asm hlt
#endif // ARCH_X86
    for (;;);
}