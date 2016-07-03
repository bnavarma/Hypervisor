#include "gdt.h"
#include <string.h>

struct gdtr {
    uint16_t m_limit;
    uint32_t m_base;
};

static struct gdt_descriptor _gdt[MAX_DESCRIPTIONS];

static struct gdtr _gdtr;

static void gdt_install();

static void gdt_install() {
#ifdef ARCH_X86
    _asm lgdt[_gdtr]
#endif // ARCH_X86
}

void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand) {
    if (i > MAX_DESCRIPTIONS) return;

    memset((void*)&_gdt[i], 0, sizeof(gdt_descriptor));

    _gdt[i].baseLo = uint16_t(base & 0xffff);
    _gdt[i].baseMid = uint8_t((base >> 16) & 0xff);
    _gdt[i].baseHi = uint8_t((base >> 24) & 0xff);
    _gdt[i].limit = uint16_t(limit & 0xffff);

    _gdt[i].flags = access;
    _gdt[i].grand = uint8_t((limit >> 16) & 0x0f);
    _gdt[i].grand |= grand & 0xf0;
}

gdt_descriptor* i86_gdt_get_descriptor(int i) {
    if (i > MAX_DESCRIPTIONS) return 0;

    return &_gdt[i];
}

int i86_gdt_initialize() {
    _gdtr.m_limit = (sizeof(struct gdt_descriptor)*MAX_DESCRIPTIONS) - 1;
    _gdtr.m_base = (uint32_t)&_gdt[0];

    gdt_set_descriptor(0, 0, 0, 0, 0);

    gdt_set_descriptor(1, 0, 0xffffffff, I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY, I86_GDT_GRAND_4K, I86_GDT_GRAND_32BIT, I86_GDT_GRAND_LIMITHI_MASK);

    gdt_install();

    return 0;
}