#include "hw_init.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef CC26X2
#include <ti/drivers/Board.h>
#include <Temperature.h>
#include <TemperatureCC26X2.h>
#endif /*CC26X2*/

#ifdef NORTOS
/*mandatory space NoRTOS uses stdint*/
#ifdef CC26XX
#include <NoRTOS.h>
#endif /*CC26XX*/
#include "common_functions.h"
#endif /*NORTOS*/

#include "health_monitor.h"
#include "clocks.h"
#include "sys_tick.h"

#ifdef HAS_ADC
#include "adc_drv.h"
#endif

#ifdef HAS_FLASH
#include "flash_drv.h"
#endif

#ifdef HAS_FLASH_NVS
#include "flash_nvs_drv.h"
#endif

#ifdef HAS_TCAN4550
#include "TCAN_hl.h"
//#include "tcan4550_drv.h"
#endif /*HAS_TCAN4550*/

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

#ifdef HAS_BQ25171_Q1
#include "bq25171_q1_drv.h"
#endif /*HAS_BQ25171_Q1*/

#ifdef HAS_RS232
#include "rs232_drv.h"
#endif /*HAS_RS232*/

#ifdef HAS_DAC
#include "dac_drv.h"
#endif

#ifdef HAS_RF
#include "rf_drv.h"
#endif

#ifdef HAS_RNG
#include "rng_drv.h"
#endif

#ifdef HAS_RTC
#include "rtc_drv.h"
#endif /*HAS_RTC*/


#ifdef HAS_BLE
#include "ble_drv.h"
#endif /*HAS_BLE*/

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif /*HAS_SX1262*/

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif/*HAS_WDT*/

bool hw_init(void) {
  bool res = true;
#ifdef CC26XX
  Board_init();
#endif /*CC26X2*/

#ifdef NORTOS
  #ifdef CC26XX
  NoRTOS_start();
  #endif /*CC26X2*/
#endif /*NORTOS*/

#ifdef HAS_TEMP
  Temperature_init();
#endif /*HAS_TEMP*/

#ifdef USE_HAL_DRIVER
   SystemClockConfig();
#endif
#ifdef NORTOS  
   SysTickInit();
#endif /*NORTOS*/

#ifdef HAS_GPIO
  res = gpio_init() && res;
#endif /*HAS_GPIO*/


#ifdef HAS_UART
  res = try_init(uart_init(),"UART") && res;
#endif /*HAS_UART*/

#ifdef HAS_RTC
  res = try_init(rtc_init(),"RTC") && res;
#endif /*HAS_RTC*/

#ifdef HAS_RS232
  res = try_init(rs232_init(),"RS232") && res;
#endif /*HAS_UART*/

#ifdef HAS_WDT
  res = try_init(watchdog_init(),"WDT") && res;
#endif /*HAS_WDT*/

#ifdef HAS_TIM
  res = try_init(tim_init(),"TIM") && res;
#endif /*HAS_TIM*/

#ifdef HAS_FLASH
  res = try_init(flash_init(),"Flash") && res;
#endif /*HAS_FLASH*/

#ifdef HAS_FLASH_NVS
  res = try_init(flash_nvs_init(),"Flash_nvs") && res;
#endif /*HAS_FLASH*/

#ifdef HAS_ADC
  res = try_init(adc_init(),"ADC") && res;
#endif /*HAS_ADC*/

#ifdef HAS_DAC
  res = try_init(dac_init(),"DAC") && res;
#endif /*HAS_DAC*/

#ifdef HAS_SPI
  res = try_init(spi_init(),"SPI") && res;
#endif /*HAS_SPI*/

#ifdef HAS_TCAN4550
  res = try_init(init_tcan(),"tcan4550") && res;
  //res = try_init(tcan4550_init(),"tcan4550") && res;
#endif /*HAS_TCAN4550*/

#ifdef HAS_I2C
  res = try_init(i2c_init(),"I2C") && res;
#endif /*HAS_I2C*/

#ifdef HAS_RNG
  res = try_init( rng_init(),"RNG") && res;
#endif /*HAS_RNG*/

#ifdef HAS_RTC
  res = try_init( rtc_init(),"RTC") && res;
#endif /*HAS_RTC*/

#ifdef HAS_BLE
  res = try_init(ble_init(),"BLE") && res;
#endif /*HAS_BLE*/

#ifdef HAS_SX1262
  res = try_init(sx1262_init(),"sx1262") && res;
#endif /*HAS_SX1262*/

#ifdef HAS_RF
  res = try_init(rf_init(),"RF") && res;
#endif /*HAS_RF*/

#ifdef HAS_BQ25171_Q1
  res = try_init(bq25171_q1_init(),"bq251") && res;
#endif /*HAS_BQ25171_Q1*/

  return res;
}
