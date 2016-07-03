#ifdef _MMNGR_PHYS_H
#define _MMNGR_PHYS_H

#include <stdint.h>

typedef uint32_t physical_addr;

extern void pmmngr_init(size_t, physical_addr);

extern void pmmngr_init_region(physical_addr, size_t);

extern void pmmngr_deinit_region(physical_addr, size_t);

extern void* pmmngr_alloc_block();

extern void pmmngr_free_block();

extern void* pmmngr_alloc_blocks();

extern void pmmngr_free_blocks(void*, size_t);

extern size_t pmmngr_get_memory_size();

extern uint32_t pmmngr_get_use_block_count();

extern uint32_t pmmngr_get_free_block_count();

extern uint32_t pmmngr_get_block_count();

extern uint32_t pmmngr_get_block_size();

extern void pmmngr_paging_enable(bool);

extern bool pmmngr_is_paging();

extern bool pmmngr_load_PDBR(physical_addr);

extern physical_addr pmmngr_get_PDBR();

#endif // _MMNGR_PHYS_H
