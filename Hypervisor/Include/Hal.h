#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

#ifdef _MSC_VER
#define interrupt __declspec(naked)
#else
#define interrupt
#endif

#define far
#define near

extern int __cdecl hal_initialize();

extern int __cdecl hal_shutdown();

extern void __cdecl enable();

extern void __cdecl disable();

extern void __cdecl geninterrupt(int n);

extern unsigned char __cdecl inportb(unsigned short id);

extern void __cdecl outportb(unsigned short id, unsigned char value);

extern void __cdecl setvect(int intno, void(__cdecl far &vect)());

extern void (__cdecl far * __cdecl getvect(int intno))();

extern void __cdecl interruptdone(unsigned int intno);

extern void __cdecl sound(unsigned frequency);

extern const char* __cdecl get_cpu_vender();

extern int __cdecl get_tick_count();

#endif