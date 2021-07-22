#include "common_functions.h"

#include <inttypes.h>

#include "clocks.h"
#include "log.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#endif

#include "task_info.h"
//#include "reboot_info.h"

void common_loop(uint64_t loop_start_time) {
  if (0u == loop_start_time) {
    //return;
  }

#ifdef HAS_CLI
  cli_process();
  //MEASURE_TASK_INTERVAL(UART, 300, cli_process);
#endif
}

void common_main_loop(void) {
  LOG_NOTICE(SYS, "Main Task started, time_ms=%" PRIu32, get_time_ms32());

  for (;;) {
    MAIN_LOOP_START;
    // MEASURE_TASK_INTERVAL(LED, 10000, common_update_leds_state);
    common_loop(loop_start_time);
  }
}
