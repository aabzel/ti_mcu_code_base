
#include "tbfp_diag.h"

#include <string.h>
#include <time.h>
#include <inttypes.h>

#include "byte_utils.h"
#include "data_utils.h"
#include "debug_info.h"
#include "time_diag.h"
#include "time_utils.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "tbfp_protocol.h"


char *tbfp_id2str(uint8_t id){
   char *name="?";
   switch(id){
      case  FRAME_ID_ACK: name="Ack"; break;
      case  FRAME_ID_CHAT: name="Chat"; break;
      case  FRAME_ID_CMD: name="Cmd"; break;
      case  FRAME_ID_PING: name="Ping"; break;
      case  FRAME_ID_TUNNEL: name="Tunnel"; break;
      case  FRAME_ID_PONG: name="Pong"; break;
      case  FRAME_ID_RTCM3: name="RTCM3"; break;
      default: name="??"; break;
   }
   return name;
}

bool tbfp_print_ping_frame(TbfPingFrame_t *pingFrame){
    bool res = true;
    if (pingFrame) {
        io_printf("MAC: 0x%" PRIX64 CRLF, pingFrame->mac);
        struct tm * time_date =  gmtime(&pingFrame->time_stamp);
        if (time_date) {
            res = is_valid_time_date(time_date);
            if(res){
                res =  print_time_date(time_date )&&res;
            }
        } else {
            res = false;
        }
#ifdef HAS_GNSS
        res = print_coordinate(pingFrame->coordinate, true)&&res;
#endif
    }else{
        res = false;
    }
    return res;
}
