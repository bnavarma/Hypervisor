#include <string.h>
#include <stdio.h>

extern void itoa_s(int i, unsigned base, char* buf);

int vsprintf(char *str, const char *format, va_list ap){
    if(!str) return 0;

    if(!format) return 0;

    size_t loc = 0;
    size_t i;

    for(i=0; i<=strlen(format);i++,loc++){
        switch(format[i]){
            case '%':
                switch(format[i+1]){
                    case 'c':{
                        char c = va_arg(ap, char);
                        str[loc] = c;
                        i++;
                        break;
                    }

                    case 'd':
                    case 'i':{
                        int c = va_arg(ap, int);
                        char s[32]={0};
                        itoa_s(c, 10, s);
                        strcpy(&str[loc], s);
                        loc += strlen(s) - 2;
                        i++;
                        break;
                    }

                    case 'X':
                    case 'x':{
                        int c = va_arg(ap, int);
                        char s[32] = {0};
                        itoa_s(c, 16, s);
                        strcpy(&str[loc], s);
                        i++;
                        loc += strlen(s) - 2;
                        break;
                    }

                    case 's':{
                        int c = (int&)va_arg(ap, char);
                        char s[32] = {0};
                        strcpy(s, (const char*)c);
                        strcpy(&str[loc], s);
                        i++;
                        loc += strlen(s) - 2;
                        break;
                    }
                }
                break;

            default:
                str[loc] = format[i];
                break;
        }
    }

    return i;
}