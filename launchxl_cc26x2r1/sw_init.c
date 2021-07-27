#include "sw_init.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_CLI
#include "cli_manager.h"
#include "log.h"
#endif /*HAS_CLI*/

#ifdef HAS_UBLOX
#include "ubx_protocol.h"
#endif /*HAS_UBLOX*/

bool sw_init(void) {
  bool res = true;
  
#ifdef HAS_CLI
  set_log_level(SYS, LOG_LEVEL_DEBUG);
  cli_init();
#endif /*HAS_CLI*/

#ifdef HAS_UBLOX
  ublox_protocol_init();
#endif /*HAS_UBLOX*/

  return res;
}
