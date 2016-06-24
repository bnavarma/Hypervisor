#ifndef _MSC_VER
#error "We need Microsoft Visual C++ compiler"
#endif

//Function pointer
typedef void(__cdecl *_PVFV)(void);

//__xc_a points to beginning of initializer table
#pragma data_seg(".CRT$XCA")
_PVFV __xc_a[] = { 0 };

//__xc_z points to end of initalizer table
#pragma data_seg(".CRT$XCZ")
_PVFV __xc_z[] = { 0 };

//Select the default data segment again (.data) for the rest of the unit
#pragma comment(linker, "/merge:.CRT=.data")

/*
=====================================
    Globals
=====================================
*/

// function pointer table to global deinitializer table
static _PVFV *pf_atexitlist = 0;

// Maximum entries allowed in table. Change as needed
static unsigned max_atexitlist_entries = 32;

// Current amount of entries in table
static unsigned cur_atexitlist_entries = 0;

/*
====================================
    CRT Routines
====================================
*/

// Initialize all global initializers (ctors, statics, globals, etc...)
void __cdecl _initterm(_PVFV *pfbegin, _PVFV *pfend) {
    // Go through each initializer
    while (pfbegin < pfend) {
        // Execute the global initializer
        if (*pfbegin != 0) {
            (**pfbegin)();
            ++pfbegin;
        }
    }
}

// initialize the de-initializer function table
void __cdecl _atexit_init(void) {
    max_atexitlist_entries = 32;

    pf_atexitlist = (_PVFV *)0x5000;
}

// adds a new function entry that is to be called at shutdown
int __cdecl atexit(_PVFV fn) {
    // Insure we have enough free space
    if (cur_atexitlist_entries >= max_atexitlist_entries) {
        return 1;
    }
    else {
        *(pf_atexitlist++) = fn;
        cur_atexitlist_entries++;
    }
    return 0;
}

// shutdown the C++ runtime; calls all global de-initializers
void __cdecl Exit() {
    while (cur_atexitlist_entries--) {
        (*(--pf_atexitlist))();
    }
}

// purecall function handler
int __cdecl ::_purecall() { for (;;); return 0; }

// global new and delete operators
void* __cdecl :: operator new (unsigned int size) { return 0; }
void* __cdecl :: operator new[](unsigned int size) {return 0; }
void __cdecl :: operator delete (void* p){}
void __cdecl :: operator delete[] (void* p){}

extern "C" float __declspec(naked) _CIcos() {
#ifdef ARCH_X86
    _asm fcos
#endif
};

extern "C" float __declspec(naked) _CIsin() {
#ifdef ARCH_X86
    _asm fsin
#endif
};

extern "C" float __declspec(naked) _CIsqrt() {
#ifdef ARCH_X86
    _asm fsqrt
#endif
};

// called by MSVC++ to convert a float to a long
extern "C" long __declspec(naked) _ftol2_sse() {
    int a;
#ifdef ARCH_X86
    _asm {
        fistp [a]
        mov ebx, a
    }
#endif
}

// required by MSVC++ runtime for floating point operations (Must be 1)
extern "C" int _fltused = 1;