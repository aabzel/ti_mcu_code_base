#ifndef DIAG_SYS_H
#define DIAG_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "stm32f4xx.h"

#include <stdbool.h>
#include <stdint.h>

#include "sys.h"

/*TODO: cacl frequency from system registers*/

void print_sysinfo(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* DIAG_SYS_H */
