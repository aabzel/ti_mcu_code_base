#include "tbfp_protocol.h"

#include <string.h>
#include <time.h>

#include "byte_utils.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#endif

#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#ifdef HAS_LOG
#include "log.h"
#include "io_utils.h"
#endif
#ifdef HAS_MCU
#include "core_driver.h"
#endif

#include "crc8_sae_j1850.h"
#include "data_utils.h"
#include "debug_info.h"

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#include "float_utils.h"
#include "gnss_utils.h"


#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/
#ifdef HAS_RS232
#include "rs232_drv.h"
#endif
#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif
#include "system.h"
#include "tbfp_diag.h"
#ifdef HAS_CLI
#include "writer_config.h"
#endif
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

#ifdef X86_64
#include <stdio.h>
#include "log.h"
#endif


TbfpProtocol_t TbfpProtocol[IF_CNT] = {0};

bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface) {
    bool res = false;
    if(instance) {
        memset(instance, 0x0, sizeof(TbfpProtocol_t));
        instance->interface = interface;
#ifdef HAS_TBFP_FLOW_CONTROL
        instance->prev_s_num = 0xFFFF;
#endif
        instance->rx_pkt_cnt = 0;
        res = true;
    }
    if (TBFP_SIZE_HEADER !=sizeof(TbfHeader_t)){
        LOG_ERROR(TBFP, "HeaderLenErr");
        res = false;
    }
    return res;
}

bool is_tbfp_protocol(uint8_t* arr, uint16_t len) {
    bool res = false;
#ifdef X86_64
    printf("\n%s(): len: %u", __FUNCTION__, len);
#endif
    TbfHeader_t header = {0};
    memcpy(&header, arr, sizeof(TbfHeader_t));
    if((TBFP_PREAMBLE == header.preamble) && (header.len < len)) {
        res = true;
    } else {
        res = false;
        LOG_ERROR(TBFP, "FlameErr");
    }
    if(res) {
        uint32_t frame_len = TBFP_SIZE_HEADER + header.len;
        uint8_t read_crc8 = arr[frame_len];
        res = crc8_sae_j1850_check(arr, frame_len, read_crc8);
        if(false == res) {
            LOG_ERROR(TBFP, "CrcErr Read:0x%02x",read_crc8);
        }
    }

    return res;
}

static bool tbfp_make_header(uint8_t* out_array, uint32_t payload_len, Interfaces_t interface, uint8_t lifetime) {
    bool res = false;
    if(payload_len < TBFP_MAX_PAYLOAD){
        if(out_array ) {
            TbfHeader_t header;
            header.preamble = TBFP_PREAMBLE;
#ifdef HAS_TBFP_FLOW_CONTROL
            header.snum = TbfpProtocol[interface].s_num;
            TbfpProtocol[interface].s_num++;
#endif /*HAS_TBFP_FLOW_CONTROL*/
#ifdef HAS_TBFP_RETRANSMIT
            header.lifetime = lifetime;
#endif
            TbfpProtocol[interface].tx_pkt_cnt++;
            header.len = (uint8_t)payload_len;
            memcpy(out_array, &header, sizeof(TbfHeader_t));
            res = true;
        } else {
            LOG_ERROR(TBFP, "NullPayLoad");
        }

    }else{
        LOG_ERROR(TBFP, "TooBigPayload %u (Lim: %u)",payload_len, TBFP_MAX_PAYLOAD);
    }
    return res;
}

bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame, Interfaces_t interface) {
    bool res = false;
    if(out_frame && tx_frame_len) {
        res = tbfp_make_header(out_frame, sizeof(TbfPingFrame_t), interface, 0);
        if(res) {
            memcpy(&out_frame[TBFP_INDEX_PAYLOAD], pingFrame, sizeof(TbfPingFrame_t));
            uint32_t frame_len = sizeof(TbfPingFrame_t) + TBFP_SIZE_HEADER;
            out_frame[frame_len] = crc8_sae_j1850_calc(out_frame, frame_len);
            *tx_frame_len = frame_len + TBFP_SIZE_CRC;
            res = true;
        }
    }
    return res;
}

bool tbfp_send(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime) {
    bool res = false;
    if(tx_array && (0 < len)) {
        uint8_t frame[256] = "";
        uint32_t frame_len = TBFP_SIZE_HEADER + len;
        res = tbfp_make_header(frame, len, interface, lifetime);
        if(res) {
            memcpy(&frame[TBFP_INDEX_PAYLOAD], tx_array, len);
            frame[frame_len] = crc8_sae_j1850_calc(frame, frame_len);
            res = sys_send_if(frame,   frame_len + TBFP_SIZE_CRC,  interface);
        }
    }
    return res;
}


static bool tbfp_send_text(uint8_t payload_id, uint8_t* tx_array, uint32_t len, Interfaces_t interface,
                           uint8_t lifetime) {
    bool res = false;
    if(tx_array && (0 < len)) {
        uint8_t frame[256] = "";
        uint32_t frame_len = TBFP_SIZE_HEADER + TBFP_SIZE_ID + len;
        res = tbfp_make_header(frame, len + TBFP_SIZE_ID, interface, lifetime);
        if(res) {
            frame[TBFP_INDEX_PAYLOAD] = payload_id;
            memcpy(&frame[TBFP_INDEX_PAYLOAD + 1], tx_array, len);
            frame[frame_len] = crc8_sae_j1850_calc(frame, frame_len);
            res = sys_send_if(frame, frame_len + TBFP_SIZE_CRC,  interface);

        }
    }
    return res;
}

bool tbfp_send_chat(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CHAT, tx_array, len, interface, lifetime);
    return res;
}

bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CMD, tx_array, len, interface, 0);
    return res;
}

bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface) {
    bool res = false;
    uint8_t frame[256] = "";
    memset(frame, 0, sizeof(frame));
    uint32_t tx_frame_len = 0;
    TbfPingFrame_t pingFrame = {0};
    pingFrame.id = frame_id;
#ifdef CC26XX
    pingFrame.mac = get_ble_mac();
#endif
    pingFrame.coordinate.latitude = 999999.0;
    pingFrame.coordinate.longitude = 9999.0;
#ifdef HAS_ZED_F9P
    pingFrame.time_stamp = mktime(&ZedF9P.time_date);

    res = is_valid_gnss_coordinates(ZedF9P.coordinate_cur);
    if(res) {
        pingFrame.coordinate = ZedF9P.coordinate_cur;
    } else {
        /*invalid coordinate*/
        pingFrame.coordinate.latitude = 360.0;
        pingFrame.coordinate.longitude = 360.0;
    }

#endif
    res = tbfp_compose_ping(frame, &tx_frame_len, &pingFrame, interface);
    if(res) {
        res = sys_send_if(frame, tx_frame_len,  interface);
    }
    return res;
}

static bool tbfp_proc_ping(uint8_t* ping_payload, uint16_t len, Interfaces_t interface) {
    bool res = false;
    if(NULL != ping_payload) {
        TbfPingFrame_t pingFrame = {0};
        memcpy((void*)&pingFrame, (void*)ping_payload, sizeof(TbfPingFrame_t));
#ifdef HAS_LOG
        tbfp_print_ping_frame(&pingFrame);
#endif /*HAS_MCU*/
        if(FRAME_ID_PING == pingFrame.id) {
            res = tbfp_send_ping(FRAME_ID_PONG, interface);
        }
#ifdef HAS_ZED_F9P
        double cur_dist = 0.0;
        double azimuth = 0.0;
        if(is_valid_gnss_coordinates(pingFrame.coordinate)) {
            if(is_valid_gnss_coordinates(ZedF9P.coordinate_cur)) {
                cur_dist = gnss_calc_distance_m(ZedF9P.coordinate_cur, pingFrame.coordinate);
                azimuth = gnss_calc_azimuth_deg(ZedF9P.coordinate_cur, pingFrame.coordinate);
#ifdef HAS_LOG
                LOG_INFO(TBFP, "LinkDistance %3.3f m %4.1f deg", cur_dist, azimuth);
#endif
            } else {
#ifdef HAS_LOG
                LOG_ERROR(TBFP, "InvalidLocalGNSSDot");
#endif
            }
        } else {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "InvalidRemoteGNSSDot");
#endif
        }
#endif /*HAS_ZED_F9P*/

#if defined(HAS_LORA) && defined(HAS_ZED_F9P)
        uint16_t file_len = 0;
#if defined(HAS_PARAM) && defined(HAS_FLASH_FS)
        res = mm_get(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, sizeof(double), &file_len);
#endif /*HAS_PARAM && HAS_FLASH_FS*/
        if((LoRaInterface.max_distance < cur_dist) && res) {
#if defined(HAS_PARAM) && defined(HAS_FLASH_FS)
            res = mm_set(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&cur_dist, sizeof(double));
            if(false == res) {
#ifdef HAS_LOG
                LOG_ERROR(TBFP, "UpdateMaxDist");
#endif
            }
#endif /*HAS_PARAM && HAS_FLASH_FS*/
            LoRaInterface.max_distance = cur_dist;
        }

#endif /*HAS_LORA HAS_ZED_F9P*/
    }
    return res;
}

static bool tbfp_proc_chat(uint8_t* payload, uint16_t len) {
    bool res = false;
    if((NULL != payload) && (0 < len)) {
        res = true;
#ifdef HAS_LOG
        res = print_mem(&payload[1], len - 1, false, true, true, true);
#endif
    }
    return res;
}

static bool tbfp_proc_cmd(uint8_t* payload, uint16_t len) {
    bool res = false;
    if((NULL != payload) && (0 < len) && (FRAME_ID_CMD == payload[0])) {
        res = false;
#if defined(HAS_CLI) && defined(HAS_LORA)
        curWriterPtr = &dbg_lora_o;
        payload[len] = 0x00;
        res = process_shell_cmd((char*)&payload[1]);
        curWriterPtr = &dbg_o;
#endif
    }
    return res;
}

bool tbfp_parser_reset_rx(TbfpProtocol_t* instance) {
    bool res = false;
    if(instance) {
        instance->parser.rx_state = WAIT_PREAMBLE;
        instance->parser.load_len = 0;
        res = true;
    }
    return res;
}

static bool tbfp_proc_payload(uint8_t* payload, uint16_t len, Interfaces_t interface) {
    bool res = false;
#ifdef X86_64
    LOG_DEBUG(TBFP,"%s():", __FUNCTION__);
#endif
    switch(payload[0]) {
#ifdef HAS_RTCM3
    case FRAME_ID_RTCM3:
        LOG_DEBUG(TBFP,"RTCMpayload");
        res = rtcm3_proc_array(payload, len, interface);
        break;
#endif /*HAS_RTCM3*/
    case FRAME_ID_CHAT:
        LOG_DEBUG(TBFP,"ChatPayload");
        res = tbfp_proc_chat(payload, len);
        break;
    case FRAME_ID_PONG:
    case FRAME_ID_PING:
        LOG_DEBUG(TBFP,"PingPayload");
        res = tbfp_proc_ping(payload, len, interface);
        break;
    case FRAME_ID_CMD:
        LOG_DEBUG(TBFP,"CmdPayload");
        res = tbfp_proc_cmd(payload, len);
        break;
    default:
        LOG_ERROR(TBFP,"UndefPayload ID: 0x%02x",payload[0] );
        res = false;
        break;
    }
    return res;
}

/*One LoRa frame can contain several TBFP frames*/
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface, bool is_reset) {
    bool res = true;
    uint32_t cur_rx_prk = 0;
    uint32_t init_rx_prk = TbfpProtocol[interface].rx_pkt_cnt;
    if(is_reset){
        res = tbfp_parser_reset_rx(&TbfpProtocol[interface]);
    }
    uint32_t i = 0, ok_cnt = 0, err_cnt = 0;
    for(i = 0; i < len; i++) {
        res = tbfp_proc_byte(&TbfpProtocol[interface], arr[i]);
        if(res) {
            ok_cnt++;
        } else {
            err_cnt++;
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "i=%u", i);
#endif
        }
    }
    cur_rx_prk = TbfpProtocol[interface].rx_pkt_cnt - init_rx_prk;
    if(0 < cur_rx_prk) {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "InPktCnt:%u in %u byte", cur_rx_prk, len);
#endif
    } else {
#ifdef HAS_DEBUG
        if(true == TbfpProtocol[interface].debug) {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "LackPkt:%u", len);
#endif
            print_mem(arr, len, true, false, true, true);
        }
#endif
    }
    if(len == ok_cnt) {
        res = true;
    } else {
        res = false;
    }

    return res;
}

bool tbfp_proc_full(uint8_t* arr, uint16_t len, Interfaces_t interface) {
    bool res = true;
#ifdef X86_64
    LOG_DEBUG(TBFP,"%s():", __FUNCTION__);
#endif
    res = is_tbfp_protocol(arr, len);
    if(res) {
        TbfHeader_t inHeader = {0};
        memcpy(&inHeader, arr, sizeof(TbfHeader_t));
#ifdef HAS_TBFP_FLOW_CONTROL

        LOG_DEBUG(TBFP,"prev_snum:%u snum:%u flow:%u", TbfpProtocol[interface].prev_s_num,
               inHeader.snum, TbfpProtocol[interface].con_flow);

        if((TbfpProtocol[interface].prev_s_num + 1) == inHeader.snum) {
            /*Flow ok*/
            TbfpProtocol[interface].con_flow++;
            TbfpProtocol[interface].max_con_flow =
                max16(TbfpProtocol[interface].max_con_flow, TbfpProtocol[interface].con_flow);
        } else if((TbfpProtocol[interface].prev_s_num + 1) < inHeader.snum) {
#ifdef HAS_LOG
            uint32_t lost_frame_cnt = inHeader.snum - TbfpProtocol[interface].prev_s_num - 1;
            if((TbfpProtocol[interface].prev_s_num + 1) == (inHeader.snum - 1)) {
                LOG_WARNING(TBFP, "Lost %u=%u Flow:%u", inHeader.snum - 1, lost_frame_cnt,
                            TbfpProtocol[interface].con_flow);
            } else {
                LOG_WARNING(TBFP, "Lost %u-%u=%u Flow:%u", TbfpProtocol[interface].prev_s_num + 1, inHeader.snum - 1,
                            lost_frame_cnt, TbfpProtocol[interface].con_flow);
            }
#endif
            TbfpProtocol[interface].con_flow = 1;
        } else if(TbfpProtocol[interface].prev_s_num < inHeader.snum) {
            /*Unreal situation*/
            TbfpProtocol[interface].con_flow = 1;
            TbfpProtocol[interface].err_cnt++;
        } else if(TbfpProtocol[interface].prev_s_num == inHeader.snum) {
            TbfpProtocol[interface].err_cnt++;
        } else {
            /*Unreal situation*/
            TbfpProtocol[interface].con_flow = 1;
            TbfpProtocol[interface].err_cnt++;
        }
        TbfpProtocol[interface].prev_s_num = inHeader.snum;

        LOG_DEBUG(TBFP,"prev_snum:%u snum:%u flow:%u", TbfpProtocol[interface].prev_s_num,
               inHeader.snum, TbfpProtocol[interface].con_flow);

#endif /*HAS_TBFP_FLOW_CONTROL*/

#ifdef HAS_TBFP_RETRANSMIT
        if(inHeader.lifetime) {
            res = tbfp_send(&arr[TBFP_INDEX_PAYLOAD], inHeader.len, interface, inHeader.lifetime - 1);
        }
#endif /*HAS_TBFP_RETRANSMIT*/
        res = tbfp_proc_payload(&arr[TBFP_INDEX_PAYLOAD], inHeader.len, interface);
    }

    return res;
}
