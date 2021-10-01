#ifndef UNIT_TEST_COMMANDS_H
#define UNIT_TEST_COMMANDS_H

#include <stdbool.h>
#include <stdint.h>

bool cmd_unit_test_list(int32_t argc, char* argv[]);
bool cmd_unit_test_run(int32_t argc, char* argv[]);

#define UNIT_TEST_COMMANDS                                                                                             \
    SHELL_CMD("test_all", "tsa", cmd_unit_test_list, "Print all unit tests"),                                          \
        SHELL_CMD("test_run", "tsr", cmd_unit_test_run, "Run test"),

#endif /* UNIT_TEST_COMMANDS_H */
