#include "string_writer.h"

#include <string.h>

void string_putc(void* _s, char ch) {
    string_stream_t* s = (string_stream_t*)_s;
    if(s->pos < s->size - 1) {
        s->data[s->pos] = ch;
        s->pos++;
        s->data[s->pos] = '\0';
    } else {
        s->overflow = true;
    }
}

void string_puts(void* _s, const char* str, int32_t len) {
    int i;
    if(len < 0) {
        len = strlen(str);
    }
    for(i = 0; i < len; i++) {
        string_putc(_s, str[i]);
    }
}
