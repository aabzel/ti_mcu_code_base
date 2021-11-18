#ifndef WIN_OS_UTILS_H
#define WIN_OS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifdef HAS_MCU
#error That code only for desktop builds
#endif

void clear_tui(void);


#ifdef __cplusplus
}
#endif

#endif /* WIN_OS_UTILS_H */
