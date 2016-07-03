#include "exception.h"
#include <hal.h>
#include <stdint.h>

extern void __cdecl kernel_panic(const char *fmt, ...);

#define intstart() \
    _asm    cli \
    _asm    sub     ebp, 4

void __cdecl divide_by_zero_fault(uint32_t eflags, uint32_t cs, uint32_t eip, uint32_t other){

    _asm{
        cli
        add     esp, 12
        pushad
    }

    kernel_panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] other: 0x%x", cs, epi, eflags, other);
    for(;;);
}

void interrupt __cdecl single_step_trap(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Single step");
    for(;;);
}

void interrupt __cdecl nmi_trap(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("NMI trap");
    for(;;);
}

void interrupt __cdecl breakpoint_trap(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Breakpoint trap");
    for(;;);
}

void interrupt __cdecl overflow_trap(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Overflow trap");
    for(;;);
}

void interrupt __cdecl bounds_check_fault(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Bounds check fault");
    for(;;);
}

void interrupt __cdecl invalid_opcode_fault(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Invalid opcode");
    for(;;);
}

void interrupt __cdecl no_device_fault(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Device not found");
    for(;;);
}

void interrupt __cdecl double_fault_abort(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Double fault");
    for(;;);
}

void interrupt __cdecl invalid_tss_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Invalid TSS");
    for(;;);
}

void interrupt __cdecl no_segment_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Invalid segment");
    for(;;);
}

void interrupt __cdecl stack_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Stack fault");
    for(;;);
}

void interrupt __cdecl general_protection_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("General Protection Fault");
    for(;;);
}

void __cdecl page_fault(uint32_t err, uint32_t eflags, uint32_t cs, uint32_t eip){
    _asm        cli
    _asm        sub     ebp, 4

    _asm        popad
    _asm        sti
    _asm        iretd
}

void interrupt __cdecl fpu_fault(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("FPU Fault");
    for(;;);
}

void interrupt __cdecl alignment_check_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Alignment Check");
    for(;;);
}

void interrupt __cdecl machine_check_abort(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("Machine check");
    for(;;);
}

void interrupt __cdecl simd_fpu_fault(unsigned int cs, unsigned int eip, unsigned int eflags){
    intstart();
    kernel_panic("FPU SIMD fault");
    for(;;);
}