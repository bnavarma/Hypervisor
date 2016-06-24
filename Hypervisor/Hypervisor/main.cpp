/*
========================================
    main.cpp
        -kernel startup code
========================================
*/

#include "DebugDisplay.h"

void __cdecl main() {
    int i = 0x12;

    DebugClrScr(0x18);
}