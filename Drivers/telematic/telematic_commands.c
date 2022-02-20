#include "telematic_commands.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#ifdef HAS_LOG
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#endif

#include "convert.h"
#include "core_driver.h"
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "system.h"
#include "tbfp_protocol.h"
#include "table_utils.h"
#include "writer_config.h"
#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

bool telematic_sent_command(int32_t argc, char* argv[]) {
    bool res = false;
    char tx_array[256] = "";
    char suffix[256] = "";
    uint16_t tx_array_len = 0;
    uint32_t timeout_s = 0;
    uint8_t interface = IF_NONE;

    if(1 <= argc) {
        res = try_str2uint8(argv[0], &interface);
    }

    if(2 <= argc) {
        res = true;
        uint8_t a = 0;
        snprintf((char*)suffix, sizeof(suffix), "%s", argv[1]);
        strncat(tx_array, suffix, sizeof(tx_array));

        LOG_INFO(SYS, "arg: [%s]", (char*)tx_array);
        for(a = 2; a < argc; a++) {
            snprintf((char*)suffix, sizeof(suffix), " %s", argv[a]);
            strncat(tx_array, suffix, sizeof(tx_array));
        }
        LOG_INFO(SYS, "arg: [%s]", (char*)tx_array);
        tx_array_len = strlen((char*)tx_array) + 1;
    }

    if(0 == argc) {
        LOG_ERROR(SYS, "Usage: lcm if cmd arg1 arg2 [argN]");
        LOG_INFO(SYS, "cli_cmd command for remote target");
        res = false;
    }

    if(res) {
        res = false;
        (void)tx_array_len;
        (void)timeout_s;
        res = tbfp_send_cmd((uint8_t*) tx_array, tx_array_len, (Interfaces_t)interface);
        if(res) {
            LOG_INFO(SYS, "OK");
        }
    }

    return res;
}

bool ping_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t interface = IF_NONE;
    if(0 <= argc) {
        res = true;
        interface = IF_LORA;
    }

    if(1 <= argc) {
        res = try_str2uint8(argv[0], &interface);
    }

    if(res) {
        res = tbfp_send_ping(FRAME_ID_PING, (Interfaces_t)interface);
        if(res) {
            LOG_INFO(SYS, "OK");
        } else {
            LOG_ERROR(SYS, "Err");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ping if");
    }
    return res;
}

bool chat_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tx_array[256] = {0};
    memset(tx_array, 0x00, sizeof(tx_array));
    uint32_t tx_array_len = 0;
    uint8_t interface = IF_LORA;
    uint8_t lifetime = 0;
    uint8_t ack = ACK_NEED;
    if(1 <= argc) {
        res = try_str2array(argv[0], tx_array, sizeof(tx_array), &tx_array_len);
        if(false == res) {
            strncpy((char*)tx_array, argv[0], sizeof(tx_array));
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            res = true;
        }
    }

    if(2 <= argc) {
        res = try_str2uint8(argv[1], &lifetime);
    }

    if(3 <= argc) {
        res = try_str2uint8(argv[2], &interface);
    }

    if(4 <= argc) {
        res = try_str2uint8(argv[3], &ack);
    }

    if( (0 == argc) || (4 < argc) ) {
        LOG_ERROR(SYS, "Usage: char text ttl if");
        res = false;
    }

    if(res) {
        res = tbfp_send_chat(tx_array, tx_array_len, (Interfaces_t)interface,
                             lifetime,
                             (TbfpAck_t) ack);
        if(res) {
            LOG_INFO(SYS, "ok [%s]", tx_array);
            res = print_mem(tx_array, tx_array_len, false, true, true, false);
        } else {
            LOG_ERROR(SYS, "chat error");
        }
    }
    return res;
}
#ifdef HAS_GNSS
bool link_info_diag(void){
    bool res = false;
    SpreadingFactor_t sf;
    BandWidth_t bw;
    LoRaCodingRate_t cr;
    uint16_t file_len = 0;
    uint16_t id_modulation = 0;
    LinkInfoPayload_t LinkInfoPayloadRead;
    double dist_prev = 0.0;
    double data_rate = 0.0;
    static const table_col_t cols[] = {
        {8, "SF"},
        {9, "BW"},
        {9, "CR"},
        {9, "Byte/s"},
        {9, "Dist"},
    };
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    for(sf=SF5; sf<=SF12; sf++){
        for(bw=LORA_BW_7; bw<=0x0A; bw++){
            for(cr=LORA_CR_4_5; cr<=LORA_CR_4_8; cr++){
                data_rate = lora_calc_data_rate(sf, bw, cr);
                id_modulation = param_calc_modulation_id(bw,sf,cr);
                uint16_t file_len=0;
                res = mm_get(id_modulation,
                             (uint8_t*) &LinkInfoPayloadRead,
                             sizeof(LinkInfoPayload_t),
                             &file_len);
                if(res && (sizeof(LinkInfoPayload_t)==file_len) ){
                    dist_prev = gnss_calc_distance_m( LinkInfoPayloadRead.coordinate_local,
                                                      LinkInfoPayloadRead.coordinate_remote);
                    io_printf(TSEP);
                    io_printf(" %6s " TSEP, spreading_factor2str(sf));
                    io_printf(" %7s " TSEP, bandwidth2str(bw));
                    io_printf(" %7s " TSEP, coding_rate2str(cr));
                    io_printf(" %7.1f " TSEP, data_rate/8);
                    io_printf(" %5u " TSEP, dist_prev);
                    io_printf(CRLF);
                }
            }
        }
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}
#endif /*HAS_GNSS*/

#ifdef HAS_GNSS
bool link_info_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0 == argc) {
        res = link_info_diag();
    }else{
        LOG_ERROR(SYS, "Usage: li");
    }
    return res;
}
#endif /*HAS_GNSS*/
