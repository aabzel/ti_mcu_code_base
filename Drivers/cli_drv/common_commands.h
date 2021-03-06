#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

#include "log.h"

#ifdef HAS_ADC_COMMANDS
#include "adc_commands.h"
#else
#define ADC_COMMANDS
#endif

#ifdef NORTOS
#include "task_commands.h"
#else
#define TASK_COMMANDS
#endif /*NORTOS*/

#ifdef HAS_BOOT_DIAG
#include "boot_commands.h"
#else
#define BOOT_COMMANDS
#endif

#ifdef HAS_GPIO_COMMANDS
#include "gpio_commands.h"
#else
#define GPIO_COMMANDS
#endif

#ifdef HAS_GPIO_PWM
#include "gpio_pwm_commands.h"
#else
#define GPIO_PWM_COMMANDS
#endif

#ifdef HAS_CLOCK_COMMANDS
#include "clock_commands.h"
#else
#define CLOCK_COMMANDS
#endif

#ifdef HAS_INTERRUPT
#include "interrupt_commands.h"
#else
#define INTERRUPT_COMMANDS
#endif

#ifdef HAS_TIM_COMMANDS
#include "tim_commands.h"
#else
#define TIM_COMMANDS
#endif

#ifdef HAS_TCAN4550_COMMANDS
#include "tcan4550_commands.h"
#include "tcan4550_ti_commands.h"
#else
#define TCAN4550_COMMANDS
#define TCAN4550_TI_TI_COMMANDS
#endif

#ifdef HAS_RTC_COMMANDS
#include "rtc_commands.h"
#else
#define RTC_COMMANDS
#endif

#ifdef HAS_SPI_COMMANDS
#include "spi_commands.h"
#else
#define SPI_COMMANDS
#endif

#ifdef HAS_UNIT_TEST
#include "unit_test_commands.h"
#else
#define UNIT_TEST_COMMANDS
#endif

#ifdef HAS_FLASH_COMMANDS
#include "flash_commands.h"
#else
#define FLASH_COMMANDS
#endif

#ifdef HAS_UART_COMMANDS
#include "uart_commands.h"
#else
#define UART_COMMANDS
#endif
#include "uart_drv.h"

#ifdef HAS_DAC
#include "dac_commands.h"
#else
#define DAC_COMMANDS
#endif

#ifdef HAS_I2C_COMMANDS
#include "i2c_commands.h"
#else
#define I2C_COMMANDS
#endif

#ifdef HAS_WDT_COMMANDS
#include "watchdog_commands.h"
#else
#define WDT_COMMANDS
#endif

#define COMMON_COMMANDS                                                                                                \
    ADC_COMMANDS                                                                                                       \
    BASE_COMMANDS                                                                                                      \
    BOOT_COMMANDS                                                                                                      \
    CLOCK_COMMANDS                                                                                                     \
    DAC_COMMANDS                                                                                                       \
    FLASH_COMMANDS                                                                                                     \
    GPIO_COMMANDS                                                                                                      \
    GPIO_PWM_COMMANDS                                                                                                  \
    I2C_COMMANDS                                                                                                       \
    INTERRUPT_COMMANDS                                                                                                 \
    RTC_COMMANDS                                                                                                       \
    SPI_COMMANDS                                                                                                       \
    TASK_COMMANDS                                                                                                      \
    TCAN4550_COMMANDS                                                                                                  \
    TCAN4550_TI_TI_COMMANDS                                                                                            \
    TEST_FIRMWARE_COMMANDS                                                                                             \
    TIM_COMMANDS                                                                                                       \
    UART_COMMANDS                                                                                                      \
    UNIT_TEST_COMMANDS                                                                                                 \
    WDT_COMMANDS

#endif /* COMMON_COMMANDS_H */
