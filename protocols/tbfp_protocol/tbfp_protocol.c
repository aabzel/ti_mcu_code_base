
#include "tbfp_protocol.h"

#include <string.h>
#include <time.h>

#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "byte_utils.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#endif
#include "core_driver.h"
#include "crc8_sae_j1850.h"
#include "data_utils.h"
#include "debug_info.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif

#ifdef HAS_PARAM
#include "param_ids.h"
#endif
#include "float_utils.h"
#include "gnss_utils.h"
#include "io_utils.h"
#include "log.h"
#include "tbfp_diag.h"
#include "writer_config.h"
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

TbfpPorotocol_t TbfpPorotocol = {0};

bool tbfp_protocol_init(TbfpPorotocol_t* instance) {
    bool res = false;
    if(instance) {
        memset(instance, 0x0, sizeof(TbfpPorotocol_t));
        res = true;
    }
    return res;
}

bool is_tbfp_protocol(uint8_t* arr, uint16_t len) {
    bool res = false;
    TbfHeader_t header = {0};
    memcpy(&header, arr, sizeof(TbfHeader_t));
    if(TBFP_PREAMBLE == header.preamble && (header.len < len)) {
        res = true;
    } else {
        res = false;
    }
    if(res) {
        uint32_t frame_len = TBFP_SIZE_HEADER + header.len;
        uint8_t read_crc8 = arr[frame_len];
        uint8_t calc_crc8 = 0;
        calc_crc8 = crc8_sae_j1850_calc(arr, frame_len);
        if(calc_crc8 == read_crc8) {
            res = true;
        } else {
            res = false;
        }
    }

    return res;
}

static bool tbfp_make_header(uint8_t* out_array, uint32_t payload_len) {
    bool res = false;
    if(out_array && (0 < payload_len) && (payload_len < TBFP_MAX_PAYLOAD)) {
        out_array[TBFP_INDEX_PREAMBLE] = TBFP_PREAMBLE;
        out_array[TBFP_INDEX_LEN] = (uint8_t)payload_len;
        res = true;
    }
    return res;
}

bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame) {
    bool res = false;
    if(out_frame && tx_frame_len) {
        res = tbfp_make_header(out_frame, sizeof(TbfPingFrame_t));
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

static bool tbfp_send_text(uint8_t payload_id, uint8_t* tx_array, uint32_t len) {
    bool res = false;
    if(tx_array && (0 < len)) {
        uint8_t frame[256] = "";
        uint32_t frame_len = TBFP_SIZE_HEADER + TBFP_SIZE_ID + len;
        res = tbfp_make_header(frame, len + TBFP_SIZE_ID);
        if(res) {
            frame[TBFP_INDEX_PAYLOAD] = payload_id;
            memcpy(&frame[TBFP_INDEX_PAYLOAD + 1], tx_array, len);
            frame[frame_len] = crc8_sae_j1850_calc(frame, frame_len);
            if(res) {
                res = lora_send_queue(frame, frame_len + TBFP_SIZE_CRC);
            }
        }
    }
    return res;
}

bool tbfp_send_chat(uint8_t* tx_array, uint32_t len) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CHAT, tx_array, len);
    return res;
}

bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len) {
    bool res = false;
    res = tbfp_send_text(FRAME_ID_CMD, tx_array, len);
    return res;
}

bool tbfp_send_ping(uint8_t frame_id) {
    bool res = false;
    uint8_t frame[256] = "";
    memset(frame, 0, sizeof(frame));
    uint32_t tx_frame_len = 0;
    TbfPingFrame_t pingFrame = {0};
    pingFrame.id = frame_id;
    pingFrame.mac = get_ble_mac();
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
    res = tbfp_compose_ping(frame, &tx_frame_len, &pingFrame);
    if(res) {
        res = lora_send_queue(frame, tx_frame_len);
    }
    return res;
}

static bool tbfp_proc_ping(uint8_t* ping_payload, uint16_t len) {
    bool res = false;
    if(NULL != ping_payload) {
        TbfPingFrame_t pingFrame = {0};
        memcpy((void*)&pingFrame, (void*)ping_payload, sizeof(TbfPingFrame_t));

        tbfp_print_ping_frame(&pingFrame);

        if(FRAME_ID_PING == pingFrame.id) {
            res = tbfp_send_ping(FRAME_ID_PONG);
        }
        double cur_dist = 0;
#ifdef HAS_ZED_F9P
        if(is_valid_gnss_coordinates(pingFrame.coordinate)) {
            cur_dist = gnss_calc_distance_m(ZedF9P.coordinate_cur, pingFrame.coordinate);
            LOG_INFO(LORA, "link distance %f m", cur_dist);
        } else {
            LOG_ERROR(LORA, "InvalidGNSSCoordinate");
        }
#endif /*HAS_ZED_F9P*/

#ifdef HAS_LORA
        uint16_t file_len = 0;
        res = mm_get(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, sizeof(double), &file_len);
        if((LoRaInterface.max_distance < cur_dist) && (res) && (sizeof(double) == file_len)) {
#if defined(HAS_PARAM) && defined(HAS_FLASH_FS)
            res = mm_set(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&cur_dist, sizeof(double));
            if(false == res) {
                LOG_ERROR(LORA, "UpdateMaxDist");
            }
#endif /*HAS_PARAM && HAS_FLASH_FS*/
            LoRaInterface.max_distance = cur_dist;
        }

#endif /*HAS_LORA*/
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
#ifdef HAS_CLI
        curWriterPtr = &dbg_lora_o;
        payload[len] = 0x00;
        res = process_shell_cmd((char*)&payload[1]);
        curWriterPtr = &dbg_o;
#endif
    }
    return res;
}

bool tbfp_proc_payload(uint8_t* payload, uint16_t len) {
    bool res = false;
    switch(payload[0]) {
    case FRAME_ID_CHAT:
        res = tbfp_proc_chat(payload, len);
        break;
    case FRAME_ID_PONG:
    case FRAME_ID_PING:
        res = tbfp_proc_ping(payload, len);
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

bool tbfp_proc(uint8_t* arr, uint16_t len) {
    bool res = false;
    res = is_tbfp_protocol(arr, len);
    if(res) {
        TbfHeader_t header = {0};
        memcpy(&header, arr, sizeof(TbfHeader_t));
        res = tbfp_proc_payload(&arr[TBFP_INDEX_PAYLOAD], header.len);
    }
    return res;
}
