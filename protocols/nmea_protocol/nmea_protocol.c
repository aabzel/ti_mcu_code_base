#include "nmea_protocol.h"

#include <string.h>
#ifdef X86_64
#include <stdio.h>
#endif

#ifdef HAS_CLOCK
#include "clocks.h"
#endif
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#ifdef HAS_PARAM
#include "param_ids.h"
#endif
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif
#ifdef HAS_LED
#include "led_drv.h"
#endif

#include "gnss_utils.h"
#ifdef HAS_NMEA_DIAG
#include "nmea_diag.h"
#endif
#include "convert.h"
#include "time_utils.h"

NmeaProtocol_t NmeaProto = {0};
NmeaData_t NmeaData = {0};


static bool nmea_reset_parser(void){
    bool res = true;
    LOG_DEBUG(NMEA,"ResetParser");
    NmeaProto.pos = 0;
    NmeaProto.got_massege = false;
    NmeaProto.pos_mode = PM_UNDEF;
    return res;
}

bool nmea_init(void) {
    bool res = false;
    LOG_INFO(NMEA,"Init");
    memset(&NmeaData, 0x00, sizeof(NmeaData));
    memset(&NmeaProto, 0x00, sizeof(NmeaProto));
    res = set_log_level(NMEA, LOG_LEVEL_INFO);
    res=nmea_reset_parser();
    return res;
}

/*$GNZDA,122013.00,29,11,2021,00,00*71*/
bool gnss_parse_zda(char* nmea_msg, zda_t* zda) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') ;
    // 122013.00,29,11,2021,00,00*71*/
    if(ptr){
        uint32_t utc_time = 0;
        res = try_strl2uint32(ptr+1, 6, &utc_time) && res;
        if(res) {
            res = parse_time_from_val(utc_time, &zda->time_date);
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        /*29,11,2021,00,00*71*/
        res = try_strl2int32(ptr+1, 2, &zda->time_date.tm_mday) && res;
    }
    ptr++;

    ptr = strchr(ptr, ',');
    if(ptr){
        /*11,2021,00,00*71*/
        res = try_strl2int32(ptr+1, 2, &zda->time_date.tm_mon) && res;
    }
    ptr++;


    ptr = strchr(ptr, ',');
    if(ptr){
        /*2021,00,00*71*/
        res = try_strl2int32(ptr+1, 4, &zda->time_date.tm_year) && res;
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    /*00,00*71*/
    if(ptr){
        res = try_strl2uint8(ptr+1, 2, &zda->ltzh) && res;
    }
    ptr++;


    ptr = strchr(ptr, ',') ;
    /*00*71*/
    res = try_strl2uint8(ptr+1, 2, &zda->ltzn) && res;
    return res;
}

#ifdef HAS_NMEA_GGA
/*$GNGGA,140213.00,5540.70555,N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42*/
bool gnss_parse_gga(char* nmea_msg, gga_t* gga) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    uint32_t utc_time = 0;
    // 140213.00,5540.70555,N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2uint32(ptr, 6, &utc_time) && res;
    if(res) {
        res = parse_time_from_val(utc_time, &gga->time_date);
    }

    ptr = strchr(ptr, ',') + 1;
    // 5540.70555,N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 10, &gga->coordinate_ddmm.latitude) && res;

    ptr = strchr(ptr, ',') + 1;
    // N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    gga->lat_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 11, &gga->coordinate_ddmm.longitude) && res;

    ptr = strchr(ptr, ',') + 1;
    // E,1,12,0.58,201.4,M,13.3,M,,*42
    gga->lon_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2uint16(ptr, 1, &gga->quality) && res;

    ptr = strchr(ptr, ',') + 1;
    // 12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2uint16(ptr, 2, &gga->nb_sat) && res;

    ptr = strchr(ptr, ',') + 1;
    // 0.58,201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 4, &gga->hdop) && res;

    ptr = strchr(ptr, ',') + 1;
    // 201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 5, &gga->height) && res;

    ptr = strchr(ptr, ',') + 1;
    // M,13.3,M,,*42
    gga->height_unit = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 13.3,M,,*42
    res = try_strl2double(ptr, 4, &gga->geoid_separation) && res;

    ptr = strchr(ptr, ',') + 1;
    // M,,*42
    gga->geoid_unit = ptr[0];

    return res;
}
#endif

#ifdef HAS_NMEA_GSV
//$GLGSV,3,3,10, 85,46,314,38,
//               86,04,339,,3*7A
//"$GBGSV,3,3,09,35,18,108,,3*4B",
bool gnss_parse_gsv(char* nmea_msg, gsv_t* gsv) {
    bool res = true;
    LOG_DEBUG(NMEA,"GSV [%s]",nmea_msg);
    uint8_t cnt=0;
    char* ptr = strchr(nmea_msg, ',') ;
    if(ptr){
        res = try_strl2uint8(ptr+1, 1, &gsv->numMsg) ;
        if(res){
            LOG_DEBUG(NMEA,"numMsg Ok");
            cnt++;
        }
    }
    if(ptr){
       ptr++;
    }

    ptr = strchr(ptr, ',') ;
    if(ptr){
        res = try_strl2uint8(ptr+1, 1, &gsv->msgNum) ;
        if(res){
            cnt++;
        }
    }
    if(ptr){
       ptr++;
    }

    ptr = strchr(ptr, ',') ;
    if(ptr){
        res = try_strl2uint16(ptr+1, 2, &gsv->numSV) ;
        if(res){
                 cnt++;
         }
    }
    if(ptr){
       ptr++;
    }
    uint8_t i = 0;
    LOG_DEBUG(NMEA,"Iterations %u",gsv->msgNum - 1);
    for(i = 0; i < (gsv->msgNum - 1); i++) {
        if(i < NUM_OF_PARSED_SAT) {
            ptr = strchr(ptr, ',') ;
            if(ptr){
                LOG_DEBUG(NMEA,"ParseSatelliteID");
                res = try_strl2uint16(ptr+1, 2, &gsv->sat[i].svid) ;
                if(res){
                     cnt++;
                }
            }
            if(ptr){
                LOG_DEBUG(NMEA,"IncrPtr L:%u",__LINE__);
               ptr++;
            }
            if(ptr){

                ptr = strchr(ptr, ',') ;
                if(ptr){
                    LOG_DEBUG(NMEA,"ParseElv");
                    res = try_strl2uint8(ptr+1, 2, &gsv->sat[i].elv) ;
                    if(res){
                             cnt++;
                    }
                }
            }
            if(ptr){
               ptr++;
            }

            if(ptr ){
                ptr = strchr(ptr, ',');
                if(ptr){
                    LOG_DEBUG(NMEA,"ParseAz");
                    res = try_strl2uint16(ptr+1, 3, &gsv->sat[i].az) ;
                    if(res){
                        cnt++;
                    }
                }
            }
            if(ptr){
                ptr++;
            }

            if(ptr){
                ptr = strchr(ptr, ',') ;
                if(ptr){
                    LOG_DEBUG(NMEA,"ParseSignalStrength");
                    res = try_strl2uint8(ptr+1, 2, &gsv->sat[i].cno) ;
                    if(res){
                             cnt++;
                    }
                }
            }
            if(ptr){
               ptr++;
            }
        }
    }
    if(ptr){
        ptr = strchr(ptr, ',') ;
        if(ptr){
            LOG_DEBUG(NMEA,"ParseSignalID");
            res = try_strl2uint8(ptr+1, 1, &gsv->signalId) ;
            if(res){
               cnt++;
            }
        }
    }

    res = false;
    if(cnt){
        res = true;
    }
    return res;
}
#endif


//$GNVTG,,T,,M,0.019,N,0.036,K,A*30
//$GPVTG,,,,,,,,,N*30
//$GNVTG,,T,,M,0.017,N,0.032,K,A*3A
bool gnss_parse_vtg(char* nmea_msg, vtg_t* vtg) {
    bool res = true;
    uint8_t cnt = 0;
    char* ptr = strchr(nmea_msg, ',') ;
    //,T,,M,0.019,N,0.036,K,A*30  //Course over ground
    if(ptr){
        res = try_strl2double(ptr+1, 5, &vtg->cogt) ;
        if(res){
            cnt++;
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // T,,M,0.019,N,0.036,K,A*30  Course over ground units
    if(ptr){
       vtg->cogtUnit = ptr[1];
       LOG_DEBUG(NMEA,"%c",ptr[1]);
    }
    ptr++;

    ptr = strchr(ptr, ',') + 1;
    //,M,0.019,N,0.036,K,A*30  Course over ground
    if(ptr){
        res = try_strl2double(ptr+1, 5, &vtg->cogm) ;
        if(res){
            cnt++;
        }
    }
    ptr = strchr(ptr, ',');
    // M,0.019,N,0.036,K,A*30   Course over ground units
    if(ptr){
        vtg->cogmUnit = ptr[1];
        LOG_DEBUG(NMEA,"%c",ptr[1]);
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // 0.019,N,0.036,K,A*30  Speed over ground
    if(ptr){
       res = try_strl2double(ptr+1, 5, &vtg->sogn) ;
       if(res){
           cnt++;
       }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // N,0.036,K,A*30  Speed over ground  Speed over ground units
    if(ptr){
        vtg->sognUnit = ptr[1];
        LOG_DEBUG(NMEA,"%c",ptr[1]);
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // 0.036,K,A*30  Speed over ground
    if(ptr){
        res = try_strl2double(ptr+1, 5, &vtg->sogk);
        if(res){
            cnt++;
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // K,A*30     Speed over ground units
    if(ptr){
       vtg->sogkUnit = ptr[1];
       LOG_DEBUG(NMEA,"%c",ptr[1]);
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // A*30
    if(ptr){
        vtg->posMode = ptr[1];
        LOG_DEBUG(NMEA,"%c",ptr[1]);
    }

    if(cnt){
        res= true;
    }else{
        res = false;
    }
    return res;
}

/* GNSS DOP and active satellites */
//$GNGSA,A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B //18 commas
//"$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30"  //17 commas
bool gnss_parse_gsa(char* nmea_msg, gsa_t* gsa) {
    bool res = true;

    char* ptr = strchr(nmea_msg, ',') + 1;
    // A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B
    gsa->opMode = ptr[0];

    ptr = strchr(ptr, ',');
    // 3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B
    if(ptr){
        res = try_strl2uint8(ptr+1, 1, &gsa->navMode) && res;
    }
    ptr++;

    uint8_t i = 0;
    for(i = 0; i < 12; i++) {
        ptr = strchr(ptr, ',') + 1;
    }

    ptr = strchr(ptr, ',');
    // 1.04,0.58,0.86,2*0B
    if(ptr){
        res = try_strl2double(ptr+1, 4, &gsa->PDOP) && res;
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // 0.58,0.86,2*0B
    if(ptr){
        res = try_strl2double(ptr+1, 4, &gsa->HDOP) && res;
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // 0.86,2*0B
    if(ptr){
        res = try_strl2double(ptr+1, 4, &gsa->VDOP) && res;
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    // 2*0B
    if(ptr){
        res = try_strl2uint8(ptr+1, 1, &gsa->systemId) && res;
    }
    return res;
}
#ifdef HAS_NMEA_GLL

// Latitude and longitude, with time of position fix and status
//$GNGLL,5540.70588,N,03737.93396,E,140121.00,A,A*70
//$GPGLL,,,,,213748.00,V,N*41
bool gnss_parse_gll(char* nmea_msg, gll_t* gll) {
    bool res = true;
    uint8_t cnt=0;
    uint32_t cur_pos = 0;
    if(nmea_msg && gll) {
        char* ptr = strchr(nmea_msg, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if(cur_pos < NMEA_MSG_SIZE) {
            // 5540.70588,N,03737.93396,E,140121.00,A,A*70
            res = try_strl2double(ptr+1, 10, &gll->coordinate_ddmm.latitude) ;
            if(res){
                cnt++;
            }
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // N,03737.93396,E,140121.00,A,A*70
            gll->lat_dir = ptr[1];
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // 03737.93396,E,140121.00,A,A*70
            res = try_strl2double(ptr+1, 11, &gll->coordinate_ddmm.longitude) ;
            if(res){
                         cnt++;
                     }
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // E,140121.00,A,A*70
            gll->lon_dir = ptr[1];
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // 140121.00,A,A*70
            uint32_t utc_time = 0;
            res = try_strl2uint32(ptr+1, 6, &utc_time) ;
            if(res) {
                res = parse_time_from_val(utc_time, &gll->time_date);
                if(res){
                    cnt++;
                }
            }
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // A,A*70
            gll->status = ptr[1];
        }
        ptr++;

        ptr = strchr(ptr, ',') ;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // A*70
            gll->pos_mode = ptr[1];
        }
        ptr++;

        if(cnt){
            res = true;
        }else{
            res = false;
        }
    }

    return res;
}
#endif

//$PUBX,00,000717.00,0000.00000,N,00000.00000,E,0.000,NF,5303316,3750010,0.000,0.00,0.000,,99.99,99.99,99.99,0,0,0*2C
bool gnss_parse_pbux_pos(char* nmea_msg, pbux_t* const pbux) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    res = try_strl2uint8(ptr, 2, &pbux->msg_id) && res;
    ptr = strchr(ptr, ',') + 1;
    uint32_t utc_time = 0;
    res = try_strl2uint32(ptr, 6, &utc_time) && res;
    if(res) {
        res = parse_time_from_val(utc_time, &pbux->time_date);
    }
    return res;
}
#ifdef HAS_NMEA_RMC
/*
$GNRMC,142710.60,A,5551.84214,N,03725.61511,E,0.015,,301121,,,D,V*1C
$GNRMC,072316.27,A,5551.85875,N,03755.65965,E,0.010,,290721,11.73,E,A,V*76
$GNRMC,163320.60,A,5540.70459,N,03737.93393,E,0.047,,140222,,,A,V*1B
*/
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc) {
    bool res = true;
    bool out_res = false;
    uint8_t parse_cnt=0;
    char* ptr = strchr(nmea_msg, ',') ;
    uint32_t utc_time = 0;
    if(ptr){
       res = try_strl2uint32(ptr+1, 6, &utc_time);
       if(res) {
           LOG_DEBUG(NMEA,"SpotTime");
           res = parse_time_from_val(utc_time, &rmc->time_date);
           if(false==res){

           }else{
               parse_cnt++;
           }
       }else{
           LOG_ERROR(NMEA,"ParseTimeErr");
       }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->data_valid = ptr[1];
    }
    ptr++;

    ptr = strchr(ptr, ',');
    if(ptr){
        res = try_strl2double(ptr+1, 10, &rmc->coordinate_ddmm.latitude) ;
        if(false==res) {

        }else{
            parse_cnt++;
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->lat_dir = ptr[1];
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        res = try_strl2double(ptr+1, 11, &rmc->coordinate_ddmm.longitude);
        if(false==res){

        }else{
            parse_cnt++;
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->lon_dir = ptr[1];
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        res = try_strl2double(ptr+1, 5, &rmc->speed_knots) ;
        if(res){
            parse_cnt++;
        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
       res = try_strl2double(ptr+1, 5,&rmc->true_course);
       if(res){
           parse_cnt++;
       }
    }
    ptr++;

    ptr = strchr(ptr, ',');
    uint32_t date = 0;
    if(ptr){
        LOG_DEBUG(NMEA,"SpotData");
        res = try_strl2uint32(ptr+1, 6, &date) ;
        if(res) {
            res = parse_date_from_val(date, &rmc->time_date);
            if(false==res) {
               LOG_ERROR(NMEA,"ParseDateErr");

            }else{
                parse_cnt++;
            }
        }else{
            LOG_ERROR(NMEA,"SpotDateErr [%s]",ptr+1);

        }
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        try_strl2double(ptr+1, 5, &rmc->mv) ;
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->mv_ew = ptr[1];
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->pos_mode = ptr[1];
    }
    ptr++;

    ptr = strchr(ptr, ',') ;
    if(ptr){
        rmc->nav_status = ptr[1];
    }

    if(0<parse_cnt){
        out_res=true;
    }else{
        out_res=false;
    }
    return out_res;
}
#endif

uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len) {
    uint8_t crc8 = 0;
    uint16_t i = 0;
    if(nmea_data){
      for(i = 0; i < len; i++) {
          crc8 ^= nmea_data[i];
      }
    }
    return crc8;
}
/*Error in Here*/
bool nmea_parse(char* nmea_msg, uint16_t len, NmeaData_t* gps_ctx) {
    bool res = false;
    uint8_t read_crc = 0;
    LOG_DEBUG(NMEA,"Parse len: %u", len);
    if(len < NMEA_MSG_SIZE) {
        res = true;
    } else {
        LOG_ERROR(NMEA,"lenErr: %u", len);
        NmeaProto.err_cnt++;
        res = false;
    }
    if(nmea_msg && (0<len)){
        res = true;
    }else{
        LOG_ERROR(NMEA,"Line: %u", __LINE__);
        res = false;
    }
    if(res) {
        uint32_t crc_pos = 0;
        char* crc_ptr = strchr(nmea_msg, '*') + 1;
        if(crc_ptr) {
            crc_pos = (uint32_t)(crc_ptr - nmea_msg);
            if(crc_pos < NMEA_MSG_SIZE) {
                res = try_strl2uint8_hex(crc_ptr, 2, &read_crc);
            } else {
                NmeaProto.err_cnt++;
            }
        } else {
            LOG_ERROR(NMEA,"Err");
            NmeaProto.err_cnt++;
        }
    }
    if(true == res) {
        NmeaProto.crc_read_cnt++;
        res = false;
        uint8_t calc_crc = 0;
        calc_crc = nmea_calc_checksum(&nmea_msg[1], len-NMEA_OVERHEAD);
        if(read_crc == calc_crc) {
            LOG_DEBUG(NMEA,"CRC OK! [%s]",nmea_msg);
            res = false;
            NmeaProto.crc_ok_cnt++;

            if(!strncmp(nmea_msg + 3, "GGA", 3)) {
#ifdef HAS_NMEA_GGA
                gps_ctx->gga.fcnt.h_cnt++;
                LOG_DEBUG(NMEA,"GGA");
                res = gnss_parse_gga(nmea_msg, &gps_ctx->gga);
                if(res) {
                    gps_ctx->gga.fcnt.cnt++;
                    gps_ctx->gga.coordinate_dd = encode_gnss_coordinates(gps_ctx->gga.coordinate_ddmm);
                    gps_ctx->coordinate_dd = gps_ctx->gga.coordinate_dd;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "RMC", 3)) {
#ifdef HAS_NMEA_RMC
                gps_ctx->rmc.fcnt.h_cnt++;
                LOG_DEBUG(NMEA,"RMC");
                res = gnss_parse_rmc(nmea_msg, &gps_ctx->rmc);
                if(res) {
#ifdef HAS_CLOCK
                    gps_ctx->gnss_time_stamp = get_time_ms32();
#endif
                    gps_ctx->rmc.coordinate_dd = encode_gnss_coordinates(gps_ctx->rmc.coordinate_ddmm);
                    gps_ctx->coordinate_dd = gps_ctx->rmc.coordinate_dd;
                    gps_ctx->rmc.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "GLL", 3)) {
#ifdef HAS_NMEA_GLL
                gps_ctx->gll.fcnt.h_cnt++;
                LOG_DEBUG(NMEA,"GLL");
                /*here lat and lon*/
                res = gnss_parse_gll(nmea_msg, &gps_ctx->gll);
                if(res) {
#ifdef HAS_CLOCK
                    gps_ctx->gnss_time_stamp = get_time_ms32();
#endif
                    gps_ctx->gll.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "GSV", 3)) {
#ifdef HAS_NMEA_GSV
                gps_ctx->gsv.fcnt.h_cnt++;
                res = gnss_parse_gsv(nmea_msg, &gps_ctx->gsv);
                if(res) {
                    gps_ctx->gsv.fcnt.cnt++;
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "VTG", 3)) {
#ifdef HAS_NMEA_VTG
                gps_ctx->vtg.fcnt.h_cnt++;
                res = gnss_parse_vtg(nmea_msg, &gps_ctx->vtg);
                if(res) {
                    gps_ctx->vtg.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "GSA", 3)) {
#ifdef HAS_NMEA_GSA
                gps_ctx->gsa.fcnt.h_cnt++;
                res = gnss_parse_gsa(nmea_msg, &gps_ctx->gsa);
                if(res) {
                    gps_ctx->gsa.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "ZDA", 3)) {
#ifdef HAS_NMEA_ZDA
                gps_ctx->zda.fcnt.h_cnt++;
                res = gnss_parse_zda(nmea_msg, &gps_ctx->zda);
                if(res) {
                    gps_ctx->zda.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else if(!strncmp(nmea_msg + 3, "TXT", 3)) {

                gps_ctx->txt.fcnt.h_cnt++;
            } else if(!strncmp(nmea_msg + 3, "PUBX", 4)) {
#ifdef HAS_NMEA_PBUX
                gps_ctx->pbux.fcnt.h_cnt++;
                res = gnss_parse_pbux_pos(nmea_msg, &gps_ctx->pbux);
                if(res) {
                    gps_ctx->pbux.fcnt.cnt++;
                }else{
                    LOG_ERROR(NMEA,"UnableToParse[%s]",nmea_msg);
                }
#endif
            } else {
                NmeaProto.undef_err_cnt++;
                LOG_ERROR(NMEA,"UndefMess [%s]",nmea_msg+3 );
                strncpy(NmeaProto.undef_message, nmea_msg, NMEA_MSG_SIZE);
                res = false;
            }
        } else {
            LOG_ERROR(NMEA,"Mesg [%s] len %u", &nmea_msg[1],len);
            LOG_ERROR(NMEA,"CrcErr Read 0x%02x Calc 0x%02x", read_crc, calc_crc);
            NmeaProto.crc_err_cnt++;
        }
    }

    return res;
}


bool nmea_proc_byte(uint8_t rx_byte) {
    bool res = false;
    if((0 == NmeaProto.pos) && ('$' != rx_byte)) {
        NmeaProto.pos = 0;
    }
    if('$' == rx_byte){
        LOG_DEBUG(NMEA,"SpotStart");
    }
    // static uint8_t prev_rx_byte = 0;
    if(('\n' == rx_byte) || ('\r' == rx_byte)) {
        LOG_DEBUG(NMEA,"SpotEnd");
        if((0 < NmeaProto.pos) && (NmeaProto.pos < NMEA_MSG_SIZE)) {
            memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
            memcpy(NmeaProto.fix_message, NmeaProto.message, NMEA_MSG_SIZE);
            memset(NmeaProto.message, 0x00, NMEA_MSG_SIZE);
            NmeaProto.msg_cnt++;
            NmeaProto.pos = 0;
            NmeaProto.got_massege = true;
            uint16_t len = 0;
            LOG_PARN(NMEA,"SpotLine [%s]", NmeaProto.fix_message);
            if(4<strlen(NmeaProto.fix_message)){
                len = strlen(NmeaProto.fix_message) - 4;
            }

            res = nmea_parse(NmeaProto.fix_message, len, &NmeaData);
            if(true == res) {
                NmeaProto.proc_msg_cnt++;
            }

            memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
            NmeaProto.got_massege = false;
            res = true;
        }
    } else {
        if(NmeaProto.pos < NMEA_MSG_SIZE) {
            LOG_PARN(NMEA,"data[%u]=0x%02x",NmeaProto.pos, rx_byte);
            NmeaProto.message[NmeaProto.pos] = rx_byte;
            NmeaProto.pos++;
            res = true;
        } else {
            NmeaProto.pos = 0;
            NmeaProto.overflow++;
        }
    }

    //  prev_rx_byte = rx_byte;
    return res;
}

static bool nmea_calc_mode(PositionMode_t* pos_mode, NmeaData_t* nmea_data) {
    bool res = false;
    switch(nmea_data->gga.quality) {
    case QUA_NO_FIX: {
        *pos_mode = PM_NO_POSITION_FIX;
        res = true;
    } break;

    case QUA_AUTONOMOUS_GNSS_FIX:
    case QUA_DIFFERENTIAL_GNSS_FIX: {
        res = true;
        if(NAV_MODE_3D_FIX == nmea_data->gsa.navMode) {
            *pos_mode = PM_3D_GNSS_FIX;
        } else if(NAV_MODE_2D_FIX == nmea_data->gsa.navMode) {
            *pos_mode = PM_2D_GNSS_FIX;
        } else {
            res = false;
        }
    } break;
    case QUA_ESTIMATED_DEAD_RECKONING_FIX: {
        res = true;
        if('A' == nmea_data->gll.status) {
            *pos_mode = PM_DEAD_RECKONING_FIX;
        } else if('V' == nmea_data->gll.status) {
            *pos_mode = PM_DEAD_RECKONING_FIX_BUT_USER_LIMITS_EXCEEDED;
        } else {
            res = false;
        }
    } break;

    case QUA_RTK_FIXED: {
        *pos_mode = PM_RTK_FIXED;
        res = true;
    } break;
    case QUA_RTK_FLOAT: {
        *pos_mode = PM_RTK_FLOAT;
        res = true;
    } break;
    default: {
        res = false;
    } break;
    }

    if(POM_RTK_FIXED == nmea_data->rmc.pos_mode) {
        *pos_mode = PM_RTK_FIXED;
    } else if(POM_RTK_FLOAT == nmea_data->rmc.pos_mode) {
        *pos_mode = PM_RTK_FLOAT;
    }

    return res;
}

bool nmea_update_rtk_fixed_duration(void) {
    bool res = false;
    uint32_t cur_up_time = 0;
#ifdef HAS_CLOCK
    cur_up_time = get_time_ms32();
#endif
    uint32_t cur_rtk_fixed_duration = cur_up_time - NmeaProto.rtk_fixed_start_ms;
    uint16_t file_len = 0;
#ifdef HAS_FLASH_FS
    res = mm_get(PAR_ID_RTK_FIX_LONG, (uint8_t*)&NmeaProto.rtk_fixed_max_duration_ms,
                 sizeof(NmeaProto.rtk_fixed_max_duration_ms), &file_len);
#endif
    if(NmeaProto.rtk_fixed_max_duration_ms < cur_rtk_fixed_duration) {
        NmeaProto.rtk_fixed_max_duration_ms = cur_rtk_fixed_duration;
#ifdef HAS_FLASH_FS
        res = mm_set(PAR_ID_RTK_FIX_LONG, (uint8_t*)&NmeaProto.rtk_fixed_max_duration_ms,
                     sizeof(NmeaProto.rtk_fixed_max_duration_ms));
#endif
    }
    return res;
}

bool nmea_proc(void) {
    bool res = false;
    static uint32_t call_cnt = 0;
    call_cnt++;
    static uint32_t prev_zda_cnt = 0;
    static uint32_t prev_rmc_cnt = 0;
    static uint32_t prev_gga_cnt = 0;
#ifdef HAS_CLOCK
    uint32_t cur_time_ms = get_time_ms32();
    uint32_t lack_of_frame_time_out_ms = 0;
#endif
    if(prev_zda_cnt < NmeaData.zda.fcnt.cnt) {
        res = is_valid_time_date(&NmeaData.zda.time_date);
        if(res) {
            NmeaData.time_date = NmeaData.zda.time_date;
        } else {
#ifdef HAS_LOG
            LOG_ERROR(NMEA, "InvalZdaTimeDate");
#endif
        }
    }

    if(prev_rmc_cnt < NmeaData.rmc.fcnt.cnt) {
        NmeaData.coordinate_dd = encode_gnss_coordinates(NmeaData.rmc.coordinate_ddmm);
        NmeaData.time_date = NmeaData.rmc.time_date;
        res = is_valid_time_date(&NmeaData.rmc.time_date);
        if(res) {
            NmeaData.time_date = NmeaData.rmc.time_date;
        } else {
#ifdef HAS_LOG
            LOG_ERROR(NMEA, "InvalidRmcTimeDate");
#endif
        }
    }

    if(prev_gga_cnt < NmeaData.gga.fcnt.cnt) {
        NmeaData.coordinate_dd = encode_gnss_coordinates(NmeaData.gga.coordinate_ddmm);
        res = is_valid_time(&NmeaData.gga.time_date);
        if(res) {
            NmeaData.time_date.tm_hour = NmeaData.gga.time_date.tm_hour;
            NmeaData.time_date.tm_min = NmeaData.gga.time_date.tm_min;
            NmeaData.time_date.tm_sec = NmeaData.gga.time_date.tm_sec;
        } else {
#ifdef HAS_LOG
            LOG_ERROR(NMEA, "InvalidGgaTimeDate");
#endif
        }
    }
    /*If new coordinates had not been received in the last 3 seconds, then FW would have erased the old ones*/
#if defined(HAS_LOG) && defined(HAS_CLOCK)
    lack_of_frame_time_out_ms = (cur_time_ms - NmeaData.gnss_time_stamp);
    if(NMEA_LACK_FRAME_WARNING_TIME_OUT_MS < lack_of_frame_time_out_ms) {
        LOG_WARNING(NMEA, "LackOfFrame %u<%u s", NMEA_LACK_FRAME_WARNING_TIME_OUT_MS / 1000,
                    lack_of_frame_time_out_ms / 1000);
        if(NMEA_LACK_FRAME_ERROR_TIME_OUT_MS < lack_of_frame_time_out_ms) {
            LOG_ERROR(NMEA, "LackOfFrame %u<%u s", NMEA_LACK_FRAME_ERROR_TIME_OUT_MS / 1000,
                      lack_of_frame_time_out_ms / 1000);
            NmeaData.coordinate_dd.latitude = 0.0;
            NmeaData.coordinate_dd.longitude = 0.0;
        }
    }
#endif

    res = nmea_calc_mode(&NmeaProto.pos_mode, &NmeaData);
#ifdef HAS_NMEA_DIAG
    static PositionMode_t prev_pos_mode = PM_UNDEF;
    if(res) {
        if(prev_pos_mode != NmeaProto.pos_mode) {
            LOG_INFO(NMEA, "Mode %s", nmea_pos_mode2std(NmeaProto.pos_mode));
            if(PM_RTK_FIXED == prev_pos_mode) {
                nmea_update_rtk_fixed_duration();
            }
            if(PM_RTK_FIXED == NmeaProto.pos_mode) {
#ifdef LED_INDEX_GREEN
                Led[LED_INDEX_GREEN].mode = LED_MODE_ON;
#endif
#ifdef HAS_CLOCK
                NmeaProto.rtk_fixed_start_ms = get_time_ms32();
#endif
            } else {
#ifdef LED_INDEX_GREEN
                Led[LED_INDEX_GREEN].mode = LED_MODE_PWM;
                Led[LED_INDEX_GREEN].period_ms = LED_GREEN_PERIOD_MS;
#endif
            }
        } else if(prev_pos_mode == NmeaProto.pos_mode) {
            if(PM_RTK_FIXED == prev_pos_mode) {
                if(0 == (call_cnt % 5)) {
                    nmea_update_rtk_fixed_duration();
                }
            }
        }
    }
    prev_pos_mode = NmeaProto.pos_mode;
#endif

    prev_zda_cnt = NmeaData.zda.fcnt.cnt;
    prev_gga_cnt = NmeaData.gga.fcnt.cnt;
    prev_rmc_cnt = NmeaData.rmc.fcnt.cnt;

    return res;
}

bool nmea_check(void){
    bool res = false;
    uint32_t diff = NmeaProto.crc_err_cnt-NmeaProto.crc_err_cnt_prev;
    if(0<diff ){
        LOG_ERROR(NMEA, "CrcError %u times", diff);
    }
    NmeaProto.crc_err_cnt_prev=NmeaProto.crc_err_cnt;
    return res;
}
