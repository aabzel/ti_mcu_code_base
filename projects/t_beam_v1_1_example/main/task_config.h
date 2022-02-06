#ifndef TASKS_CONFIG_H
#define TASKS_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS           \
  TASKS_BASE                \
  TASK(LORA)                \
  TASK(TBFP)                \
  TASK(SX1262)              \

#endif /* TASKS_CONFIG_H */
