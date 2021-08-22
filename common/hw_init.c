#include "hw_init.h"

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/Board.h>
#include <Temperature.h>
#include <TemperatureCC26X2.h>

#ifdef NORTOS
/*mandatory space NoRTOS uses stdint*/
#include <NoRTOS.h>
#endif /*NORTOS*/

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

#ifdef HAS_TIM
#include "tim_drv.h"
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

#ifdef HAS_DAC
#include "dac_drv.h"
#endif

#ifdef HAS_RF
#include "rf_drv.h"
#endif

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

#ifdef HAS_BLE
#include "ble_drv.h"
#endif

bool hw_init(void) {
  bool res = true;
  Board_init();
#ifdef NORTOS
  NoRTOS_start();
#endif /*NORTOS*/

  Temperature_init();
  SysTickInit();

#ifdef HAS_WDT
  res = watchdog_init()&&res;
#endif

#ifdef HAS_TIM
  res = tim_init()&&res;
#endif

#ifdef HAS_ADC
  res = adc_init()&&res;
#endif

#ifdef HAS_DAC
  res = dac_init() && res;
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

#ifdef HAS_RF
  res = rf_init() &&res;
#endif

  #ifdef HAS_BLE
  res = ble_init()&&res;
#endif

  return res;
}
