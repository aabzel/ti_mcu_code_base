#ifndef SYS_STM32F413ZGJ6_H
#define SYS_STM32F413ZGJ6_H

#include <stdbool.h>
#include <stdint.h>

#include "macro_utils.h"

#define RAM_SIZE (192 * K_BYTES)
#define APP_START_ADDRESS (0x08000000)

#define _disable_interrupt_()
#define _enable_interrupt_()

#ifdef __cplusplus
extern "C" {
#endif

#define RAM_START 0x20000000
#define RAM_END (RAM_START + RAM_SIZE)


#define ROM_START 0x08000000
#define ROM_END (ROM_START + ROM_SIZE)

extern uint32_t critical_nesting_level;

bool isFromInterrupt(void);
void enter_critical(void);
void exit_critical(void);
bool set_read_protection(void);

#ifdef __cplusplus
}
#endif

#endif /* SYS_STM32F413ZGJ6_H */
