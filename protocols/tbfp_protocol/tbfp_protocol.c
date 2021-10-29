
#include "tbfp_protocol.h"

#include <string.h>
#include <time.h>

#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "byte_utils.h"
#include "tbfp_diag.h"
#include "float_utils.h"
#include "crc8_sae_j1850.h"
#include "zed_f9p_drv.h"
#include "core_driver.h"
#include "data_utils.h"
#include "debug_info.h"
#include "gnss_utils.h"
#include "io_utils.h"
#include "log.h"

TbfpPorotocol_t TbfpPorotocol={0};

bool tbfp_protocol_init(TbfpPorotocol_t* instance){
    bool res = false;
    if(instance){
        memset(instance, 0x0, sizeof(TbfpPorotocol_t));
        res = true;
    }
    return res;
}


bool is_tbfp_protocol(uint8_t* arr, uint16_t len){
    bool res = false;
    TbfHeader_t header = {0} ;
    memcpy(&header, arr, sizeof(TbfHeader_t));
    if (TBFP_PREAMBLE==header.preamble &&(header.len<len)) {
        res = true;
    } else {
        res = false;
    }
    if (res) {
        uint32_t frame_len = TBFP_HEADER_SIZE+header.len;
        uint8_t read_crc8=arr[frame_len];
        uint8_t calc_crc8=0;
        calc_crc8= crc8_sae_j1850_calc(arr,  frame_len);
        if(calc_crc8==read_crc8){
            res = true;
        }else{
            res = false;
        }
    }

    return res;
}

bool tbfp_compose_ping(uint8_t* out_frame,
                       uint32_t* tx_frame_len,
                       TbfPingFrame_t *pingFrame){
    bool res = false;
    if(out_frame && tx_frame_len){
        out_frame[0]=TBFP_PREAMBLE;
        out_frame[1]=sizeof(TbfPingFrame_t);
        memcpy(&out_frame[2], pingFrame, sizeof(TbfPingFrame_t));
        uint32_t frame_len = sizeof(TbfPingFrame_t)+TBFP_HEADER_SIZE;
        out_frame[frame_len]= crc8_sae_j1850_calc(out_frame,  frame_len);
        *tx_frame_len = frame_len+TBFP_SIZE_CRC;
        res = true;
    }
    return res;
}

bool tbfp_send_ping(uint8_t frame_id){
    bool res = false;
        uint8_t frame[256] = "";
        uint32_t tx_frame_len=0;
        TbfPingFrame_t pingFrame={0};
        pingFrame.id = frame_id;
        pingFrame.mac = get_ble_mac();
        pingFrame.time_stamp = mktime(&ZedF9P.time_date);
        pingFrame.coordinate = ZedF9P.coordinate_cur;
        res = tbfp_compose_ping(frame, &tx_frame_len, &pingFrame);
        if (res) {
            res = lora_send_queue(frame, tx_frame_len);
        }
    return res;
}

bool tbfp_proc_ping(uint8_t* ping_payload, uint16_t len){
    bool res = false;
    if(NULL!=ping_payload){
        TbfPingFrame_t pingFrame={0};
        memcpy((void*)&pingFrame,(void*) ping_payload, sizeof(TbfPingFrame_t));

        tbfp_print_ping_frame(&pingFrame);

        if(FRAME_ID_PING==pingFrame.id){
            res = tbfp_send_ping(FRAME_ID_PONG);
        }
        double cur_dist = gnss_calc_distance_m(ZedF9P.coordinate_cur, pingFrame.coordinate);
        LOG_INFO(LORA, "link distance %f m", cur_dist);
#ifdef HAS_LORA
        LoRaInterface.max_distance = double_max(LoRaInterface.max_distance, cur_dist);
#endif

    }
    return res;
}

bool tbfp_proc_payload(uint8_t* payload, uint16_t len){
    bool res = false;
    switch(payload[0]){
    case FRAME_ID_PONG:
    case FRAME_ID_PING:
        res = tbfp_proc_ping(payload, len);
        break;
    default  :
        res = false;
        break;

    }
    return res;
}

bool tbfp_proc(uint8_t* arr, uint16_t len){
    bool res = false;
    res = is_tbfp_protocol(arr, len);
    if(res){
        TbfHeader_t header = {0};
        memcpy(&header, arr, sizeof(TbfHeader_t));
        res = tbfp_proc_payload(&arr[TBFP_PAYLOAD_INDEX], header.len) ;
    }
    return res;
}
