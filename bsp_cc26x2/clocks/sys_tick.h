#ifndef SYS_TICK_H
#define SYS_TICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define HAS_SYS_TIC_INT

#define SYS_TICK_MAX_VAL 0xFFFFFF

volatile extern uint32_t g_up_time_ms;

void SysTickIntHandler(void);
bool SysTickInit(void);

#ifdef __cplusplus
}
#endif

#endif // SYS_TICK_H
