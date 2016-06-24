#include <stdarg.h>
#include <string.h>
#include "DebugDisplay.h"

#define VID_MEMORY 0xB8000

static unsigned int _xPos = 0, _yPos = 0;
static unsigned _startX = 0, _startY = 0;

static unsigned _color = 0;

void DebugPutc(unsigned char c) {
    if (c == 0) {
        return;
    }

    if (c == '\n' || c == '\r') {
        _yPos += 2;
        _xPos = _startX;
        return;
    }

    if (_xPos > 79) {
        _yPos += 2;
        _xPos = _startX;
        return;
    }

    unsigned char* p = (unsigned char*)VID_MEMORY + (_xPos++) * 2 + _yPos * 80;
    *p++ = c;
    *p = _color;
}

char tbuf[32];
char bchars[] = { '0', '1','2','3','4','5','6','7','8','9','A','B','C','D', 'E','F' };

void itoa(unsigned i, unsigned base, char *buf) {
    int pos = 0;
    int opos = 0;
    int top = 0;

    if (i == 0 || base > 16) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (i != 0) {
        tbuf[pos] = bchars[i % base];
        pos++;
        i /= base;
    }
    top = pos--;
    for (opos = 0; opos < top; pos--, opos++) {
        buf[opos] = tbuf[pos];
    }
    buf[opos] = 0;
}

void itoa_s(int i, unsigned base, char* buf) {
    if (base > 16) return;
    if (i < 0) {
        *buf++ = '-';
        i *= -1;
    }
    itoa(i, base, buf);
}

unsigned DebugSetColor(const unsigned c) {
    unsigned t = _color;
    _color = c;
    return t;
}

void DebugGotoXY(unsigned x, unsigned y) {
    _xPos = x * 2;
    _yPos = y * 2;
    _startX = _xPos;
    _startY = _yPos;
}

void DebugClrScr(const unsigned short c) {
    unsigned char* p = (unsigned char*)VID_MEMORY;

    for (int i = 0; i < 160 * 30; i += 2) {
        p[i] = ' ';
        p[i + 1] = c;
    }

    _xPos = _startX; _yPos = _startY;
}

void DebugPuts