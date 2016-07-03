#include <string.h>
#include "mmngr_virtual.h"
#include "mmngr_phys.h"

#define PTABLE_ADDR_SPACE_SIZE 0x400000

#define DTABLE_ADDR_SPACE_SIZE 0x100000000

#define PAGE_SIZE 4096

pdirectory* _cur_directory = 0;

physical_addr _cur_pdbr = 0;

inline pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr) {
    if (p) return &p->m_entries[PAGE_TABLE_INDEX(addr)];

    return 0;
}

inline pd_entry* vmmngr_pdirectory_lookup_entry(pdirectory* p, virtual_addr addr) {
    if (p) return &p->m_entries[PAGE_TABLE_INDEX(addr)];
    return 0;
}

inline bool vmmngr_switch_pdirectory(pdirectory* dir) {
    if (!dir) return false;

    _cur_directory = dir;
    pmmngr_load_PDBR(_cur_pdbr);
    return true;
}

void vmmngr_flush_tlb_entry(virtual_addr addr) {
#ifdef ARCH_X86
    _asm {
        cli
        invlpg addr
        sti
    }
#endif
}

pdirectory* vmmngr_get_directory() {
    return _cur_directory;
}

bool vmmngr_alloc_page(pt_entry* e) {
    void* p = pmmngr_alloc_block();
    if (!p) {
        return false;
    }

    pt_entry_set_frame(e, (physical_addr)p);
    pt_entry_add_attrib(e, I86_PTE_PRESENT);

    return true;
}

void vmmngr_free_page(pt_entry* e) {
    void* p = (void*)pt_entry_pfn(*e);
    if (p) {
        pmmngr_free_block(p);
    }
    pt_entry_del_attrib(e, I86_PTE_PRESENT);
}

void vmmngr_map_page(void* phys, void* virt) {
    pdirectory* pageDirectory = vmmngr_get_directory();

    pd_entry* e = &pageDirectory->m_entries[PAGE_DIRECTORY_INDEX((uint32_t)virt)];
    if ((*e & I86_PTE_PRESENT) != I86_PTE_PRESENT) {
        ptable *table = (ptable*)pmmngr_alloc_block();
        if (!table) return;

        memset(table, 0, sizeof(ptable));

        pd_entry* entry = &pageDirectory->m_entries[PAGE_DIRECTORY_INDEX((uint32_t)virt)];

        pd_entry_add_attrib(entry, I86_PDE_PRESENT);
        pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
        pd_entry_set_frame(entry, (physical_addr)table);
    }

    ptable* table = (ptable*)PAGE_GET_PHYSICAL_ADDRESS(e);

    pt_entry* page = &table->m_entries[PAGE_TABLE_INDEX((uint32_t)virt)];

    pt_entry_set_frame(page, (physical_addr)phys);
    pt_entry_add_attrib(page, I86_PTE_PRESENT);
}

void vmmngr_initialize() {
    ptable* table = (ptable*)pmmngr_alloc_block();
    if (!table) return;

    ptable* table2 = (ptable*)pmmngr_alloc_block();
    if (!table2) return;

    memset(table, 0, sizeof(ptable));

    for (int i = 0, frame = 0x0, virt = 0x00000000; i < 1024; i++, frame += 4096, virt += 4096) {
        pt_entry page = 0;
        pt_entry_add_attrib(&page, I86_PTE_PRESENT);
        pt_entry_set_frame(&page, frame);

        table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    for (int i = 0, frame = 0x100000, virt = 0xc0000000; i < 1024; i++, frame += 4096, virt += 4096) {
        pt_entry page = 0;
        pt_entry_add_attrib(&page, I86_PTE_PRESENT);
        pt_entry_set_frame(&page, frame);

        table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
    }

    pdirectory* dir = (pdirectory*)pmmngr_alloc_blocks(3);
    if (!dir) return;

    memset(dir, 0, sizeof(pdirectory));

    pd_entry* entry = &dir->m_entries[PAGE_DIRECTORY_INDEX(0xc0000000)];
    pd_entry_add_attrib(entry, I86_PDE_PRESENT);
    pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
    pd_entry_set_frame(entry, (physical_addr)table);

    pd_entry* entry2 = &dir->m_entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    pd_entry_add_attrib(entry2, I86_PDE_PRESENT);
    pd_entry_add_attrib(entry2, I86_PDE_WRITABLE);
    pd_entry_set_frame(entry2, (physical_addr)table2);

    _cur_pdbr = (physical_addr)&dir->m_entries;

    vmmngr_switch_pdirectory(dir);

    pmmngr_paging_enable(true);
}