#ifndef UTILS_SHELL_H
#define UTILS_SHELL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ostream.h"
#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool show_shell_prompt;
extern bool user_mode;

typedef bool (*shellcmd_t)(int32_t argc, char* argv[]);

typedef struct {
    const char* short_name;
    const char* long_name;
    const char* description;
    const shellcmd_t handler;
} shell_cmd_info_t;

bool cmd_ascii(int32_t argc, char* argv[]);
bool cmd_calc(int32_t argc, char* argv[]);
bool cmd_echo(int32_t argc, char *argv []);
bool cmd_find_addr(int32_t argc, char* argv[]);
bool cmd_help(int32_t argc, char* argv[]);
bool cmd_launch_function(int32_t argc, char* argv[]);
bool cmd_low_level_control(int32_t argc, char* argv[]);
bool cmd_read_memory(int32_t argc, char* argv[]);
bool cmd_repeat(int32_t argc, char* argv[]);
bool cmd_sysinfo(int32_t argc, char* argv[]);
bool cmd_version(int32_t argc, char* argv[]);
bool cmd_wd_test(int32_t argc, char* argv[]);
bool cmd_wd_test_hw(int32_t argc, char* argv[]);
bool cmd_write_memory(int32_t argc, char* argv[]);
bool dump_cmd_result(bool res);
bool dump_cmd_result_ex(bool res, const char* message);
bool reboot(void);
void shell_prompt(void); /*TODO: replace to cli_prompt*/


#define BASE_COMMANDS                                                                                          \
    SHELL_CMD("version", "vi", cmd_version, "Print version information"),                                      \
    SHELL_CMD("help", "h", cmd_help, "Print list of shell commands"),                                          \
    SHELL_CMD("sysinfo", "si", cmd_sysinfo, "Print information about threads & OS"),                           \
    SHELL_CMD("echo","e",cmd_echo, "set echo"),

#define TEST_FIRMWARE_COMMANDS                                                                                         \
    SHELL_CMD("write_mem", "wm", cmd_write_memory, "Write memory address"),                                            \
        SHELL_CMD("read_mem", "rm", cmd_read_memory, "Read memory address"),                                           \
        SHELL_CMD("bit_ctrl", "bc", cmd_low_level_control, "Set Clear bit in memory address"),                         \
        SHELL_CMD("wd_test", "wd_test", cmd_wd_test, "Stop board (for watchdog test)"),                                \
        SHELL_CMD("launch_function", "lfun", cmd_launch_function, "Launch any function by address in ROM"),            \
        SHELL_CMD("repeat", "rpt", cmd_repeat, "Repeat any command N time with period"),                               \
        SHELL_CMD("calc", "calc", cmd_calc, "Calculator of data types"),                                               \
        SHELL_CMD("ascii", "ascii", cmd_ascii, "Print ascii"),                                                         \
        SHELL_CMD("find_addr", "fadr", cmd_find_addr, "Find address by value"),


#define COMMANDS_END SHELL_CMD(NULL, NULL, NULL, NULL)

#ifdef __cplusplus
}
#endif

#endif /* UTILS_SHELL_H */
