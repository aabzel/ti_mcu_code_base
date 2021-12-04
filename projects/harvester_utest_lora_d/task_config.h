#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define TASKS               \
  TASKS_BASE                \
  TASK(LORA)                \
  TASK(RS232)               \
  TASK(SX1262)              \
  TASK(UART0_FWD)           \
  TASK(UART1_FWD)

#endif /* TASK_CONFIG_H */
