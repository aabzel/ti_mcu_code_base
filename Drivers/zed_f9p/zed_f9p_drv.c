#include "zed_f9p_drv.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_CALENDAR
#include "calendar.h"
#endif
#include "cli_manager.h"
#include "clocks.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#include "gnss_diag.h"
#include "log.h"
#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif
#include "none_blocking_pause.h"
#ifdef HAS_PARAM
#include "param_ids.h"
#endif
#include "sys_config.h"
#include "task_config.h"
#include "task_info.h"
#include "time_diag.h"
#include "time_utils.h"
#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif
#ifdef HAS_RS232
#include "rs232_drv.h"
#endif
#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif
#include "uart_common.h"
#ifdef HAS_UBX_PTOTO
#include "ublox_driver.h"
#include "ubx_key_ids.h"
#include "ubx_protocol.h"
#endif
#include "writer_config.h"
#include "zed_f9p_diag.h"

extern ZedF9P_t ZedF9P = {0};

static bool zed_f9p_proc_base(void) {
    bool res = false;
#ifdef HAS_SX1262
    Sx1262Instance.sync_reg = false;
#endif

    res = is_valid_gnss_coordinates(ZedF9P.coordinate_base);
    if(res) {
        ZedF9P.coordinate_cur = ZedF9P.coordinate_base;
    } else {
        LOG_ERROR(ZED_F9P, "InvalGnssBaseDot");
    }

    if(task_data[TASK_ID_NMEA].on) {
        task_data[TASK_ID_NMEA].on = false;
    }
    if(task_data[TASK_ID_UBX].on) {
        task_data[TASK_ID_UBX].on = false;
    }

    if(IF_RS232 == ZedF9P.channel) {
        res = cli_set_echo(false);
#ifdef HAS_RTCM3
        Rtcm3Protocol[IF_UART1].lora_fwd = false;
        Rtcm3Protocol[IF_UART1].rs232_fwd = true;
#endif
        task_data[TASK_ID_RS232].on = true;
#ifdef HAS_RS232
        rs232.ping = true;
#endif
    }

    if(IF_LORA == ZedF9P.channel) {
#ifdef HAS_RTCM3
        Rtcm3Protocol[IF_UART1].lora_fwd = true;
        Rtcm3Protocol[IF_UART1].rs232_fwd = false;
#endif
        task_data[TASK_ID_RS232].on = false;
#ifdef HAS_RS232
        rs232.ping = false;
#endif
    }

    return res;
}

#ifdef HAS_NMEA
bool zed_f9p_uptate_nmea(void) {
    bool res = true, res_time = false, res_dot = false;
    static bool first_time = true;
    static bool first_gnss = true;
    res = is_valid_time_date(&NmeaData.time_date);
    if(res) {
        if(first_time) {
            LOG_INFO(ZED_F9P, "SpotValidTime!");
#ifdef HAS_CALENDAR
            calendar_settime(&NmeaData.time_date);
#endif /*HAS_CALENDAR*/
            print_time_date(&NmeaData.time_date);
            first_time = false;
        }
        ZedF9P.time_date = NmeaData.time_date;
        res_time = true;
    } else {
        LOG_ERROR(ZED_F9P, "InvalNmeaTimeDate");
    }

    res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
    if(res) {
        if(first_gnss) {
            LOG_INFO(ZED_F9P, "SpotValidGNSSData!");
            print_coordinate(NmeaData.coordinate_dd, true);
            first_gnss = false;
        }
        ZedF9P.coordinate_cur = NmeaData.coordinate_dd;
        res_dot = true;
    } else {
        LOG_ERROR(ZED_F9P, "InvalNmeaGNSSDot");
    }

    if(res_time && res_dot) {
        res = true;
    } else {
        res = false;
    }
    return res;
}
#endif

static bool zed_f9p_proc_rover(void) {
    bool res = true;
    if(task_data[TASK_ID_UBX].on) {
        task_data[TASK_ID_UBX].on = false;
    }
#ifdef HAS_NMEA
    res = zed_f9p_uptate_nmea();
    task_data[TASK_ID_NMEA].on = true;
#endif

    if(IF_RS232 == ZedF9P.channel) {
        res = cli_set_echo(false);
#ifdef HAS_RS232
        rs232.ping = true;
#endif
    }
    if(IF_LORA == ZedF9P.channel) {
#ifdef HAS_LORA
        task_data[TASK_ID_LORA].on = false;
#endif
#ifdef HAS_RS232
        task_data[TASK_ID_RS232].on = false;
        rs232.ping = false;
#endif
    }

    return res;
}

static bool zed_f9p_proc_none(void) {
    bool res = false;
    task_data[TASK_ID_NMEA].on = true;
    task_data[TASK_ID_UBX].on = false;

#ifdef HAS_NMEA
    res = zed_f9p_uptate_nmea();

#endif
    return res;
}

bool zed_f9p_proc(void) {
    /*is new GNSS samples*/
    bool res = false;

    switch(ZedF9P.rtk_mode) {
    case RTK_BASE_SURVEY_IN:
    case RTK_BASE_FIX:{
        res = zed_f9p_proc_base();
    } break;

    case RTK_ROVER: {
        res = zed_f9p_proc_rover();
    } break;
    case RTK_NONE: {
        res = zed_f9p_proc_none();
    } break;
    default:
        res = false;
        break;
    }

    res = is_valid_gnss_coordinates(ZedF9P.coordinate_cur);
    if(res) {
        ZedF9P.coordinate_last = ZedF9P.coordinate_cur;
    } else {
        LOG_ERROR(ZED_F9P, "Inval GNSS cur coordinate");
    }

    return res;
}
#ifdef HAS_UBLOX
static const keyValItem_t BaseCfgLut[] = {
    /*21*/ {CFG_UART1_BAUDRATE, 38400},
    /*1 */ {CFG_UART1INPROT_NMEA, 0},
    /*2 */ {CFG_UART1INPROT_RTCM3X, 0},
    /*6 */ {CFG_UART1INPROT_UBX, 1},
    /*4 */ {CFG_UART1OUTPROT_NMEA, 0},
    /*5 */ {CFG_UART1OUTPROT_RTCM3X, 1},
    /*3 */ {CFG_UART1OUTPROT_UBX, 1},
    /*7 */ {CFG_MSGOUT_RTCM_3X_TYPE1005_UART1, 1},
    /*8 */ {CFG_MSGOUT_RTCM_3X_TYPE1074_UART1, 1},
    /*9 */ {CFG_MSGOUT_RTCM_3X_TYPE1084_UART1, 1},
    /*10*/ {CFG_MSGOUT_RTCM_3X_TYPE1124_UART1, 1},
    /*11*/ {CFG_MSGOUT_RTCM_3X_TYPE1230_UART1, 5},
    /*12*/ {CFG_MSGOUT_RTCM_3X_TYPE1005_USB, 1},
    /*13*/ {CFG_MSGOUT_RTCM_3X_TYPE1074_USB, 1},
    /*14*/ {CFG_MSGOUT_RTCM_3X_TYPE1084_USB, 1},
    /*15*/ {CFG_MSGOUT_RTCM_3X_TYPE1124_USB, 1},
    /*16*/ {CFG_MSGOUT_RTCM_3X_TYPE1230_USB, 5},
    /*17*/ {CFG_MSGOUT_RTCM_3X_TYPE1094_UART1, 1},
    /*18*/ {CFG_MSGOUT_RTCM_3X_TYPE1094_USB, 1},
    /*19*/ {CFG_MSGOUT_UBX_NAV_PVT_USB, 1},
    /*20*/ {CFG_MSGOUT_UBX_NAV_SVIN_USB, 1},
};
#endif

#define METER_TO_MM(METER) (METER * 1000)

bool zed_f9p_deploy_base(GnssCoordinate_t coordinate_base, double altitude_sea_lev_m, RTKmode_t receiver_mode) {
    bool res = false, out_res = true;
    res = is_valid_gnss_coordinates(coordinate_base);
    if(res) {
        res = false;
#ifdef HAS_UBLOX
        task_data[TASK_ID_UART0_FWD].on = false;
        task_data[TASK_ID_UART1_FWD].on = false;
        /*
          perform settings from here
          https://www.youtube.com/watch?v=FpkUXmM7mrc

          reset to dflt cfg
         */
        res = ubx_reset_to_dflt();
        if(false == res) {
            LOG_ERROR(ZED_F9P, "Set Dflt error");
            out_res = false;
        }
        uint32_t i = 0;
        for(i = 0; i < ARRAY_SIZE(BaseCfgLut); i++) {
            res = ubx_cfg_set_val(BaseCfgLut[i].key_id, (uint8_t*)&BaseCfgLut[i].u_value.u8[0],
                                  ubx_keyid_2len(BaseCfgLut[i].key_id), LAYER_MASK_RAM);
            if(false == res) {
                LOG_ERROR(ZED_F9P, "Set 0x%x error", BaseCfgLut[i].key_id);
                out_res = false;
            }
        }
        /*Write base station antenna coordinates*/
        // UBX-CFG-TMODE3 (0x06 0x71)
        UbxCfgTmode3Data_t_t data;
        data.version = 0;
        data.svin_min_dur_s = MIN_2_SEC(4);
        data.mode = receiver_mode; /**/
        data.lla = 0x01; /*Position is given in LAT/LON/ALT*/

        data.ecefXOrLat = 1e7 * coordinate_base.latitude;
        data.ecefYOrLon = 1e7 * coordinate_base.longitude;
        data.ecefZOrAlt = altitude_sea_lev_m * 100;
        data.fixedPosAcc = METER_TO_MM(1);
        res = ubx_send_message_ack(UBX_CLA_CFG, UBX_ID_CFG_TMODE3, (uint8_t*)&data, sizeof(data));
        if(false == res) {
            LOG_ERROR(ZED_F9P, "SetBaseDotErr");
            out_res = false;
        } else {
            LOG_INFO(ZED_F9P, "SetBaseDotOk");
        }

        ZedF9P.rtk_mode = receiver_mode;
        task_data[TASK_ID_NMEA].on = false;
#ifdef HAS_RTCM3
        if(IF_LORA == ZedF9P.channel) {
            Rtcm3Protocol[IF_UART1].lora_fwd = true;
            Rtcm3Protocol[IF_UART1].rs232_fwd = false;
        }
        if(IF_RS232 == ZedF9P.channel) {
            Rtcm3Protocol[IF_UART1].lora_fwd = false;
            Rtcm3Protocol[IF_UART1].rs232_fwd = true;
        }
#endif /*HAS_RTCM3*/
#endif /*HAS_UBLOX*/
    } else {
        LOG_ERROR(ZED_F9P, "InvalBaseGNSScoordinate");
    }
    return out_res;
}
#ifdef HAS_UBLOX
static const keyValItem_t RoverCfgLut[] = {
    {CFG_UART1_BAUDRATE, 38400}, {CFG_UART1INPROT_UBX, 1},   {CFG_UART1INPROT_NMEA, 0},    {CFG_UART1INPROT_RTCM3X, 1},
    {CFG_UART1OUTPROT_UBX, 1},   {CFG_UART1OUTPROT_NMEA, 1}, {CFG_UART1OUTPROT_RTCM3X, 0}, {CFG_USBINPROT_UBX, 1},
    {CFG_USBINPROT_NMEA, 1},     {CFG_USBINPROT_RTCM3X, 1},  {CFG_USBOUTPROT_UBX, 1},      {CFG_USBOUTPROT_RTCM3X, 0},
};
#endif

bool zed_f9p_deploy_rover(void) {
    bool res = false, out_res = true;
#ifdef HAS_UBLOX
#ifdef HAS_SX1262
    Sx1262Instance.sync_reg = false;
#endif
    task_data[TASK_ID_PARAM].on = false;
    task_data[TASK_ID_UBX].on = false;
    /*
      perform settings from here
      https://www.youtube.com/watch?v=FpkUXmM7mrc
     */
    res = ubx_reset_to_dflt();
    if(false == res) {
        LOG_ERROR(ZED_F9P, "Set Dflt error");
        // out_res = false;
    }

    uint32_t i = 0;
    for(i = 0; i < ARRAY_SIZE(RoverCfgLut); i++) {
        res = ubx_cfg_set_val(RoverCfgLut[i].key_id, (uint8_t*)&RoverCfgLut[i].u_value.u8[0],
                              ubx_keyid_2len(RoverCfgLut[i].key_id), LAYER_MASK_RAM);
        if(false == res) {
            LOG_ERROR(ZED_F9P, "Key:0x%08x set Error", RoverCfgLut[i].key_id);
            out_res = false;
        }
    }
    /*adjust output rate*/
    res = ubx_set_rate(ZedF9P.rate_ms, TIME_UTC);
    if(false == res) {
        LOG_ERROR(ZED_F9P, "SetRateUtcErr");
        out_res = false;
    }
    res = ubx_set_rate(ZedF9P.rate_ms, TIME_GPS);
    if(false == res) {
        LOG_ERROR(ZED_F9P, "SetRateGpsErr");
        out_res = false;
    }
    res = ubx_set_rate(ZedF9P.rate_ms, TIME_GLONASS);
    if(false == res) {
        LOG_ERROR(ZED_F9P, "SetRateGloErr");
        out_res = false;
    }
    res = ubx_set_rate(ZedF9P.rate_ms, TIME_GALILEO);
    if(false == res) {
        LOG_ERROR(ZED_F9P, "SetRateGalErr");
        out_res = false;
    }
    res = ubx_set_rate(ZedF9P.rate_ms, TIME_BEIDOU);
    if(false == res) {
        LOG_ERROR(ZED_F9P, "SetRateBeErr");
        out_res = false;
    }
#endif
    return out_res;
}

bool zed_f9p_load_params(void) {
    bool res = true;
#if defined(HAS_FLASH_FS) && defined(HAS_PARAM)
    uint16_t file_len = 0;

    ZedF9P.time_zone = 0;
    res = mm_get(PAR_ID_TIME_ZONE, (uint8_t*)&ZedF9P.time_zone, 1, &file_len);
    if(res && (1 == file_len)) {
        ZedF9P.is_init = true;
        LOG_INFO(ZED_F9P, "Read TimeZone UTC%u", ZedF9P.time_zone);
    } else {
        res = false;
        LOG_ERROR(ZED_F9P, "Read TimeZone Error");
        ZedF9P.time_zone = DFLT_TIME_ZONE;
        res = mm_set(PAR_ID_TIME_ZONE, (uint8_t*)&ZedF9P.time_zone, 1);
        if(res) {
            LOG_WARNING(ZED_F9P, "SetDfltTimeZone UTC+3");
        } else {
            LOG_ERROR(ZED_F9P, "SetDfltTimeZoneError");
        }
    }
    res = mm_get(PAR_ID_RTK_MODE, (uint8_t*)&ZedF9P.rtk_mode, 1, &file_len);
    if(res && (1 == file_len)) {
        LOG_INFO(ZED_F9P, "RTKmode:%u-%s", ZedF9P.rtk_mode, rtk_mode2str(ZedF9P.rtk_mode));
    } else {
        LOG_ERROR(ZED_F9P, "GetRTKmodeErr");
        res = false;
    }

    res = mm_get(PAR_ID_RTK_CHANNEL, (uint8_t*)&ZedF9P.channel, 1, &file_len);
    if(res && (1 == file_len)) {
        LOG_INFO(ZED_F9P, "RTKchannel:%u-%s", ZedF9P.channel, interface2str(ZedF9P.channel));
    } else {
        LOG_ERROR(ZED_F9P, "GetRTKchanErr");
        res = false;
    }

    switch(ZedF9P.rtk_mode) {
    case RTK_BASE_SURVEY_IN :
        break;
    case RTK_BASE_FIX:{
        res = mm_get(PAR_ID_BASE_LOCATION, (uint8_t*)&ZedF9P.coordinate_base, sizeof(GnssCoordinate_t), &file_len);
        if(res && (16 == file_len)) {
            LOG_INFO(ZED_F9P, "RTKBaseLocLoadOk");
            res = print_coordinate(ZedF9P.coordinate_base, true);
        } else {
            LOG_ERROR(ZED_F9P, "ReadBaseLocLoadErr");
            res = false;
        }

        res = mm_get(PAR_ID_BASE_ALT, (uint8_t*)&ZedF9P.alt_base, sizeof(double), &file_len);
        if(res && (8 == file_len)) {
            LOG_INFO(ZED_F9P, "RTKBaseAlt: %f m", ZedF9P.alt_base);
        } else {
            LOG_ERROR(ZED_F9P, "ReadBaseAltLoadErr");
            res = false;
        }

    } break;
    case RTK_ROVER: {
        res = mm_get(PAR_ID_GNSS_PERIOD, (uint8_t*)&ZedF9P.rate_ms, 2, &file_len);
        if(res && (2 == file_len)) {
            LOG_INFO(ZED_F9P, "GNSS period %u ms", ZedF9P.rate_ms);
        } else {
            LOG_ERROR(ZED_F9P, "GnssPerLoadErr");
            ZedF9P.rate_ms = DFLT_GNSS_PER_MS;
            res = mm_set(PAR_ID_GNSS_PERIOD, (uint8_t*)&ZedF9P.rate_ms, 2);
        }

    } break;
    default:
        break;
    }

#endif /*HAS_FLASH_FS && HAS_PARAM*/
    return res;
}

bool zed_f9p_init(void) {
    bool res = true;
    res = set_log_level(ZED_F9P, LOG_LEVEL_INFO);
    ZedF9P.rate_ms = DFLT_GNSS_PER_MS;
    ZedF9P.channel = IF_LORA;
    res = zed_f9p_load_params();
    if(res) {
        switch(ZedF9P.rtk_mode) {
        case RTK_NONE:
            res = true;
            break;
        case RTK_BASE_SURVEY_IN:
        case RTK_BASE_FIX:
            res = zed_f9p_deploy_base(ZedF9P.coordinate_base, ZedF9P.alt_base, ZedF9P.rtk_mode);
            break;
        case RTK_ROVER:
            res = zed_f9p_deploy_rover();
            break;
        default:
            res = false;
            break;
        }
    } else {
        LOG_ERROR(ZED_F9P, "ParLoadErr");
    }
    return res;
}
