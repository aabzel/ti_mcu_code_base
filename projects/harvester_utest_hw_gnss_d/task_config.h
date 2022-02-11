#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

#include "task_config_base.h"

#define SYS_TASKS               \
  TASKS_BASE                \
  TASK(NMEA)                \
  TASK(TBFP)                \
  TASK(RS232)               \
  TASK(UART0_FWD)               \
  TASK(UART1_FWD)               \
  TASK(UBX)                 \
  TASK(ZED_F9P)


#endif /* TASK_CONFIG_H */
