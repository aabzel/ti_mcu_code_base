#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS           \
  TASKS_BASE                \
  TASK(RS232)               \
  TASK(TCAN4550)            \
  TASK(CAN)                 \
  TASK(UART0_FWD)           \
  TASK(UART1_FWD)           \
  TASK(WDT)

#endif /* TASK_CONFIG_H */
