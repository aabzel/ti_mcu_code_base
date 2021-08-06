#include "health_monitor_command.h"

#include "health_monitor.h"

#include "io_utils.h"
#include "log.h"
#include "table_utils.h"

bool health_monitor_command(int32_t argc, char *argv[]) {
  io_printf(TSEP " init error " TSEP " %u " CRLF, HealthMon.init_error);
  io_printf(TSEP " crc error code " TSEP " %u " CRLF, HealthMon.crc_error_cnt);
  io_printf(TSEP " error cnt      " TSEP " %u " CRLF, HealthMon.error_cnt);
  return true;
}
