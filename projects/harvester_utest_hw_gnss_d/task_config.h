#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define TASKS               \
  TASKS_BASE                \
  TASK(NMEA)                \
  TASK(RS232)               \
  TASK(UBX)                 \
  TASK(ZED_F9P)


#endif /* TASK_CONFIG_H */
