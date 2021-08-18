#include "clock_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <systick.h>
#include <osc.h>

#include "clocks.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "diag_report.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "base_cmd.h"
#include "str_utils.h"
#include "sys_tick.h"

bool systick_set_command(int32_t argc, char* argv[]){
    bool res = false;
    if (1 == argc) {
          res = true;
          uint32_t period24bit = 0;

          if (true == res) {
              res = try_str2uint32(argv[0], &period24bit);
              if (false == res) {
                  LOG_ERROR(SYS, "Unable to extract period %s", argv[0]);
              }
          }

          if (true == res) {
              SysTickPeriodSet(period24bit);
          }
      } else {
          LOG_ERROR(SYS, "Usage: sts period24bit");
      }
    return res;
}

bool clock_diag_command(int32_t argc, char* argv[]){
    bool res = false;
     if (0 == argc) {
         res = true;
         uint32_t millivolt= OSCHF_DebugGetCrystalAmplitude(  );
         io_printf("CrystalAmpl: %u mV" CRLF, millivolt);
         io_printf("up_time_ms %u" CRLF, g_up_time_ms);
         io_printf("SysTickPeriod %u" CRLF, SysTickPeriodGet());
         io_printf("SysTickValue %u" CRLF, SysTickValueGet());

     } else {
         LOG_ERROR(SYS, "Usage: cld");
     }
     return res;
}


