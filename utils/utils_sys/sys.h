#ifndef SYS_CC2652R1F_H
#define SYS_CC2652R1F_H

#include <stdbool.h>
#include <stdint.h>


#define APP_START_ADDRESS (0x00000000)
#define MCU_NAME "CC2652R1F"

#define _disable_interrupt_()
#define _enable_interrupt_()

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t critical_nesting_level;

bool isFromInterrupt(void);
void enter_critical(void);
void exit_critical(void);
bool set_read_protection(void);

#ifdef __cplusplus
}
#endif

#endif /* SYS_CC2652R1F_H */
