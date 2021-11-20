#ifndef OSTREAM_H
#define OSTREAM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*f_putch)(void* f_data, char ch);
    void (*f_putstr)(void* f_data, const char* s, int32_t len);
} ostream_t;

void oputs(ostream_t* ostream, const char* str);
void oputs_len(ostream_t* ostream, const char* str, int32_t len);
void oputc(ostream_t* ostream, char c);

#ifdef __cplusplus
}
#endif

#endif /* OSTREAM_H*/
