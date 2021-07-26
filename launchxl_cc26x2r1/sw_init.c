#include "sw_init.h"

#include <stdbool.h>
#include <stdint.h>


#ifdef HAS_CLI
#include "cli_manager.h"
#endif

#include "log.h"


bool sw_init(void) {
  bool res = true;
  
#ifdef HAS_CLI
  set_log_level(SYS, LOG_LEVEL_DEBUG);
  cli_init();
#endif /*HAS_CLI*/


  return res;
}
