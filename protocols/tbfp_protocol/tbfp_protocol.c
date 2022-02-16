#include "tbfp_protocol.h"

#include <string.h>
#include <time.h>

#include "byte_utils.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#endif

#include "data_utils.h"
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
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
#include "protocol_diag.h"

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
#ifdef HAS_TBFP_RETX
#include "tbfp_re_tx_ack_fsm.h"
#endif
#ifdef HAS_CLI
#include "writer_config.h"
#endif

#ifdef HAS_GNSS
#include "gnss_drv.h"
#endif

#ifdef X86_64
#include "log.h"
#include <stdio.h>
#endif


#ifndef HAS_TBFP
#error "That components needs HAS_TBFP macro define"
#endif

TbfpProtocol_t TbfpProtocol[IF_CNT] = {0};/*TODO: make as linked list due to run-time build*/

bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface, uint8_t preamble_val) {
    bool res = false;
    if(instance) {
        memset(instance, 0x0, sizeof(TbfpProtocol_t));
        instance->interface = interface;
#ifdef HAS_TBFP_FLOW_CONTROL
        instance->prev_s_num = 0;
        instance->s_num = 1;
#endif
#ifdef HAS_DEBUG
        instance->max_len = 0;
        instance->min_len = 0xFFFF;
#endif
        instance->parser.preamble_val=preamble_val; /*For pack tunneling*/
        instance->rx_pkt_cnt = 0;
        res = true;
#ifdef HAS_TBFP_RETX
        res = tbfp_retx_init(instance);
#endif
    }
    if(TBFP_SIZE_HEADER != sizeof(TbfHeader_t)) {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "HeaderLenErr");
#endif
        res = false;
    }
    return res;
}

bool is_tbfp_protocol(uint8_t* arr, uint16_t len, Interfaces_t interface) {
    bool res = false;
#ifdef HAS_LOG
    LOG_DEBUG(TBFP, "IsTBFP len: %u", len);
#endif
    TbfHeader_t header = {0};
    memcpy(&header, arr, sizeof(TbfHeader_t));
    if(TbfpProtocol[interface].parser.preamble_val != header.preamble){
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "FramePreambleErr Exp:0x%x Real:0x%x",TbfpProtocol[interface].parser.preamble_val,header.preamble);
#endif
        res = false;
    }else{
        res = true;
    }
    if(res){
        if(header.len < len) {
            res = true;
        } else {
            res = false;
    #ifdef HAS_LOG
            LOG_ERROR(TBFP, "FrameLenErr",header.len);
    #endif
        }
    }
    if(res) {
        uint32_t frame_len = TBFP_SIZE_HEADER + header.len;
        uint8_t read_crc8 = arr[frame_len];
        res = crc8_sae_j1850_check(arr, frame_len, read_crc8);
        if(false == res) {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "CrcErr Read:0x%02x", read_crc8);
#endif
        }
    }

    return res;
}

static bool tbfp_make_header(uint8_t* out_array, uint32_t payload_len, Interfaces_t interface, uint8_t lifetime, TbfpAck_t ack) {
    bool res = false;
    if(payload_len < TBFP_MAX_PAYLOAD) {
        if(out_array) {
            TbfHeader_t header;
            memset(&header,0,sizeof(header));
            header.preamble = TbfpProtocol[interface].parser.preamble_val;
#ifdef HAS_TBFP_FLOW_CONTROL
            header.snum = TbfpProtocol[interface].s_num;
            TbfpProtocol[interface].ReTxFsm.expected_ser_num = header.snum ;
            TbfpProtocol[interface].s_num++;
#endif /*HAS_TBFP_FLOW_CONTROL*/

            header.flags.ack_need = ack;
            if(lifetime<3){
                header.flags.lifetime = lifetime;
            }
            TbfpProtocol[interface].tx_pkt_cnt++;
            header.len = (uint16_t)payload_len;
            memcpy(out_array, &header, sizeof(TbfHeader_t));
            res = true;
        } else {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "NullPayLoad");
#endif
        }

    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "TooBigPayload %u (Lim: %u)", payload_len, TBFP_MAX_PAYLOAD);
#endif
    }
    return res;
}

bool tbfp_send(uint8_t* payload, uint32_t payload_len, Interfaces_t interface,
               uint8_t lifetime, TbfpAck_t ack) {
    bool res = false;
    Retx_t retx = RETX_NO_NEED;
    if(ACK_NEED==ack){
       retx = RETX_NEED;
    }

    if(payload && (0 < payload_len)) {
        uint16_t tx_frame_len = payload_len + TBFP_SIZE_OVERHEAD;
        if(tx_frame_len < sizeof(TbfpProtocol[interface].tx_frame)) {
            uint32_t frame_len = TBFP_SIZE_HEADER + payload_len;
            memset(TbfpProtocol[interface].tx_frame, 0x00, sizeof(TbfpProtocol[interface].tx_frame));
            res = tbfp_make_header(TbfpProtocol[interface].tx_frame, payload_len, interface, lifetime, ack);
            if(res) {
                TbfHeader_t OutHeader = {0};
                memcpy(&OutHeader, TbfpProtocol[interface].tx_frame, sizeof(TbfHeader_t));
                memcpy(&(TbfpProtocol[interface].tx_frame[TBFP_INDEX_PAYLOAD]), payload, payload_len);
                TbfpProtocol[interface].tx_frame[frame_len] =
                    crc8_sae_j1850_calc(TbfpProtocol[interface].tx_frame, frame_len);
                LOG_DEBUG(TBFP,"%s Sent SN:%u=0x%04x Len:%u crc8 0x%02x", interface2str(interface),
                          OutHeader.snum,OutHeader.snum,
                          OutHeader.len,TbfpProtocol[interface].tx_frame[frame_len]);
                TbfpProtocol[interface].tx_byte += frame_len + TBFP_SIZE_CRC;
                res = sys_send_if(TbfpProtocol[interface].tx_frame, frame_len + TBFP_SIZE_CRC, interface, retx);
            } else {
#ifdef HAS_LOG
                LOG_ERROR(TBFP, "TooBigPayload cur: %u max: %u", payload_len, sizeof(TbfpProtocol[interface].tx_frame));
#endif
            }
        } else {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "TooBigTxFrame cur: %u max: %u", tx_frame_len, sizeof(TbfpProtocol[interface].tx_frame));
#endif
        }
    }
    return res;
}

static bool tbfp_send_ack(uint16_t snum, Interfaces_t interface){
    bool res = false;
    uint8_t payload[4] = "";
    payload[0] =FRAME_ID_ACK  ;
    memcpy(&payload[1], &snum, 2);
    res = tbfp_send(payload, 3, interface, 0, ACK_NO_NEED);
    if(false==res){
        LOG_ERROR(TBFP, "SendAckErr");
    }else{
        TbfpProtocol[interface].ReTxFsm.ack_tx_cnt++;
        LOG_DEBUG(TBFP, "SendAckOk");
    }
    return res;
}

static bool tbfp_send_text(uint8_t payload_id, uint8_t* tx_array, uint32_t len, Interfaces_t interface,
                           uint8_t lifetime, TbfpAck_t ack) {
    bool res = false;
    uint8_t frame[256] = "";
    LOG_DEBUG(TBFP, "%s SendText Ack:%u", interface2str(interface),ack);
    if(tx_array && (0 < len) && ((len+TBFP_SIZE_OVERHEAD+TBFP_SIZE_ID)<sizeof(frame))) {
        frame[0] = payload_id;
        memcpy(&frame[1], tx_array, len);
        res = tbfp_send(frame, len + TBFP_SIZE_ID, interface, lifetime, ack);
        if(false == res) {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "SendTestErr");
#endif
        }
    }
    return res;
}

bool tbfp_send_chat(uint8_t* tx_array, uint32_t len,
                    Interfaces_t interface,
                    uint8_t lifetime, TbfpAck_t ack) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CHAT, tx_array, len, interface, lifetime, ack);
    return res;
}

bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CMD, tx_array, len, interface, 0, ACK_NEED);
    return res;
}

bool tbfp_send_tunnel(uint8_t* tx_array, uint32_t len, Interfaces_t interface, TbfpAck_t ack) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_TUNNEL, tx_array, len, interface, 0, ack);
    return res;
}

bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface) {
    bool res = false;
    uint8_t frame[256] = "";
    memset(frame, 0, sizeof(frame));
    TbfPingFrame_t pingFrame = {0};
    pingFrame.id = frame_id;
#ifdef CC26XX
    pingFrame.mac = get_ble_mac();
#endif
    pingFrame.coordinate.latitude = 99999.0;
    pingFrame.coordinate.longitude = 9999.0;
#ifdef HAS_GNSS
    pingFrame.time_stamp = mktime(&Gnss.time_date);

    res = is_valid_gnss_coordinates(Gnss.coordinate_cur);
    if(res) {
        pingFrame.coordinate = Gnss.coordinate_cur;
    } else {
        /*invalid coordinate*/
        pingFrame.coordinate.latitude = 360.0;
        pingFrame.coordinate.longitude = 360.0;
    }
#endif
    memcpy(frame, &pingFrame, sizeof(TbfPingFrame_t));
    res = tbfp_send(frame, sizeof(TbfPingFrame_t), interface, 0, ACK_NO_NEED);
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "SendErr");
#endif
    }
    return res;
}

static bool tbfp_proc_ping(uint8_t* ping_payload, uint16_t len, Interfaces_t interface) {
    bool res = false;
    if(NULL != ping_payload) {
        TbfPingFrame_t pingFrame = {0};
        memcpy((void*)&pingFrame, (void*)ping_payload, sizeof(TbfPingFrame_t));
#if defined(HAS_LOG) && defined(HAS_MCU)
        tbfp_print_ping_frame(&pingFrame);
#endif /*HAS_MCU*/
        if(FRAME_ID_PING == pingFrame.id) {
            res = tbfp_send_ping(FRAME_ID_PONG, interface);
        }
#ifdef HAS_GNSS
        double cur_dist = 0.0;
        double azimuth = 0.0;
        if(is_valid_gnss_coordinates(pingFrame.coordinate)) {
            if(is_valid_gnss_coordinates(Gnss.coordinate_cur)) {
                cur_dist = gnss_calc_distance_m(Gnss.coordinate_cur, pingFrame.coordinate);
                azimuth = gnss_calc_azimuth_deg(Gnss.coordinate_cur, pingFrame.coordinate);
                double sec = difftime(pingFrame.time_stamp ,
                                      mktime(&Gnss.time_date));
#ifdef HAS_LOG
                LOG_INFO(TBFP, "LinkDistance %3.3f m %4.1f deg %f s", cur_dist, azimuth, sec);
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
#endif /*HAS_GNSS*/

#if defined(HAS_LORA) && defined(HAS_GNSS)
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

#endif /*HAS_LORA HAS_GNSS*/
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

bool tbfp_parser_reset_rx(TbfpProtocol_t* instance, RxState_t state) {
    bool res = false;
    if(instance) {
        instance->parser.rx_state = WAIT_PREAMBLE;
        instance->parser.load_len = 0;
        LOG_PARN(TBFP, "ResetFsmIn: %s", RxState2Str(state));
        res = true;
    }
    return res;
}

static bool tbfp_proc_payload(uint8_t* payload, uint16_t len, Interfaces_t interface) {
    bool res = false;
    LOG_DEBUG(TBFP, "%s ProcPayload ID:%s", interface2str(interface), tbfp_id2str(payload[0]));
    switch(payload[0]) {
    case FRAME_ID_ACK: {
        LOG_DEBUG(TBFP, "RxAck");
        uint16_t ser_num=0;
        memcpy(&ser_num, &payload[1], 2);
#ifdef HAS_TBFP_RETX
        res = tbfp_retx_ack(&TbfpProtocol[interface], ser_num);
#endif
    } break;
    case FRAME_ID_TUNNEL: {
        LOG_DEBUG(TBFP, "TBFP in TBFP"); /*matryoshka*/
        res = tbfp_proc(&payload[1], len-1, IF_LORA, false);
    } break;
#ifdef HAS_RTCM3
    case FRAME_ID_RTCM3:
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "RTCMpayload");
#endif
        res = rtcm3_proc_array(payload, len, interface);
        break;
#endif /*HAS_RTCM3*/
    case FRAME_ID_CHAT:
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "ChatPayload");
#endif
        res = tbfp_proc_chat(payload, len);
        break;
    case FRAME_ID_PONG:

    case FRAME_ID_PING:
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "PingPayload");
#endif
        res = tbfp_proc_ping(payload, len, interface);
        break;
    case FRAME_ID_CMD:
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "CmdPayload");
#endif
        res = tbfp_proc_cmd(payload, len);
        break;
    default:
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "%s UndefPayload ID: 0x%02x",interface2str(interface), payload[0]);
#endif
        res = false;
        break;
    }
    return res;
}

/*One arr frame can contain several TBFP frames*/
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface, bool is_reset_parser) {
#ifdef HAS_LOG
    LOG_PARN(TBFP, "%s Proc Len:%u", interface2str(interface), len);
#endif
    bool res = true;
    uint32_t cur_rx_prk = 0;
    uint32_t init_rx_prk = TbfpProtocol[interface].rx_pkt_cnt;
    if(is_reset_parser) {
        res = tbfp_parser_reset_rx(&TbfpProtocol[interface], WAIT_INIT);
    }
    uint32_t i = 0, ok_cnt = 0, err_cnt = 0;
    for(i = 0; i < len; i++) {
        res = tbfp_proc_byte(&TbfpProtocol[interface], arr[i]);
        if(res) {
            ok_cnt++;
        } else {
            err_cnt++;
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "Arr[%u]=0x%x", i, arr[i]);
#endif
        }
    }

    cur_rx_prk = TbfpProtocol[interface].rx_pkt_cnt - init_rx_prk;

    if(0 < cur_rx_prk) {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "%s %u Packets in %u byte",interface2str(interface), cur_rx_prk, len);
#endif
    } else {
        if(is_reset_parser){
            TbfpProtocol[interface].lack_frame_in_data++;
#ifdef HAS_LOG
            LOG_DEBUG(TBFP, "%s LackPktInFrame:%u ",
                  interface2str(interface),
                  len);
        }
#endif
#ifdef HAS_DEBUG
        if(true == TbfpProtocol[interface].debug) {
            print_mem(arr, len, true, false, true, true);
        }
#endif
    }
    if(len == ok_cnt) {
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "ProcErr %u!=%u", ok_cnt, len);
#endif
        res = false;
    }

    return res;
}

#ifdef HAS_TBFP_FLOW_CONTROL
static bool flow_ctrl_print_lost(uint16_t prev_s_num, uint16_t s_num, uint32_t con_flow, Interfaces_t interface ) {
    bool res = true;
    if(prev_s_num < (s_num - 1)) {
        uint32_t lost_frame_cnt = s_num - prev_s_num - 1;
        if(0<lost_frame_cnt){
            TbfpProtocol[interface].lost_rx_frames += lost_frame_cnt;

        }
        if((prev_s_num + 1) == (s_num - 1)) {
            LOG_DEBUG(TBFP, "%s Lost_%u %u: Flow:%u",
                        interface2str(interface),
                        lost_frame_cnt,
                        s_num - 1,
                        con_flow
                        );
        } else {
            LOG_DEBUG(TBFP, "%s Lost_%u %u-%u Flow:%u",
                        interface2str(interface),
                        lost_frame_cnt,
                        prev_s_num + 1,
                        s_num - 1,
                        con_flow
                        );
        }

    } else {
        res = false;
    }
    return res;
}
#endif /*HAS_TBFP_FLOW_CONTROL*/

#ifdef HAS_TBFP_FLOW_CONTROL
bool tbfp_check_flow_control(
                             Interfaces_t interface,
                             uint16_t snum,
                             uint16_t * const io_prev_s_num,
                             uint32_t * const io_con_flow,
                             uint32_t * const io_max_con_flow
                             ) {
    bool res = false;
    uint16_t prev_s_num=*io_prev_s_num;
    uint16_t con_flow=*io_con_flow;
    uint16_t max_con_flow=*io_max_con_flow;
#ifdef HAS_LOG
    LOG_PARN(TBFP, "%s prev_snum:%u snum:%u flow:%u",interface2str(interface),  prev_s_num, snum,  con_flow);
#endif
    if( snum <  prev_s_num) {
        /*Unreal situation*/
        LOG_ERROR(TBFP, "SnOrderError SNcur:%u<=SNprev:%u", snum,  prev_s_num);
        //  con_flow = 1;
        res = false;
    }else if( snum==( 1+ prev_s_num ) ) {
        /*Flow ok*/
        LOG_DEBUG(TBFP, "%s FlowOk %u->%u",interface2str(interface),prev_s_num,snum);
        con_flow++;
         max_con_flow = max16u( max_con_flow,  con_flow);
        res = true;
    } else if(( prev_s_num + 1) < snum) {
        TbfpProtocol[interface].flow_torn_cnt++;
        LOG_DEBUG(TBFP, "%s FlowTorn! SN:%u",interface2str(interface),snum);
        flow_ctrl_print_lost( prev_s_num, snum,  con_flow, interface );
        con_flow = 1;
        res = true;
    }  else if( snum==prev_s_num ) {
        /*Rx Retx*/
        LOG_DEBUG(TBFP, "Duplicate! SN=%u", snum);
        res = false;
    }else {
        /*Unreal situation*/
        res = false;
    }

    prev_s_num= snum;


#ifdef HAS_LOG
    LOG_PARN(TBFP, "%s prev_snum:%u snum:%u flow:%u",interface2str(interface),  prev_s_num, snum,  con_flow);
#endif

    *io_prev_s_num=prev_s_num;
    *io_con_flow=con_flow;
    *io_max_con_flow=max_con_flow;

    return res;
}
#endif /*HAS_TBFP_FLOW_CONTROL*/

bool tbfp_proc_full(uint8_t* arr, uint16_t len, Interfaces_t interface) {
    bool res = true;
    LOG_INFO(TBFP, "%s ProcFull Len: %u", interface2str(interface), len);
    res = is_tbfp_protocol(arr, len, interface);
    if(res) {
        TbfpProtocol[interface].rx_byte +=len;
        TbfHeader_t inHeader = {0};
        memcpy(&inHeader, arr, sizeof(TbfHeader_t));
#ifdef HAS_TBFP_FLOW_CONTROL
        bool flow_ctrl_ok = false;
        flow_ctrl_ok = tbfp_check_flow_control(interface,
                                               inHeader.snum,
                                               &(TbfpProtocol[interface].prev_s_num),
                                               &(TbfpProtocol[interface].con_flow),
                                               &(TbfpProtocol[interface].max_con_flow)
                                               );
        if(false == flow_ctrl_ok) {
            TbfpProtocol[interface].err_cnt++;
#ifdef HAS_LOG
            LOG_DEBUG(TBFP, "FlowErr %s", interface2str(interface));
#endif
        }else{

        }
#endif /*HAS_TBFP_FLOW_CONTROL*/

#ifdef HAS_TBFP_RETRANSMIT
        if((0<inHeader.flags.lifetime) && (inHeader.flags.lifetime<4)) {
            res = tbfp_send(&arr[TBFP_INDEX_PAYLOAD],
                            inHeader.len,
                            interface,
                            inHeader.flags.lifetime - 1,
                            (TbfpAck_t) inHeader.flags.ack_need );
        }
#endif /*HAS_TBFP_RETRANSMIT*/

        if(flow_ctrl_ok){
            res = tbfp_proc_payload(&arr[TBFP_INDEX_PAYLOAD], inHeader.len, interface);
        }
        /*Ack After Proc to pass PC unit tests*/
        if(inHeader.flags.ack_need){
            res = tbfp_send_ack( inHeader.snum, interface);
        }

    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "NotAframe");
#endif
    }

    return res;
}

bool tbfp_generate_frame(uint8_t* buff, uint32_t buff_len, Interfaces_t interface) {
    bool res = false;
    if(buff && (TBFP_OVERHEAD_SIZE <= buff_len)) {
        uint16_t snum = 0;
        uint16_t payload_len = buff_len - TBFP_OVERHEAD_SIZE;
        buff[TBFP_INDEX_PREAMBLE] = TbfpProtocol[interface].parser.preamble_val;
        buff[TBFP_INDEX_RETX] = 0;
        memcpy(&buff[TBFP_INDEX_SER_NUM], &snum, TBFP_SIZE_SN);
        memcpy(&buff[TBFP_INDEX_LEN], &payload_len, TBFP_SIZE_SN);
        uint32_t i = 0;
        for(i = 0; i < payload_len; i++) {
            buff[TBFP_INDEX_PAYLOAD + i] = i;
        }
        uint16_t frame_len = payload_len + sizeof(TbfHeader_t);
        buff[frame_len] = crc8_sae_j1850_calc(buff, frame_len);
        print_mem(buff, frame_len + 1, true, false, true, false);

        res = true;
    }
    return res;
}

bool tbfp_check(void){
    bool res = true;
    Interfaces_t interface=(Interfaces_t)0;
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){
            uint32_t diff =TbfpProtocol[interface].ReTxFsm.silence_cnt-TbfpProtocol[interface].ReTxFsm.silence_cnt_prev ;
            if(0<diff ){
                res = false;
                LOG_ERROR(RETX,"%s LackOfAck %u times",interface2str(interface),diff);
            }
            TbfpProtocol[interface].ReTxFsm.silence_cnt_prev = TbfpProtocol[interface].ReTxFsm.silence_cnt ;

            diff=TbfpProtocol[interface].crc_err_cnt-TbfpProtocol[interface].crc_err_cnt_prev;
            if(0<diff ){
                res = false;
                LOG_ERROR(TBFP,"%s CrcErr %u times",interface2str(interface),diff);
            }
            TbfpProtocol[interface].crc_err_cnt_prev=TbfpProtocol[interface].crc_err_cnt;


            diff=TbfpProtocol[interface].flow_torn_cnt-TbfpProtocol[interface].flow_torn_cnt_prev;
            if(0<diff ){
                res = false;
                LOG_ERROR(TBFP,"%s FlowTorn %u times",interface2str(interface),diff);
            }
            TbfpProtocol[interface].flow_torn_cnt_prev=TbfpProtocol[interface].flow_torn_cnt;


            diff=TbfpProtocol[interface].err_tx_cnt-TbfpProtocol[interface].err_tx_cnt_prev;
            if(0<diff ) {
                res = false;
                LOG_ERROR(TBFP,"%s TxErr %u times",interface2str(interface),diff);
            }
            TbfpProtocol[interface].err_tx_cnt_prev=TbfpProtocol[interface].err_tx_cnt;



        }
    }
    return res;
}

bool tbfp_calc_byte_rate(void){
    bool res = true;
    Interfaces_t interface=(Interfaces_t)0;
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){
            TbfpProtocol[interface].rx_rate.cur=TbfpProtocol[interface].rx_byte -TbfpProtocol[interface].rx_byte_prev;
            TbfpProtocol[interface].rx_rate.min=min32u(TbfpProtocol[interface].rx_rate.cur,TbfpProtocol[interface].rx_rate.min);
            TbfpProtocol[interface].rx_rate.max=max32u(TbfpProtocol[interface].rx_rate.cur,TbfpProtocol[interface].rx_rate.max);
            TbfpProtocol[interface].rx_byte_prev = TbfpProtocol[interface].rx_byte;

            TbfpProtocol[interface].tx_rate.cur=TbfpProtocol[interface].tx_byte -TbfpProtocol[interface].tx_byte_prev;
            TbfpProtocol[interface].tx_rate.min=min32u(TbfpProtocol[interface].tx_rate.cur,TbfpProtocol[interface].tx_rate.min);
            TbfpProtocol[interface].tx_rate.max=max32u(TbfpProtocol[interface].tx_rate.cur,TbfpProtocol[interface].tx_rate.max);
            TbfpProtocol[interface].tx_byte_prev = TbfpProtocol[interface].tx_byte;
            res = true;
        }
    }
    return res;
}
