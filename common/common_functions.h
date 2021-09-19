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
    TASK(WDT)                                                                                                          \
    TASK(UART)                                                                                                         \
    TASK_DIAG                                                                                                          \
    TASK_INTEGRITY

bool sys_init(void);
#ifdef NORTOS
void common_loop(uint64_t loop_start_time);
void custom_loop(uint64_t loop_start_time);
void common_main_loop(void);
#endif /*NORTOS*/


#endif /* COMMON_LOOP_H */
