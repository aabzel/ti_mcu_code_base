#ifndef OPRINTF_H
#define OPRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "ostream.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef X86_64
typedef int32_t ssize_t;
#endif

void oprintf(ostream_t* s, const char* fmt, ...);
//    __attribute__ ((format (printf, 2, 3)));

void ovprintf(ostream_t* s, const char* fmt, va_list va);

#ifdef __cplusplus
}
#endif

#endif /* OPRINTF_H */
