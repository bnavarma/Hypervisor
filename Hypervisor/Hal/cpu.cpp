#include "cpu.h"
#include "gdt.h"
#include "idt.h"

int i86_cpu_initialize() {
    i86_gdt_initialize();
    i86_idt_initialize(0x8);

    return 0;
}

void i86_cpu_shutdown() {

}

char* i86_cpu_get_vender() {
    static char vender[32] = { 0 };

#ifdef ARCH_X86
    _asm {
        mov eax, 0
        cpuid
        mov dword ptr [vender], ebx
        mov dword ptr [vender + 4], edx
        mov dword ptr [vender + 8], ecx
    }
#endif // ARCH_X86

    return vender;
}

void i86_cpu_flush_caches() {
#ifdef ARCH_X86
    _asm {
        cli
        invd
        sti
    }
#endif // ARCH_X86
}

void i86_cpu_flush_caches_write() {
#ifdef ARCH_X86
    _asm {
        cli
        wbinvd
        sti
    }
#endif // ARCH_X86
}

void i86_cpu_flush_tlb_entry(uint32_t addr) {
#ifdef ARCH_X86
    _asm {
        cli
        invlpg addr
        sti
    }
#endif // ARCH_X86
}