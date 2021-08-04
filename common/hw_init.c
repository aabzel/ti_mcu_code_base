#include "hw_init.h"

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/Board.h>
/*mandatory space NoRTOS uses stdint*/
#include <NoRTOS.h>

#include "clocks.h"
#include "sys_tick.h"

#ifdef HAS_FLASH
#include "flash_drv.h"
#endif

#ifdef HAS_RTC
#include "rtc_drv.h"
#endif

#ifdef HAS_I2C
#include "i2c_drv.h"
#endif

#ifdef HAS_SPI
#include "spi_drv.h"
#endif

#ifdef HAS_UART
#include "uart_drv.h"
#endif

#ifdef HAS_GPIO
#include "gpio_drv.h"
#endif

#ifdef HAS_ADC
#include "adc_drv.h"
#endif

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

bool hw_init(void) {
  bool res = true;
  Board_init();
  NoRTOS_start();
  SysTickInit();

#ifdef HAS_WDT
  res = watchdog_init()&&res;
#endif

#ifdef HAS_ADC
  res = adc_init()&&res;
#endif

#ifdef HAS_FLASH
  res = flash_init()&&res;
#endif

#ifdef HAS_RTC
  res = rtc_init()&&res;
#endif

#ifdef HAS_GPIO
  res = gpio_init() && res;
#endif

#ifdef HAS_UART
  res = uart_init()&&res;
#endif

#ifdef HAS_SPI
  res = spi_init()&&res;
#endif

#ifdef HAS_I2C
  res = i2c_init()&&res;
#endif

  return res;
}
