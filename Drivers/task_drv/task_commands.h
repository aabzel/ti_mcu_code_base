#ifndef TASK_COMMANDS_H
#define TASK_COMMANDS_H

#include <stdbool.h>
#include <stdint.h>

bool cmd_task_report(int32_t argc, char* argv[]);
bool cmd_task_clear(int32_t argc, char* argv[]);

#define TASK_COMMANDS                                                                                                  \
    SHELL_CMD("task_report", "ti", cmd_task_report, "Task execution time report"),                                     \
    SHELL_CMD("task_clear", "tic", cmd_task_clear, "Clear task execution info"),

#endif /* TASK_COMMANDS_H */
