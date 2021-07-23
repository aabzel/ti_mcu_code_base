#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

#include "log.h"
#include "task_info.h"

#ifdef HAS_GPIO
#include "gpio_commands.h"
#else
#define GPIO_COMMANDS
#endif

#ifdef HAS_TIM
#include "tim_commands.h"
#else
#define TIM_COMMANDS
#endif

#ifdef HAS_SPI
#include "spi_commands.h"
#else
#define SPI_COMMANDS
#endif

#ifdef HAS_UNIT_TEST
#include "unit_test_commands.h"
#else
#define UNIT_TEST_COMMANDS
#endif

#ifdef HAS_CLOCK
#include "clock_commands.h"
#else
#define CLOCK_COMMANDS
#endif

#ifdef HAS_UART
#include "uart_commands.h"
#else
#define UART_COMMANDS
#endif

#define COMMON_COMMANDS                                                        \
  DEFAULT_COMMANDS                                                             \
  CLOCK_COMMANDS                                                               \
  GPIO_COMMANDS                                                                \
  TASK_COMMANDS                                                                \
  UART_COMMANDS                                                                \
  UNIT_TEST_COMMANDS                                                           \
  TIM_COMMANDS                                                                 \
  SPI_COMMANDS


#endif /* COMMON_COMMANDS_H */
