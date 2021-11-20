#include "oprintf.h"

#include <stdbool.h>
#include <stdint.h>

#include "convert.h"

static char* uli2a(uint64_t num, uint8_t base, bool uc, char* bf) {
    uint8_t n = 0;
    uint64_t d = 1;
    while(num / d >= base) {
        d *= base;
    }
    while(d != 0) {
        uint8_t dgt = num / d;
        num %= d;
        d /= base;
        if((n != 0) || dgt > 0 || d == 0) {
            *bf = (char)(dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10));
            bf++;
            ++n;
        }
    }
    *bf = '\0';
    return bf;
}

static char* li2a(int64_t num, char* bf) {
    if(num < 0) {
        num = -num;
        *bf = '-';
        bf++;
    }
    return uli2a(num, 10, false, bf);
}

static void ui2a(uint32_t num, unsigned int base, bool uc, char* bf) {
    int n = 0;
    uint32_t d = 1;
    while(num / d >= base) {
        d *= base;
    }
    while(d != 0) {
        uint8_t dgt = num / d;
        num %= d;
        d /= base;
        if((n != 0) || dgt > 0 || d == 0) {
            *bf = dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10);
            bf++;
            ++n;
        }
    }
    *bf = 0;
}

static void i2a(int32_t num, char* bf) {
    if(num < 0) {
        num = -num;
        *bf = '-';
        bf++;
    }
    ui2a(num, 10, false, bf);
}

static int8_t a2d(char ch) {
    if(ch >= '0' && ch <= '9') {
        return ch - '0';
    } else {
        return -1;
    }
}

static char a2i(char ch, const char** src, int32_t* nump) {
    const char* p = *src;
    int32_t num = 0, digit = a2d(ch);
    while(digit >= 0) {
        num = num * 10 + digit;
        ch = *p;
        p++;
        digit = a2d(ch);
    }
    *src = p;
    *nump = num;
    return ch;
}

static void putsw(ostream_t* s, int n, bool z, const char* bf) {
    char fc = z ? '0' : ' ';
    const char* p = bf;
    while(*p && n > 0) {
        n--;
        p++;
    }
    while(0 < n) {
        s->f_putch(s, fc);
        n--;
    }
    int32_t len = strlen(bf);
    s->f_putstr(s, bf, len); /*Error*/
}

typedef enum {
    size_signed_char,
    size_short,
    size_normal,
    size_long,
    size_long_long,
    size_long_long_long = 8,
    size_size_t
} format_size_t;

void ovprintf(ostream_t* s, const char* fmt, va_list va) {
    char bf[MAX_INT64_STR_LEN_10 + MAX_PRECISION + 1 + 1];
    const char* start = NULL;
    int len = 0;
    while(*fmt) {
        char ch = *fmt;
        fmt++;
        if(ch != '%') {
            if(len == 0) {
                len = 1;
                start = fmt - 1;
            } else {
                len++;
            }
        } else {
            bool leading_zero = false;
            format_size_t size = size_normal;
            int32_t width = 0;
            int32_t precision = -1;
            if(len) {
                s->f_putstr(s, start, len);
                len = 0;
            }
            ch = *fmt;
            fmt++;
            if(ch == '0') {
                ch = *fmt;
                fmt++;
                leading_zero = true;
            }
            if(ch >= '0' && ch <= '9') {
                ch = a2i(ch, &fmt, &width);
                if(ch == '.') {
                    ch = *fmt;
                    fmt++;
                    if(ch >= '0' && ch <= '9') {
                        ch = a2i(ch, &fmt, &precision);
                        if(precision > (int32_t)MAX_PRECISION) {
                            precision = MAX_PRECISION;
                        }
                    }
                }
            }
            if(ch == 'h') {
                ch = *fmt;
                fmt++;
                size = size_short;
            }
            if(ch == 'h') {
                ch = *fmt;
                fmt++;
                size = size_signed_char;
            }
            if(ch == 'l' || ch == 'L') {
                ch = *fmt;
                fmt++;
                size = size_long;
            }
            if(ch == 'z') {
                ch = *fmt;
                fmt++;
                size = size_size_t;
            }
            if(ch == 'l' || ch == 'L') {
                ch = *fmt;
                fmt++;
                size = size_long_long;
            }
            if(ch == 'l' || ch == 'L') {
                ch = *fmt;
                fmt++;
                size = size_long_long_long;
            }

            if(ch == 0) {
                return;
            }
            switch(ch) {
            case 'u':
                switch(size) {
                case size_long:
                    ui2a(va_arg(va, unsigned long), 10, false, bf);
                    break;
                case size_long_long:
                    uli2a(va_arg(va, uint64_t), 10, false, bf);
                    break;
                case size_long_long_long:
                    uli2a(va_arg(va, uint64_t), 10, false, bf);
                    break;
                case size_size_t:
                    ui2a(va_arg(va, size_t), 10, false, bf);
                    break;
                case size_signed_char:
                case size_short:
                case size_normal:
                default:
                    ui2a(va_arg(va, unsigned int), 10, false, bf);
                    break;
                }
                putsw(s, width, leading_zero, bf);
                break;
            case 'd':
                switch(size) {
                case size_long:
                    i2a(va_arg(va, long), bf);
                    break;
                case size_size_t:
                    i2a(va_arg(va, ssize_t), bf);
                    break;
                case size_long_long:
                    li2a(va_arg(va, long long), bf);
                    break;
                case size_signed_char:
                case size_short:
                case size_normal:
                default:
                    i2a(va_arg(va, int), bf);
                    break;
                }
                putsw(s, width, leading_zero, bf);
                break;
            case 'x':
            case 'X':
            case 'p':
                switch(size) {
                case size_long:
                    ui2a(va_arg(va, unsigned long), 16, (ch == 'X'), bf);
                    break;
                case size_size_t:
                    ui2a(va_arg(va, size_t), 16, (ch == 'X'), bf);
                    break;
                case size_long_long:
                    uli2a(va_arg(va, uint64_t), 16, (ch == 'X'), bf);
                    break;
                case size_short:
                case size_signed_char:
                case size_normal:
                default:
                    ui2a(va_arg(va, unsigned int), 16, (ch == 'X'), bf);
                    break;
                }
                putsw(s, width, leading_zero, bf);
                break;
            case 'c':
                s->f_putch(s, (char)(va_arg(va, int)));
                break;
            case 's':
                putsw(s, width, false, va_arg(va, const char*)); /*Error*/
                break;
            case 'f':
            case 'g':
                dtoa_(va_arg(va, double), precision, bf);
                putsw(s, width, false, bf);
                break;
            case '%':
                s->f_putch(s, ch);
                break;
            default:
                s->f_putstr(s, "???", 3);
                s->f_putch(s, ch);
                break;
            }
        }
    }
    if(len) {
        s->f_putstr(s, start, len);
        len = 0;
    }
}

void oprintf(ostream_t* s, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    ovprintf(s, fmt, va);
    va_end(va);
}
