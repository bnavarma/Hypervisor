#include <Hal.h>
#include "cpu.h"
#include "pic.h"
#include "pit.h"
#include "idt.h"

int __cdecl hal_initialize() {
    disable();

    i86_cpu_initialize();
    i86_pic_initialize(0x20, 0x28);
    i86_pit_initialize();
    i86_pit_start_counter(100, I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);

    enable();

    return 0;
}

int __cdecl hal_shutdown() {
    i86_cpu_shutdown();
    return 0;
}

void __cdecl geninterrupt(int n) {
#ifdef ARCH_X86
    _asm {
        mov al, byte ptr[n]
        mov byte ptr [genint+1], al
        jmp genint
    genint:
        int 0
    }
#endif // ARCH_X86
}

inline void __cdecl interruptdone(unsigned int intno) {
    if (intno > 16) return;

    if (intno >= 8) i86_pic_send_command(I86_PIC_OCW2_MASK_EOI, 1);

    i86_pic_send_command(I86_PIC_OCW2_MASK_EOI, 0);
}

void __cdecl sound(unsigned frequency) {
    outportb(0x61, 3 | unsigned char(frequency << 2));
}

unsigned char __cdecl inportb(unsigned short portid) {
#ifdef ARCH_X86
    _asm {
        mov dx, word ptr[portid]
        in al, dx
        mov byte ptr [portid], al
    }
#endif // ARCH_X86
    return (unsigned char)portid;
}

void __cdecl outportb(unsigned short portid, unsigned char value) {
#ifdef ARCH_X86
    _asm {
        mov al, byte ptr [value]
        mov dx, word ptr [portid]
        out dx, al
    }
#endif // ARCH_X86
}

void __cdecl enable() {
#ifdef ARCH_X86
    _asm sti
#endif // ARCH_X86
}

void __cdecl disable() {
#ifdef ARCH_X86
    _asm cli
#endif // ARCH_X86
}

void __cdecl setvect(int intno, void(__cdecl far &vect)()) {
    i86_install_ir(intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32, 0x8, vect);
}

void(__cdecl far * __cdecl getvect(int intno))() {
    idt_descriptor* desc = i86_get_ir(intno);
    if (!desc) return 0;

    uint32_t addr = desc->baseLo | (desc->baseHi << 16);

    I86_IRQ_HANDLER irq = (I86_IRQ_HANDLER)addr;
    return irq;
}

const char* __cdecl get_cpu_vender() {
    return i86_cpu_get_vender();
}

int __cdecl get_tick_count() {
    return i86_pit_get_tick_count();
}