#ifndef _MMNGR_VIRT_H
#define _MMNGR_VIRT_H

#include <stdint.h>
#include "vmmngr_pte.h"
#include "vmmngr_pde.h"


typedef uint32_t virtual_addr;

#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

struct ptable {
    pt_entry m_entries[PAGES_PER_TABLE];
};


struct pdirectory {
    pd_entry m_entries[PAGES_PER_DIR];
};

extern void MmMapPage(void* phys, void* virt);

extern void vmmngr_initialize();

extern bool vmmngr_alloc_page(pt_entry*);

extern void vmmngr_free_page(pt_entry* e);

extern bool vmmngr_switch_pdirectory(pdirectory*);

extern pdirectory* vmmngr_get_directory();

extern void vmmngr_flush_tlb_entry(virtual_addr addr);

extern void vmmngr_ptable_clear(ptable* p);

extern uint32_t vmmngr_ptable_virt_to_index(virtual_addr addr);

extern pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr);

extern uint32_t vmmngr_pdirectory_virt_to_index(virtual_addr addr);

extern void vmmngr_pdirectory_clear(pdirectory* dir);

extern pd_entry* vmmngr_pdirectory_lookup_entry(pdirectory* p, virtual_addr addr);

#endif
