#include <stdio.h>

#include "common_commands.h"
#include "log.h"
#include "base_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_IWDT
#include "iwdt_commands.h"
#else
#define IWDT_COMMANDS
#endif

#ifdef HAS_CORTEX_M4
#include "core_commands.h"
#else
#define CORTEX_M4_COMMANDS
#endif


#ifdef HAS_UBLOX
#include "ublox_command.h"
#else
#define UBLOX_COMMANDS
#endif

#ifdef HAS_NMEA
#include "nmea_command.h"
#else
#define NMEA_COMMANDS
#endif

#define SHELL_COMMANDS                                                         \
  COMMON_COMMANDS                                                              \
  CORTEX_M4_COMMANDS                                                           \
  UBLOX_COMMANDS                                                               \
  NMEA_COMMANDS                                                                \
  IWDT_COMMANDS                                                                \
  LOG_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif
