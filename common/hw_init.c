#include "hw_init.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef CC26X2
#include <ti/drivers/Board.h>
#ifdef HAS_TEMPERATURE
#include <Temperature.h>
#include <TemperatureCC26X2.h>
#endif
#endif /*CC26X2*/

#include "common_functions.h"
#ifdef NORTOS
/*mandatory space NoRTOS uses stdint*/
#ifdef CC26XX
#include <NoRTOS.h>
#endif /*CC26XX*/
#endif /*NORTOS*/

#include "health_monitor.h"

#ifdef HAS_SYSTIC
#include "sys_tick.h"
#endif

#ifdef HAS_CLOCK
#include "clocks.h"
#endif

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
#endif /*HAS_BLE*/

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif /*HAS_SX1262*/

#ifdef HAS_NEO_6M
#include "neo_6m_drv.h"
#endif

#ifdef HAS_AXP192
#include "axp192_drv.h"
#endif /*HAS_AXP192*/

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif/*HAS_WDT*/

#ifdef HAS_INTERRUPT
#include "interrupt_drv.h"
#endif

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/

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

#ifdef HAS_TEMPERATURE
  Temperature_init();
#endif /*HAS_TEMP*/

#ifdef USE_HAL_DRIVER
   SystemClockConfig();
#endif
#ifdef NORTOS  
#ifdef HAS_SYSTIC
   SysTickInit();
#endif
#endif /*NORTOS*/

#ifdef HAS_INTERRUPT
  res = interrupt_init() && res;
#endif

#ifdef HAS_CLOCK
  res = clock_init() && res;
#endif

#ifdef HAS_GPIO
  res = gpio_init() && res;
#endif /*HAS_GPIO*/

#ifdef HAS_UART
  res = try_init(uart_init(),"UART") && res;
#endif /*HAS_UART*/

#ifdef HAS_RTC
  res = try_init( rtc_init(),"RTC") && res;
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

#ifdef HAS_BLE
  res = try_init(ble_init(),"BLE") && res;
#endif /*HAS_BLE*/


#ifdef HAS_AXP192
  res = try_init(axp192_init(),"axp192") && res;
#endif /*HAS_AXP192*/

#ifdef HAS_FLASH_FS
    res = try_init(flash_fs_init(), "Flash_FS") && res;
#endif /*HAS_FLASH_FS*/

#ifdef HAS_PARAM
    res = try_init(param_init(), "param") && res; //
#endif

#ifdef HAS_SX1262
  res = try_init(sx1262_init(),"sx1262") && res;
#endif /*HAS_SX1262*/

#ifdef HAS_RF
  res = try_init(rf_init(),"RF") && res;
#endif /*HAS_RF*/

#ifdef HAS_BQ25171_Q1
  res = try_init(bq25171_q1_init(),"bq251") && res;
#endif /*HAS_BQ25171_Q1*/

#ifdef HAS_NEO_6M
  res = try_init(neo_6m_init( ),"neo_6m") && res;
#endif
  return res;
}
