#ifndef _GDT_H_INCLUDED
#define _GDT_H_INCLUDED

#include <stdint.h>

#define MAX_DESCRIPTIONS 3

#define I86_GDT_DESC_ACCESS 0x0001

#define I86_GDT_DESC_READWRITE 0x0002

#define I86_GDT_DESC_EXPANSION 0x0004

#define I86_GDT_DESC_EXEC_CODE 0x0008

#define I86_GDT_DESC_CODEDATA 0x0010

#define I86_GDT_DESC_DPL 0x0060

#define I86_GDT_DESC_MEMORY 0x0080

#define I86_GDT_GRAND_LIMITHI_MASK 0x0f

#define I86_GDT_GRAND_OS 0x10

#define I86_GDT_GRAND_32BIT 0x40

#define I86_GDT_GRAND_4K 0x80

struct gdt_descriptor {
    uint16_t limit;

    uint16_t baseLo;
    uint8_t baseMid;

    uint8_t flags;

    uint8_t grand;

    uint8_t baseHi;
};

extern void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand);

extern gdt_descriptor* i86_gdt_get_descriptor(int i);

extern int i86_gdt_initialize();

#endif // !_GDT_H_INCLUDED
