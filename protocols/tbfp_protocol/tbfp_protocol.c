#include "tbfp_protocol.h"

#include <string.h>
#include <time.h>

#include "byte_utils.h"
#ifdef HAS_CLI
#include "cli_manager.h"
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
#include "io_utils.h"
#include "log.h"
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
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
#include "writer_config.h"
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

TbfpProtocol_t TbfpProtocol[3] = {0};

bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface) {
    bool res = false;
    if(instance) {
        memset(instance, 0x0, sizeof(TbfpProtocol_t));
        instance->interface = interface;
        instance->rx_pkt_cnt = 0;
        res = true;
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
#ifdef X86_64
        printf("\n%s(): error", __FUNCTION__);
#endif
    }
    if(res) {
        uint32_t frame_len = TBFP_SIZE_HEADER + header.len;
        uint8_t read_crc8 = arr[frame_len];
        res = crc8_sae_j1850_check(arr, frame_len, read_crc8);
        if(false == res) {
#ifdef X86_64
            printf("\n%s(): CRC8 error", __FUNCTION__);
#endif
        }
    }

    return res;
}

static bool tbfp_make_header(uint8_t* out_array, uint32_t payload_len, Interfaces_t interface) {
    bool res = false;
    if(out_array && (0 < payload_len) && (payload_len < TBFP_MAX_PAYLOAD)) {
        TbfHeader_t header;
        header.preamble = TBFP_PREAMBLE;
#ifdef HAS_TBFP_FLOW_CONTROL
        header.snum = TbfpProtocol[interface].s_num;
        TbfpProtocol[interface].s_num++;
#endif /*HAS_TBFP_FLOW_CONTROL*/
        TbfpProtocol[interface].tx_pkt_cnt++;
        header.len = (uint8_t)payload_len;
        memcpy(out_array, &header, sizeof(TbfHeader_t));
        res = true;
    }
    return res;
}

bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame, Interfaces_t interface) {
    bool res = false;
    if(out_frame && tx_frame_len) {
        res = tbfp_make_header(out_frame, sizeof(TbfPingFrame_t), interface);
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

bool tbfp_send(uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    if(tx_array && (0 < len)) {
        uint8_t frame[256] = "";
        uint32_t frame_len = TBFP_SIZE_HEADER + len;
        res = tbfp_make_header(frame, len, interface);
        if(res) {
            memcpy(&frame[TBFP_INDEX_PAYLOAD], tx_array, len);
            frame[frame_len] = crc8_sae_j1850_calc(frame, frame_len);
            switch(interface) {
            case IF_LORA: {
#ifdef HAS_LORA
                res = lora_send_queue(frame, frame_len + TBFP_SIZE_CRC);
#endif
            } break;
            case IF_RS232: {
#ifdef HAS_RS232
                res = rs232_send(frame, frame_len + TBFP_SIZE_CRC);
#endif
            } break;
            case IF_LOOPBACK: {
                res = tbfp_proc_full(frame, frame_len + TBFP_SIZE_CRC, IF_LOOPBACK);
            } break;
            default:
                break;
            }
        }
    }
    return res;
}

static bool tbfp_send_text(uint8_t payload_id, uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    if(tx_array && (0 < len)) {
        uint8_t frame[256] = "";
        uint32_t frame_len = TBFP_SIZE_HEADER + TBFP_SIZE_ID + len;
        res = tbfp_make_header(frame, len + TBFP_SIZE_ID, interface);
        if(res) {
            frame[TBFP_INDEX_PAYLOAD] = payload_id;
            memcpy(&frame[TBFP_INDEX_PAYLOAD + 1], tx_array, len);
            frame[frame_len] = crc8_sae_j1850_calc(frame, frame_len);
            if(res) {
                switch(interface) {
                case IF_LORA: {
#ifdef HAS_LORA
                    res = lora_send_queue(frame, frame_len + TBFP_SIZE_CRC);
#endif
                } break;
                case IF_RS232: {
#ifdef HAS_RS232
                    res = rs232_send(frame, frame_len + TBFP_SIZE_CRC);
#endif
                } break;
                case IF_LOOPBACK: {
                    res = tbfp_proc_full(frame, frame_len + TBFP_SIZE_CRC, IF_LOOPBACK);
                } break;
                default:
                    break;
                }
            }
        }
    }
    return res;
}

bool tbfp_send_chat(uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CHAT, tx_array, len, interface);
    return res;
}

bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CMD, tx_array, len, interface);
    return res;
}

bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface) {
    bool res = false;
    uint8_t frame[256] = "";
    memset(frame, 0, sizeof(frame));
    uint32_t tx_frame_len = 0;
    TbfPingFrame_t pingFrame = {0};
    pingFrame.id = frame_id;
#ifdef HAS_MCU
    pingFrame.mac = get_ble_mac();
#endif
    memset(&pingFrame.coordinate, 0xFF, sizeof(GnssCoordinate_t));
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
        switch(interface) {
#ifdef HAS_LORA
        case IF_LORA:
            res = lora_send_queue(frame, tx_frame_len);
            break;
#endif
#ifdef HAS_RS232
        case IF_RS232:
            res = rs232_send(frame, tx_frame_len);
            break;
#endif
        default:
            res = false;
            break;
        }
    }
    return res;
}

static bool tbfp_proc_ping(uint8_t* ping_payload, uint16_t len, Interfaces_t interface) {
    bool res = false;
    if(NULL != ping_payload) {
        TbfPingFrame_t pingFrame = {0};
        memcpy((void*)&pingFrame, (void*)ping_payload, sizeof(TbfPingFrame_t));
#ifdef HAS_MCU
        tbfp_print_ping_frame(&pingFrame);
#endif /*HAS_MCU*/
        if(FRAME_ID_PING == pingFrame.id) {
            res = tbfp_send_ping(FRAME_ID_PONG, interface);
        }
#ifdef HAS_ZED_F9P
        double cur_dist = 0;
        if(is_valid_gnss_coordinates(pingFrame.coordinate)) {
            if(is_valid_gnss_coordinates(ZedF9P.coordinate_cur)) {
                cur_dist = gnss_calc_distance_m(ZedF9P.coordinate_cur, pingFrame.coordinate);
                LOG_INFO(LORA, "LinkDistance %f m", cur_dist);
            } else {
                LOG_ERROR(LORA, "InvalidLocalGNSSDot");
            }
        } else {
            LOG_ERROR(LORA, "InvalidRemoteGNSSDot");
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
                LOG_ERROR(LORA, "UpdateMaxDist");
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
        res = print_mem(&payload[1], len - 1, false, true, true, true);
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
    switch(payload[0]) {
#ifdef HAS_RTCM3
    case FRAME_ID_RTCM3:
        res = rtcm3_proc_array(payload, len, interface);
        break;
#endif /*HAS_RTCM3*/
    case FRAME_ID_CHAT:
        res = tbfp_proc_chat(payload, len);
        break;
    case FRAME_ID_PONG:
    case FRAME_ID_PING:
        res = tbfp_proc_ping(payload, len, interface);
        break;
    case FRAME_ID_CMD:
        res = tbfp_proc_cmd(payload, len);
        break;
    default:
        res = false;
        break;
    }
    return res;
}

/*One LoRa frame can contain several TBFP frames*/
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface) {
    bool res = true;
    uint32_t cur_rx_prk = 0;
    uint32_t init_rx_prk = TbfpProtocol[interface].rx_pkt_cnt;
    res = tbfp_parser_reset_rx(&TbfpProtocol[interface]);
    uint32_t i = 0, ok_cnt = 0, err_cnt = 0;
    for(i = 0; i < len; i++) {
        res = tbfp_proc_byte(&TbfpProtocol[interface], arr[i]);
        if(res) {
            ok_cnt++;
        } else {
            err_cnt++;
            LOG_ERROR(TBFP, "i=%u", i);
        }
    }
    cur_rx_prk = TbfpProtocol[interface].rx_pkt_cnt - init_rx_prk;
    if(0 < cur_rx_prk) {
        LOG_INFO(TBFP, "InPktCnt:%u in %u byte", cur_rx_prk, len);
    } else {
        LOG_ERROR(TBFP, "LackPkt:%u", len);
        print_mem(arr, len, true, false, true, true);
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

    res = is_tbfp_protocol(arr, len);
    if(res) {
        TbfHeader_t inHeader = {0};
        memcpy(&inHeader, arr, sizeof(TbfHeader_t));
#ifdef HAS_TBFP_FLOW_CONTROL
#ifdef X86_64
        printf("\n%s(): prev_snum:%u snum:%u", __FUNCTION__, TbfpProtocol[interface].prev_s_num, inHeader.snum);
#endif
        if((TbfpProtocol[interface].prev_s_num + 1) == inHeader.snum) {
            /*Flow ok*/
            TbfpProtocol[interface].con_flow++;
            TbfpProtocol[interface].max_con_flow =
                max16(TbfpProtocol[interface].max_con_flow, TbfpProtocol[interface].con_flow);
        } else if((TbfpProtocol[interface].prev_s_num + 1) < inHeader.snum) {
#ifdef HAS_MCU
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
            TbfpProtocol[interface].err_cnt++;
        }
        TbfpProtocol[interface].prev_s_num = inHeader.snum;
#ifdef X86_64
        printf("\n2 %s(): prev_snum:%u snum:%u", __FUNCTION__, TbfpProtocol[interface].prev_s_num, inHeader.snum);
#endif
#endif /*HAS_TBFP_FLOW_CONTROL*/
        res = tbfp_proc_payload(&arr[TBFP_INDEX_PAYLOAD], inHeader.len, interface);
    }

    return res;
}
