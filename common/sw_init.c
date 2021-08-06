#include "sw_init.h"

#include <stdbool.h>
#include <stdint.h>


#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_CLI
#include "cli_manager.h"
#include "log.h"
#endif /*HAS_CLI*/

#ifdef HAS_UBLOX
#include "ubx_protocol.h"
#include "ublox_driver.h"
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif

#ifdef  HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/

bool sw_init(void) {
  bool res = true;
  
#ifdef HAS_LED
  led_init();
#endif /*HAS_LED*/

#ifdef HAS_CLI
  set_log_level(SYS, LOG_LEVEL_DEBUG);
  res = cli_init() && res;
#endif /*HAS_CLI*/

#ifdef HAS_NMEA
  res = nmea_init() && res;
#endif

#ifdef HAS_UBLOX
  res = ublox_protocol_init() && res;
  res = ubx_driver_init() && res;
#endif /*HAS_UBLOX*/

#ifdef  HAS_HEALTH_MONITOR
  res = health_monotor_init() && res;
#endif /*HAS_HEALTH_MONITOR*/

  return res;
}
