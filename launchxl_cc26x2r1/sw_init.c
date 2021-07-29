#include "sw_init.h"

#include <stdbool.h>
#include <stdint.h>

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

bool sw_init(void) {
  bool res = true;
  
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

  return res;
}
