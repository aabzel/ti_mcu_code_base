#include "can_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "tcan4550_drv.h"
#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif
#ifdef HAS_LOG
#include "log.h"
#endif

bool can_send(uint8_t* data, uint16_t len){
    bool res=false;
    bool out_res=true;
    uint16_t i=0, tx_len = 0, rem = 0;
    uint64_t data64;
    if (8<len) {
        for(i=0; i<(len-8); i+=8) {
            memcpy(&data64,&data[i],8);
            res = tcan4550_send_std(0xD3, data64, 8);
            if(false==res){
                out_res=false;
            }
            tx_len += 8;
        }
    }
    rem = len - tx_len;
    if (0<rem) {
          data64 =0;
          memcpy(&data64,&data[tx_len],rem);
          res = tcan4550_send_std(0xD3, data64, rem);
          if(false==res){
              out_res=false;
          }
          tx_len += rem;
    }
    return out_res;
}

bool can_proc_payload(uint8_t* const rx_payload, uint32_t rx_size) {
    bool res = false;

#ifdef HAS_TBFP
    res = tbfp_proc(rx_payload, rx_size, IF_LORA, false);
#ifdef HAS_LOG
    if(false == res) {
        LOG_ERROR(CAN, "CanProcErr");
    }
#endif
#endif /*HAS_TBFP*/
    return res;
}

bool can_proc(void){
    bool res=false;
#ifdef HAS_TBFP
    /*HeartBeat Frame*/
    res = tbfp_send_ping(FRAME_ID_PONG, IF_CAN);
#endif /*HAS_TBFT*/
    return res;
}
