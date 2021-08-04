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

#ifdef HAS_RTC
#include "rtc_commands.h"
#else
#define RTC_COMMANDS
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

#ifdef HAS_FLASH
#include "flash_commands.h"
#else
#define FLASH_COMMANDS
#endif

#ifdef HAS_UART
#include "uart_commands.h"
#else
#define UART_COMMANDS
#endif

#ifdef HAS_ADC
#include "adc_commands.h"
#else
#define ADC_COMMANDS
#endif

#ifdef HAS_I2C
#include "i2c_commands.h"
#else
#define I2C_COMMANDS
#endif

#ifdef HAS_WDT
#include "watchdog_commands.h"
#else
#define WDT_COMMANDS
#endif

#define COMMON_COMMANDS                                                                                                \
    ADC_COMMANDS                                                                                                       \
    CLOCK_COMMANDS                                                                                                     \
    DEFAULT_COMMANDS                                                                                                   \
    FLASH_COMMANDS                                                                                                     \
    I2C_COMMANDS                                                                                                       \
    GPIO_COMMANDS                                                                                                      \
    RTC_COMMANDS                                                                                                       \
    SPI_COMMANDS                                                                                                       \
    TASK_COMMANDS                                                                                                      \
    TIM_COMMANDS                                                                                                       \
    UART_COMMANDS                                                                                                      \
    UNIT_TEST_COMMANDS                                                                                                 \
    WDT_COMMANDS

#endif /* COMMON_COMMANDS_H */
