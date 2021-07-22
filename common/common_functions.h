#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_UART
#define TASK_UART TASK(UART)
#else
#define TASK_UART
#endif

#define COMMON_TASKS                                                           \
  TASK(LED)                                                                    \
  TASK(WDT)                                                                    \
  TASK_UART                                                                    \
  TASK_DIAG                                                                    \
  TASK_INTEGRITY

void common_loop(uint64_t loop_start_time);
void custom_loop(uint64_t loop_start_time);
void common_init(void);
void common_update_leds_state(void);
void common_main_loop(void);
bool wait_with_common_loop(uint64_t const waitMs);
bool wait_ms(uint64_t const waitMs);

#endif /* COMMON_FUNCTIONS_H */
