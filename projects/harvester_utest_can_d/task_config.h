#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS               \
  TASKS_BASE                \
  TASK(RS232)               \
  TASK(TCAN4550)


#endif /* TASK_CONFIG_H */
