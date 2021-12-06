#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS                                                              \
  TASKS_BASE                                                                   \
  TASK(NMEA)                                                                   \
  TASK(RS232)                                                                  \
  TASK(UBX)                                                                    \

#endif /* TASK_CONFIG_H */
