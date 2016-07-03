/*
========================================
    main.cpp
        -kernel startup code
========================================
*/

#include <bootinfo.h>
#include <hal.h>

#include "DebugDisplay.h"
#include "exception.h"
#include "mmngr_phys.h"
#include "mmngr_virtual.h"

struct memory_region {
    uint32_t startLo;
    uint32_t startHi;
    uint32_t sizeLo;
    uint32_t sizeHi;
    uint32_t type;
    uint32_t acpi_3_0;
};

char* strMemoryTypes[] = {
    {"Available"},
    {"Reserved"},
    {"ACPI Reclaim"},
    {"ACPI NVS Memory"}
};

uint32_t kernelSize = 0;
uint32_t size = 0;

void __cdecl kmain(multiboot_info* bootinfo) {
#ifdef ARCH_X86
    _asm mov word ptr[kernelSize], dx
#endif // ARCH_X86

    DebugClrScr(0x13);
    DebugGotoXY(0, 0);
    DebugSetColor(0x17);
    DebugPrintf("Avinash is King...\n");

    hal_initialize();

    enable();
    setvect(0, (void(__cdecl &)(void))divide_by_zero_fault);
    setvect(1, (void(__cdecl &)(void))single_step_trap);
    setvect(2, (void(__cdecl &)(void))nmi_trap);
    setvect(3, (void(__cdecl &)(void))breakpoint_trap);
    setvect(4, (void(__cdecl &)(void))overflow_trap);
    setvect(5, (void(__cdecl &)(void))bounds_check_fault);
    setvect(6, (void(__cdecl &)(void))invalid_opcode_fault);
    setvect(7, (void(__cdecl &)(void))no_device_fault);
    setvect(8, (void(__cdecl &)(void))double_fault_abort);
    setvect(10, (void(__cdecl &)(void))invalid_tss_fault);
    setvect(11, (void(__cdecl &)(void))no_segment_fault);
    setvect(12, (void(__cdecl &)(void))stack_fault);
    setvect(13, (void(__cdecl &)(void))general_protection_fault);
    setvect(14, (void(__cdecl &)(void))page_fault);
    setvect(16, (void(__cdecl &)(void))fpu_fault);
    setvect(17, (void(__cdecl &)(void))alignment_check_fault);
    setvect(18, (void(__cdecl &)(void))machine_check_abort);
    setvect(19, (void(__cdecl &)(void))simd_fpu_fault);

    pmmngr_init(bootinfo->m_memorySize, 0xC0000000 + kernelSize * 512);
    DebugPrintf("pmm initialized with %i KB\n", bootinfor->m_memorySize);

    memory_region* region = (memory_region*)0x1000;

    for (int i = 0; i < 10; ++i) {
        if (region[i].type > 4) break;
        if (i > 0 && region[i].startLo == 0) break;

        DebugPrintf("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i(%s)\n", i, region[i].startHi, region[i].startLo, region[i].sizeHi, region[i].sizeLo, region[i].type, strMemoryTypes[region[i].type - 1]);

        pmmngr_init_region(region[i].startLo, region[i].sizeLo);
    }

    pmmngr_deinit_region(0x100000, kernelSize * 512);
    DebugPrintf("pmm regions initialized: %i allocation blocks; block size: %i bytes", pmmngr_get_block_count(), pmmngr_get_block_size());

    vmmngr_initialize();

#ifdef ARCH_X86
    _asm {
        cli
        hlt
    }
#endif // ARCH_X86

    for (;;);
}