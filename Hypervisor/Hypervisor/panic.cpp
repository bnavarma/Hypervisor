#include <hal.h>
#include <stdio.h>
#include "DebugDisplay.h"

static char* sickpic = " \
                               _______      \n\
                               |.-----.|    \n\
                               ||x . x||    \n\
                               ||_.-._||    \n\
                               `--)-(--`    \n\
                              __[=== o]___  \n\
                             |:::::::::::|\\ \n\
                             `-=========-`()\n\
                                M. O. S.\n\n";

void __cdecl kernel_panic(const char* fmt, ...) {
    disable();

    va_list args;
    static char buf[1024];

    va_start(args, fmt);
    va_end(args);

    char* disclaimer = "We apologize, but Fuck it";

    DebugClrScr(0x1f);
    DebugGotoXY(0, 0);
    DebugSetColor(0x1f);
    DebugPuts(sickpic);
    DebugPuts(disclaimer);

    DebugPrintf("*** STOP: %s", fmt);

    for (;;);
}