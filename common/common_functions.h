#ifndef COMMON_LOOP_H
#define COMMON_LOOP_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_UART
#define TASK_UART TASK(UART)
#else
#define TASK_UART
#endif

#define COMMON_TASKS                                                                                                   \
    TASK(LED)                                                                                                          \
    TASK(UART)                                                                                                         \
    TASK(WDT)                                                                                                          \
    TASK_DIAG                                                                                                          \
    TASK_INTEGRITY

bool sys_init(void);
bool try_init(bool status, char* message);
#ifdef HAS_SUPER_LOOP
void super_loop(uint64_t loop_start_time);
void custom_loop(uint64_t loop_start_time);
void super_main_loop(void);
#endif /*SUPER_LOOP*/

#endif /* COMMON_LOOP_H */
