#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS           \
  TASKS_BASE                \
  TASK(LORA)                \
  TASK(RS232)               \
  TASK(SX1262)              \
  TASK(WDT)                 \
  TASK(UART0_FWD)           \
  TASK(UART1_FWD)

#endif /* TASK_CONFIG_H */
