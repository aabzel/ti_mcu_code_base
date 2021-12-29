#include "param_ids.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "boot_driver.h"
#include "data_utils.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#include "log.h"
#include "param_types.h"
#ifdef HAS_PWR_MUX
#include "pwr_mux_diag.h"
#endif
#ifdef HAS_SX1262
#include "sx1262_diag.h"
#endif /*HAS_SX1262*/
#include "system.h"

#ifdef HAS_ZED_F9P
#include "gnss_diag.h"
#include "zed_f9p_diag.h"
#endif /*HAS_ZED_F9P*/

/*TODO: Sort by index for bin search in future*/
const ParamItem_t ParamArray[PARAM_CNT] = {
    /**/ {PAR_ID_REBOOT_CNT, 2, UINT16, "ReBootCnt"},    /*num*/
    /**/ {PAR_ID_LORA_FREQ, 4, UINT32, "LoRaFreq"},      /*Hz*/
    /**/ {PAR_ID_LORA_SF, 1, UINT8, "SF"},               /*Chips / Symbol*/
    /*4*/ {PAR_ID_LORA_CR, 1, UINT8, "CR"},               /*in raw bits/total bits*/
    /**/ {PAR_ID_LORA_BW, 1, UINT8, "BW"},               /*Hz*/
    /**/ {PAR_ID_PACKET_TYPE, 1 , UINT8, "PktType"},     /* GFSK / LoRa*/
    /**/ {PAR_ID_PREAMBLE_LENGTH, 2, UINT16, "PRE_LEN"}, /*bytes*/
    /**/ {PAR_ID_PAYLOAD_LENGTH, 1, UINT8, "PAY_LEN"},   /*bytes*/
    /**/ {PAR_ID_CRC_TYPE, 1, UINT8, "CRC_T"},
    /*10*/{PAR_ID_HEADER_TYPE, 1, UINT8, "HEAD_TYPE"},
    /**/ {PAR_ID_IQ_SETUP, 1, UINT8, "IQSetUp"},
    /**/ {PAR_ID_BOOT_CMD, 1, UINT8, "BootCmd"},                /*1-stay in boot 0-launch App*/
    /**/ {PAR_ID_BOOT_CNT, 1, UINT8, "BootCnt"},                /*num*/
    /**/ {PAR_ID_APP_START, 4, UINT32_HEX, "StartApp"},         /*Flash Addr*/
    /**/ {PAR_ID_APP_STATUS, 1, UINT8, "AppStatus"},            /*Flash Addr*/
    /**/ {PAR_ID_LORA_OUT_POWER, 1, INT8, "outPower"},          /*loRa output power*/
    /**/ {PAR_ID_PWR_SRC, 1, UINT8, "PwrSrc"},                  /*Power Source*/
    /**/ {PAR_ID_TIME_ZONE, 1, INT8, "TimeZone"},               /*Time Zone*/
    /*19*/ {PAR_ID_LORA_MAX_LINK_DIST, 8, DOUBLE, "MaxLinkDist"}, /*Max Link Distance*/
    /**/ {PAR_ID_LORA_MAX_BIT_RATE, 8, DOUBLE, "MaxBitRate"},   /*Max LoRa bit/rate*/
    /**/ {PAR_ID_BASE_LOCATION, 16, STRUCT, "BaseLocat"},
    /**/ {PAR_ID_RTK_FIX_LONG, 4, UINT32, "FTKFixedTime"}, /*RTK fixed max duration*/
    /**/ {PAR_ID_SERIAL_NUM, 4, UINT32, "SerialNum"},      /**/
    /**/ {PAR_ID_RTK_MODE, 1, UINT8, "RTKmode"},
    /*25*/ {PAR_ID_RTK_CHANNEL, 1, UINT8, "RTKchannel"},
    /**/ {PAR_ID_GNSS_PERIOD, 2, UINT16, "GnssPer"},
    /**/ {PAR_ID_BASE_ALT, 8, DOUBLE, "BaseAlt"},
    /**/ {PAR_ID_SYNC_WORD, 8, UINT64, "SyncWord"},
    /**/ {PAR_ID_LORA_SYNC_WORD, 2, UINT16, "LoRaSyncWord"},
};

bool param_init(void) {
    bool res = true;
    bool out_res = true;
    if(PARAM_CNT != ARRAY_SIZE(ParamArray)) {
        LOG_ERROR(PARAM, "Param table corrupted");
        res = false;
        out_res = false;
    } else {
        res = true;
    }
    uint16_t value_len = 0;
    uint16_t i = 0;
    for(i = 0; i < ARRAY_SIZE(ParamArray); i++) {
        uint8_t value[FLASH_FS_MAX_FILE_SIZE];
        res = mm_get(ParamArray[i].id, value, ParamArray[i].len, &value_len);
        if(false == res) {
            LOG_WARNING(PARAM, "Param %u %s lacks in Flash FS", ParamArray[i].id, ParamArray[i].name);
            res = true;
        } else {
            if(value_len != ParamArray[i].len) {
                LOG_ERROR(PARAM, "Param %u %s len error in Flash FS RealLen:%u", ParamArray[i].id, ParamArray[i].name,
                          value_len);
                res = false;
                out_res = false;
            }
        }
    }
    return out_res;
}

/*TODO: implement bin search */
ParamType_t param_get_type(Id_t id) {
    ParamType_t ret_type = UNDEF;
    uint16_t i = 0;
    for(i = 0; i < ARRAY_SIZE(ParamArray); i++) {
        if(id == ParamArray[i].id) {
            ret_type = ParamArray[i].type;
            break;
        }
    }
    return ret_type;
}

/*TODO: Test it*/
bool raw_val_2str(uint8_t* value, uint32_t value_len, ParamType_t type, char* out_str, uint32_t str_size) {
    bool res = false;
    if((NULL != value) && (NULL != out_str) && (0 < value_len) && (0 < str_size)) {
        switch(type) {
        case UINT8:
            if(1 == value_len) {
                Type8Union_t un8;
                memcpy(&un8, value, sizeof(Type8Union_t));
                snprintf(out_str, str_size, "%" PRIu8, un8.u8);
                res = true;
            }
            break;

        case INT8:
            if(1 == value_len) {
                Type8Union_t un8;
                memcpy(&un8, value, sizeof(Type8Union_t));
                snprintf(out_str, str_size, "%" PRId8, un8.s8);
                res = true;
            }
            break;

        case UINT16:
            if(2 == value_len) {
                Type16Union_t un16;
                memcpy(&un16, value, sizeof(Type16Union_t));
                snprintf(out_str, str_size, "%" PRIu16, un16.u16);
                res = true;
            }
            break;
        case INT16:
            if(2 == value_len) {
                Type16Union_t un16 = {0};
                memcpy(&un16, value, sizeof(Type16Union_t));
                snprintf(out_str, str_size, "%" PRId16, un16.s16);
                res = true;
            }
            break;
        case UINT32:
            if(4 == value_len) {
                Type32Union_t un32;
                memcpy(&un32, value, sizeof(Type32Union_t));
                snprintf(out_str, str_size, "%" PRIu32, un32.u32);
                res = true;
            }
            break;
        case UINT32_HEX:
            if(4 == value_len) {
                Type32Union_t un32;
                memcpy(&un32, value, sizeof(Type32Union_t));
                snprintf(out_str, str_size, "0x%08" PRIx32, un32.u32);
                res = true;
            }
            break;
        case INT32:
            if(4 == value_len) {
                Type32Union_t un32;
                memcpy(&un32, value, sizeof(Type32Union_t));
                snprintf(out_str, str_size, "%" PRId32, un32.s32);
                res = true;
            }
            break;
        case UINT64:
            if(8 == value_len) {
                Type64Union_t un64;
                memcpy(&un64, value, sizeof(Type64Union_t));
                snprintf(out_str, str_size, "%" PRIu64, un64.u64);
                res = true;
            }
            break;
        case INT64:
            if(8 == value_len) {
                Type64Union_t un64 = {0};
                memcpy(&un64, value, sizeof(Type64Union_t));
                snprintf(out_str, str_size, "%" PRId64, un64.s64);
                res = true;
            }
            break;
        case STRING:
            if(strlen((char*)value) < str_size) {
                snprintf(out_str, str_size, "%s", value);
                res = true;
            }
            break;
        case STRUCT:
            if(strlen((char*)value) < str_size) {
                res = hex2ascii(value, value_len, (uint8_t*)out_str, str_size);
            }
            break;
        case FLOAT:
            if(4 == value_len) {
                Type32Union_t un32;
                memcpy(&un32, value, sizeof(Type32Union_t));
                snprintf(out_str, str_size, "%.1f", un32.f32);
                res = true;
            }
            break;
        case DOUBLE:
            if(8 == value_len) {
                Type64Union_t un64;
                memcpy(&un64, value, sizeof(Type64Union_t));
                snprintf(out_str, str_size, "%.1f", un64.d64);
                res = true;
            }
            break;
        default:
            res = false;
            break;
        } /*switch*/
    }
    return res;
}

const char* param_val2str(uint16_t id, uint8_t* value, uint32_t size) {
    const char* name = "---";
    if (value && (0<size)) {

#ifdef HAS_SX1262
        static char temp_name[100] = "---";
        if(PAR_ID_LORA_OUT_POWER == id) {
            float watts = dbm2watts((uint32_t)*value);
            snprintf(temp_name, sizeof(temp_name), "%f7.3 W", watts);
            name = temp_name;
        }
        if( PAR_ID_LORA_FREQ == id){
            uint32_t freq=0;
            memcpy(&freq,value,4);
            name =  RfFreq2Str(freq);
        }
        if(PAR_ID_LORA_SYNC_WORD==id){
            name = LoRaSyncWord2Str ((uint16_t)*value);
        }

        if(PAR_ID_PACKET_TYPE == id){
            name = PacketType2Str((uint8_t)*value);
        }

        if(PAR_ID_PAYLOAD_LENGTH == id){
            name = PayloadLen2Str((uint8_t)*value);
        }

        if(PAR_ID_PREAMBLE_LENGTH == id){
            name = PreambleLen2Str ((uint16_t)*value);
        }

        if( PAR_ID_IQ_SETUP == id){
            name = IqSetUp2Str((uint8_t)*value);
        }
        if(PAR_ID_HEADER_TYPE == id) {
            name = LoraHeaderType2Str((uint8_t)*value);
        }
        if(PAR_ID_CRC_TYPE == id) {
            name = LoraCrcType2Str((uint8_t)*value);
        }
        if(PAR_ID_LORA_MAX_LINK_DIST== id){
            name = LinkDist2str((double)*value);
        }
        if(PAR_ID_LORA_SF == id) {
            name = spreading_factor2str((uint8_t)*value);
        }
        if(PAR_ID_LORA_CR == id) {
            name = coding_rate2str((LoRaCodingRate_t)*value);
        }
        if(PAR_ID_LORA_MAX_BIT_RATE == id) {
            Type64Union_t un64;
            memcpy(&un64, value, sizeof(Type64Union_t));
            name = BitRate2Str(un64.d64);
        }
        if(PAR_ID_LORA_BW == id) {
            name = bandwidth2str ((BandWidth_t)*value);
        }
#endif /*HAS_SX1262*/
        if(PAR_ID_BOOT_CMD == id) {
            name = boot_cmd2str((uint8_t)*value);
        }

#ifdef HAS_PWR_MUX
        if(PAR_ID_PWR_SRC == id) {
            name = pwr_source2str((PwrSource_t)*value);
        }
#endif /*HAS_PWR_MUX*/

    #ifdef HAS_ZED_F9P
        if(PAR_ID_RTK_MODE == id) {
            name = rtk_mode2str((uint8_t)*value);
        }
        if(  PAR_ID_RTK_FIX_LONG== id) {
            uint32_t time_ms=0;
            memcpy(&time_ms,value,4);
            name =  RtkFixDur2str(time_ms);
        }
        if(PAR_ID_RTK_CHANNEL == id) {
            name = interface2str((Interfaces_t)*value);
        }
        if(PAR_ID_BASE_LOCATION == id) {
            name = coordinate2str((void*)value);
        }
    #endif /*HAS_ZED_F9P*/
    }
    return name;
}

bool param_proc(void) {
    bool res = false;
    /*Syn params between flash and RAM*/
    return res;
}
