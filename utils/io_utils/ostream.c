#include "ostream.h"

#include <stddef.h>
#include <string.h>

void oputs(ostream_t* ostream, const char* str) {
    if(ostream && str) {
        size_t len = strlen(str);
        ostream->f_putstr(ostream, str, len);
    }
}

void oputs_len(ostream_t* ostream, const char* str, int32_t len) {
    if(ostream && str) {
        ostream->f_putstr(ostream, str, len);
    }
}

void oputc(ostream_t* ostream, char c) {
    if(ostream) {
        ostream->f_putch(ostream, c);
    }
}
