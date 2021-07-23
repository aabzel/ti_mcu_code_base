#include <stdio.h>

#include "common_commands.h"
#include "log.h"
#include "shell.h"

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
  
#define SHELL_COMMANDS                                                         \
  COMMON_COMMANDS                                                              \
  CORTEX_M4_COMMANDS                                                           \
  IWDT_COMMANDS                                                                \
  LOG_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif
