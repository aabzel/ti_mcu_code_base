#ifndef SYS_ESP32_H
#define SYS_ESP32_H

#include <stdbool.h>
#include <stdint.h>

#include "macro_utils.h"

#define _disable_interrupt_()
#define _enable_interrupt_()

#ifdef __cplusplus
extern "C" {
#endif

#define RAM_SIZE (192 * K_BYTES)
#define RAM_START 0x40070000
#define RAM_END (RAM_START + RAM_SIZE)
#define APP_START_ADDRESS 0x0001A000

#define ROM_SIZE (384 * K_BYTES)
#define ROM_START 0x40000000
#define ROM_END (ROM_START + ROM_SIZE)

extern uint32_t critical_nesting_level;

bool isFromInterrupt(void);
void enter_critical(void);
void exit_critical(void);
bool set_read_protection(void);

#ifdef __cplusplus
}
#endif

#endif /* SYS_ESP32_H */
