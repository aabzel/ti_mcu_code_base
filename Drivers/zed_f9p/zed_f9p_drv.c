#include "zed_f9p_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#include "gnss_diag.h"
#include "log.h"

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif

#ifdef HAS_PARAM
#include "param_ids.h"
#endif

#ifdef HAS_UBX_PTOTO
#include "ublox_driver.h"
#include "ubx_key_ids.h"
#endif

#include "none_blocking_pause.h"
#include "sys_config.h"
#include "zed_f9p_diag.h"

extern ZedF9P_t ZedF9P = {0};

bool zed_f9p_proc(void) {
    /*is new GNSS samples*/
    bool res = false;
    static bool first_gnss = true;
    static bool first_time = true;
    res = is_valid_time_date(&NavInfo.date_time);
    if(res) {
        if(first_time) {
            LOG_INFO(ZED_F9P, "Init time");
            print_time_date(&NavInfo.date_time);
            first_time = false;
        }
        ZedF9P.time_date = NavInfo.date_time;
    } else {
        LOG_ERROR(ZED_F9P, "InvalidUbxTimeDate");
    }

    res = is_valid_gnss_coordinates(ZedF9P.coordinate_cur);
    if(res) {
        ZedF9P.coordinate_last = ZedF9P.coordinate_cur;
    } else {
        LOG_ERROR(ZED_F9P, "Invalid GNSS cur coordinate");
    }

    res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
    if(res) {
        ZedF9P.coordinate_cur = NmeaData.coordinate_dd;
    } else {
        LOG_ERROR(ZED_F9P, "Invalid GNSS Nmea coordinate");
        res = is_valid_gnss_coordinates(NavInfo.coordinate);
        if(res) {
            if(first_gnss) {
                LOG_INFO(ZED_F9P, "Spot valid GNSS data");
                print_coordinate(NavInfo.coordinate, true);
                first_gnss = false;
            }
            ZedF9P.coordinate_cur = NavInfo.coordinate;
        } else {
            LOG_ERROR(ZED_F9P, "Invalid Ubx GNSS coordinate");
        }
    }

    return res;
}

static const keyValItem_t BaseCfgLut[] = {
 /*21*/  {CFG_UART1_BAUDRATE, 38400},
 /*1 */  {CFG_UART1INPROT_NMEA, 0},
 /*2 */  {CFG_UART1INPROT_RTCM3X, 0},
 /*6 */  {CFG_UART1INPROT_UBX, 1},
 /*4 */  {CFG_UART1OUTPROT_NMEA, 0},
 /*3 */  {CFG_UART1OUTPROT_UBX, 1},
 /*5 */  {CFG_UART1OUTPROT_RTCM3X, 1},
 /*7 */  {CFG_MSGOUT_RTCM_3X_TYPE1005_UART1, 1},
 /*8 */  {CFG_MSGOUT_RTCM_3X_TYPE1074_UART1, 1},
 /*9 */  {CFG_MSGOUT_RTCM_3X_TYPE1084_UART1, 1},
 /*10*/  {CFG_MSGOUT_RTCM_3X_TYPE1124_UART1, 1},
 /*11*/  {CFG_MSGOUT_RTCM_3X_TYPE1230_UART1, 5},
 /*12*/  {CFG_MSGOUT_RTCM_3X_TYPE1005_USB, 1},
 /*13*/  {CFG_MSGOUT_RTCM_3X_TYPE1074_USB, 1},
 /*14*/  {CFG_MSGOUT_RTCM_3X_TYPE1084_USB, 1},
 /*15*/  {CFG_MSGOUT_RTCM_3X_TYPE1124_USB, 1},
 /*16*/  {CFG_MSGOUT_RTCM_3X_TYPE1230_USB, 5},
 /*17*/  {CFG_MSGOUT_RTCM_3X_TYPE1094_UART1, 1},
 /*18*/  {CFG_MSGOUT_RTCM_3X_TYPE1094_USB, 1},
 /*19*/  {CFG_MSGOUT_UBX_NAV_PVT_USB, 1},
 /*20*/  {CFG_MSGOUT_UBX_NAV_SVIN_USB, 1},
};

bool zed_f9p_base(GnssCoordinate_t coordinate_base, double altitude_sea_lev_m) {
    bool res = false;
    res = is_valid_gnss_coordinates(coordinate_base);
    if (res) {
        uint32_t i = 0;
        ZedF9P.rtk_mode = RTK_BASE;
        res = mm_set(PAR_ID_RTK_MODE, (uint8_t*)&ZedF9P.rtk_mode, 1);

        /*
          perform settings from here
          https://www.youtube.com/watch?v=FpkUXmM7mrc

          reset to dflt cfg
         */
        for(i=0; i<ARRAY_SIZE(BaseCfgLut); i++){
            res = ubx_cfg_set_val(BaseCfgLut[i].key_id,
                                  (uint8_t *)&BaseCfgLut[i].u_value.u8[0],
                                  ubx_keyid_2len(BaseCfgLut[i].key_id),
                                  LAYER_MASK_RAM) &&
              res;
            wait_in_loop_ms(700);
        }
        /*Write base station antenna coordinates*/
        //UBX-CFG-TMODE3 (0x06 0x71)
        UbxCfgTmode3Data_t_t data;
        data.version = 0;
        data.mode = MODE_FIXED;
        data.lla = 0x01; /*Position is given in LAT/LON/ALT*/

        data.ecefXOrLat = 1e7*coordinate_base.latitude;
        data.ecefYOrLon = 1e7*coordinate_base.longitude;
        data.ecefZOrAlt = altitude_sea_lev_m*100;
        res = ubx_send_message(UBX_CLA_CFG, UBX_ID_CFG_TMODE3, (uint8_t *) &data, sizeof(data));
    }
    return res;
}

bool zed_f9p_rover(void) {
    bool res = false;
    /*
      perform settings from here
      https://www.youtube.com/watch?v=FpkUXmM7mrc
     */
    return res;
}

bool zed_f9p_init(void) {
    bool res = true;

    res = set_log_level(ZED_F9P, LOG_LEVEL_INFO);
    ZedF9P.time_zone = 0;
#if defined(HAS_FLASH_FS) && defined(HAS_PARAM)
    uint16_t file_len = 0;
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
            LOG_WARNING(ZED_F9P, "Set dflt TimeZone UTC+3");
        } else {
            LOG_ERROR(ZED_F9P, "Set dflt TimeZone Error");
        }
    }

    res = mm_get(PAR_ID_RTK_MODE, (uint8_t*)&ZedF9P.rtk_mode, 1, &file_len);
    if(res && (1 == file_len)) {
        LOG_INFO(ZED_F9P, "RTKmode:%u-%s", ZedF9P.rtk_mode, rtk_mode2str(ZedF9P.rtk_mode));
    } else {
        LOG_ERROR(ZED_F9P, "GetRTKmodeErr");
        res = false;
    }

    if(RTK_BASE==ZedF9P.rtk_mode){
        res = mm_get(PAR_ID_BASE_LOCATION, (uint8_t*)&ZedF9P.coordinate_base, sizeof(GnssCoordinate_t), &file_len);
        if(res && (16==file_len)){
            LOG_INFO(ZED_F9P, "RTKBaseLocLoadOk");
            res = print_coordinate(ZedF9P.coordinate_base, true) ;
        } else {
            LOG_ERROR(ZED_F9P, "ReadBaseLocLoadErr");
            res = false;
        }

        res = mm_get(PAR_ID_BASE_ALT, (uint8_t*)&ZedF9P.alt_base, sizeof(double), &file_len);
        if(res && (8==file_len)){
            LOG_INFO(ZED_F9P, "RTKBaseAlt: %f m",ZedF9P.alt_base);
        } else {
            LOG_ERROR(ZED_F9P, "ReadBaseAltLoadErr");
            res = false;
        }

    }




#endif /*HAS_FLASH_FS && HAS_PARAM*/
    if(res) {
        switch(ZedF9P.rtk_mode){
        case RTK_BASE:
            res = zed_f9p_base(ZedF9P.coordinate_base, ZedF9P.alt_base);
            break;
        case RTK_ROVER:
            res = zed_f9p_rover();
            break;
        default:
            res = false;
            break;
        }
    }
    return res;
}
