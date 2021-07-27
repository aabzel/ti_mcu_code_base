#include "common_functions.h"

#include <inttypes.h>

#include "clocks.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#endif
#include "io_utils.h"
#include "log.h"
#include "task_config.h"
#include "task_info.h"
#include "uart_drv.h"

#ifdef HAS_UBLOX
#include "ubx_protocol.h"
#include "ublox_driver.h"
#endif /*HAS_UBLOX*/

void common_loop(uint64_t loop_start_time) {
  if (0u == loop_start_time) {
    return;
  }
  MEASURE_TASK_INTERVAL(UART1, 1, proc_uart1);
#ifdef HAS_UBLOX
  MEASURE_TASK_INTERVAL(UBX, 1, ubx_proc_frame);
#endif /*HAS_UBLOX*/

#ifdef HAS_CLI
  MEASURE_TASK_INTERVAL(CLI, 30, cli_process);
#endif
}

uint32_t g_iteration_cnt = 10;
void common_main_loop(void) {
  io_printf( "Main Task started, time_ms=%u" CRLF, get_time_ms32());

  for (;;) {
    g_iteration_cnt++;
    MAIN_LOOP_START;
    // MEASURE_TASK_INTERVAL(LED, 10000, common_update_leds_state);
    common_loop(loop_start_time);
  }
}
