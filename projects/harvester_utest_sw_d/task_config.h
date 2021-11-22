#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define TASKS                                                                  \
  TASKS_BASE                                                                   \
  TASK(NMEA)                                                                   \
  TASK(UBX)                                                                    \

#endif /* TASK_CONFIG_H */
