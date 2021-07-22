#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "common_commands.h"
#include "log.h"
#include "shell.h"

#define SHELL_COMMANDS                                                         \
  COMMON_COMMANDS                                                              \
  LOG_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SHELL_COMMANDS_H */
