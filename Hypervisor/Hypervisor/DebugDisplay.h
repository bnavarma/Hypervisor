#ifndef _DEBUGDISPLAY_H
#define _DEBUGDISPLAY_H

extern void DebugPutc(unsigned char c);
extern void DebugClrScr(const unsigned char c);
extern void DebugPuts(char* str);
extern int DebugPrintf(const char* str, ...);
extern unsigned DebugSetColor(const unsigned c);
extern void DebugGotoXY(unsigned x, unsigned y);

#endif
