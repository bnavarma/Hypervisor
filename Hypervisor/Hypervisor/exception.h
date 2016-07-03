#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <stdint.h>

extern void __cdecl divide_by_zero_fault(uint32_t eflags, uint32_t cs, uint32_t eip, uint32_t other);

extern void __cdecl single_step_trap(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl nmi_trap(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl breakpoint_trap(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl overflow_trap(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl bounds_check_fault(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl invalid_opcode_fault(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl no_device_fault(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl double_fault_abort(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl invalid_tss_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl no_segment_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl stack_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl general_protection_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl page_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl fpu_fault(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl alignment_check_fault(unsigned int cs, unsigned int err, unsigned int eip, unsigned int eflags);

extern void __cdecl machine_check_abort(unsigned int cs, unsigned int eip, unsigned int eflags);

extern void __cdecl simd_fpu_fault(unsigned int cs, unsigned int eip, unsigned int eflags);


#endif
