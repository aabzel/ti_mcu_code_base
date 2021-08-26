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
  res = try_init(led_init(),"LED") && res;
#endif /*HAS_LED*/

#ifdef HAS_CLI
  set_log_level(SYS, LOG_LEVEL_DEBUG);
  res = try_init(cli_init(),"CLI") && res;
#endif /*HAS_CLI*/

#ifdef HAS_NMEA
  res = try_init(nmea_init(),"NMEA") && res;
#endif

#ifdef HAS_UBLOX
  res = try_init(ublox_protocol_init(),"UBXProto") && res;
  res = try_init(ubx_driver_init(),"UBXdrv") && res;
#endif /*HAS_UBLOX*/

#ifdef  HAS_HEALTH_MONITOR
  res = try_init(health_monotor_init(),"HM") && res;
#endif /*HAS_HEALTH_MONITOR*/

  return res;
}
