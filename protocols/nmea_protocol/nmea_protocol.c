#include "nmea_protocol.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "convert.h"
#include "time_utils.h"

NmeaProtocol_t NmeaProto;
NmeaData_t NmeaData;

bool nmea_init(void) {
    memset(&NmeaData, 0x00, sizeof(NmeaData));
    memset(&NmeaProto, 0x00, sizeof(NmeaProto));
    return true;
}

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

//$GLGSV,3,3,10, 85,46,314,38,
//               86,04,339,,3*7A
bool gnss_parse_gsv(char* nmea_msg, gsv_t* gsv) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    res = try_strl2uint8(ptr, 1, &gsv->numMsg) && res;

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2uint8(ptr, 1, &gsv->msgNum) && res;

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2uint16(ptr, 2, &gsv->numSV) && res;
    uint8_t i = 0;
    for(i = 0; i < (gsv->msgNum - 1); i++) {
        if(i < NUM_OF_PARSED_SAT) {
            ptr = strchr(ptr, ',') + 1;
            res = try_strl2uint16(ptr, 2, &gsv->sat[i].svid) && res;

            ptr = strchr(ptr, ',') + 1;
            res = try_strl2uint8(ptr, 2, &gsv->sat[i].elv) && res;

            ptr = strchr(ptr, ',') + 1;
            res = try_strl2uint16(ptr, 2, &gsv->sat[i].az) && res;

            ptr = strchr(ptr, ',') + 1;
            res = try_strl2uint8(ptr, 2, &gsv->sat[i].cno) && res;
        }
    }
    ptr = strchr(ptr, ',') + 1;
    res = try_strl2uint8(ptr, 1, &gsv->signalId) && res;

    return res;
}

//$GNVTG,,T,,M,0.019,N,0.036,K,A*30
bool gnss_parse_vtg(char* nmea_msg, vtg_t* vtg) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    //,T,,M,0.019,N,0.036,K,A*30  //Course over ground

    ptr = strchr(ptr, ',') + 1;
    // T,,M,0.019,N,0.036,K,A*30  Course over ground units
    vtg->cogtUnit = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    //,M,0.019,N,0.036,K,A*30  Course over ground

    ptr = strchr(ptr, ',') + 1;
    // M,0.019,N,0.036,K,A*30   Course over ground units
    vtg->cogmUnit = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 0.019,N,0.036,K,A*30  Speed over ground
    res = try_strl2double(ptr, 5, &vtg->sogn) && res;

    ptr = strchr(ptr, ',') + 1;
    // N,0.036,K,A*30  Speed over ground  Speed over ground units
    vtg->sognUnit = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 0.036,K,A*30  Speed over ground
    res = try_strl2double(ptr, 5, &vtg->sogk) && res;

    ptr = strchr(ptr, ',') + 1;
    // K,A*30     Speed over ground units
    vtg->sogkUnit = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // A*30
    vtg->posMode = ptr[0];
    return res;
}

/* GNSS DOP and active satellites */
//$GNGSA,A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B
bool gnss_parse_gsa(char* nmea_msg, gsa_t* gsa) {
    bool res = true;

    char* ptr = strchr(nmea_msg, ',') + 1;
    // A,3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B
    gsa->opMode = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 3,78,85,68,84,69,,,,,,,,1.04,0.58,0.86,2*0B
    res = try_strl2uint8(ptr, 1, &gsa->navMode) && res;

    uint8_t i = 0;
    for(i = 0; i < 12; i++) {
        ptr = strchr(ptr, ',') + 1;
    }

    ptr = strchr(ptr, ',') + 1;
    // 1.04,0.58,0.86,2*0B
    res = try_strl2double(ptr, 4, &gsa->PDOP) && res;

    ptr = strchr(ptr, ',') + 1;
    // 0.58,0.86,2*0B
    res = try_strl2double(ptr, 4, &gsa->HDOP) && res;

    ptr = strchr(ptr, ',') + 1;
    // 0.86,2*0B
    res = try_strl2double(ptr, 4, &gsa->VDOP) && res;

    ptr = strchr(ptr, ',') + 1;
    // 2*0B
    res = try_strl2uint8(ptr, 1, &gsa->systemId) && res;
    return res;
}

// Latitude and longitude, with time of position fix and status
//$GNGLL,5540.70588,N,03737.93396,E,140121.00,A,A*70
bool gnss_parse_gll(char* nmea_msg, gll_t* gll) {
    bool res = true;
    uint32_t cur_pos = 0;
    if(nmea_msg && gll) {
        char* ptr = strchr(nmea_msg, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if(cur_pos < NMEA_MSG_SIZE) {
            // 5540.70588,N,03737.93396,E,140121.00,A,A*70
            res = try_strl2double(ptr, 10, &gll->coordinate_ddmm.latitude) && res;
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // N,03737.93396,E,140121.00,A,A*70
            gll->lat_dir = ptr[0];
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // 03737.93396,E,140121.00,A,A*70
            res = try_strl2double(ptr, 11, &gll->coordinate_ddmm.longitude) && res;
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // E,140121.00,A,A*70
            gll->lon_dir = ptr[0];
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // 140121.00,A,A*70
            uint32_t utc_time = 0;
            res = try_strl2uint32(ptr, 6, &utc_time) && res;
            if(res) {
                res = parse_time_from_val(utc_time, &gll->time_date);
            }
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // A,A*70
            gll->status = ptr[0];
        }

        ptr = strchr(ptr, ',') + 1;
        cur_pos = (uint32_t)(ptr - nmea_msg);
        if((cur_pos < NMEA_MSG_SIZE) && (ptr)) {
            // A*70
            gll->pos_mode = ptr[0];
        }
    }
    return res;
}

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

/*
$GNRMC,072316.27,A,5551.85875,N,03755.65965,E,0.010,,290721,11.73,E,A,V*76
*/
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    uint32_t utc_time = 0;
    res = try_strl2uint32(ptr, 6, &utc_time) && res;
    if(res) {
        res = parse_time_from_val(utc_time, &rmc->time_date);
    }

    ptr = strchr(ptr, ',') + 1;
    rmc->data_valid = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 10, &rmc->coordinate_ddmm.latitude) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->lat_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 11, &rmc->coordinate_ddmm.longitude) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->lon_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 5, &rmc->speed_knots) && res;

    ptr = strchr(ptr, ',') + 1;
    // res = try_str2double(ptr, &rmc->true_course)&&res;

    ptr = strchr(ptr, ',') + 1;
    uint32_t date = 0;
    res = try_strl2uint32(ptr, 6, &date) && res;
    if(res) {
        res = parse_date_from_val(date, &rmc->time_date);
    }

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 5, &rmc->mv) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->mv_ew = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    rmc->pos_mode = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    rmc->nav_status = ptr[0];

    return res;
}

uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len) {
    uint8_t crc8 = 0;
    uint16_t i = 0;
    for(i = 0; i < len; i++) {
        crc8 ^= nmea_data[i];
    }
    return crc8;
}

bool nmea_parse(char* nmea_msg, NmeaData_t* gps_ctx) {
    bool res = false;
    uint8_t read_crc = 0;
    uint16_t len = strlen(nmea_msg) - 4;
#ifdef X86_64
    printf("\nlen: %u", len);
#endif
    if(len < NMEA_MSG_SIZE) {
        res = true;
    } else {
        NmeaProto.err_cnt++;
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
            NmeaProto.err_cnt++;
        }
    }
    if(true == res) {
        NmeaProto.crc_read_cnt++;
        res = false;
        uint8_t calc_crc = 0;
        calc_crc = nmea_calc_checksum(&nmea_msg[1], len);
        if(read_crc == calc_crc) {
#ifdef X86_64
            printf("\nCRC OK!");
#endif
            res = false;
            NmeaProto.crc_ok_cnt++;
            if(!strncmp(nmea_msg + 3, "GGA", 3)) {
#ifdef X86_64
                printf("\nGGA");
#endif
                res = gnss_parse_gga(nmea_msg, &gps_ctx->gga);
                if(res) {
                    gps_ctx->gga.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "RMC", 3)) {
#ifdef X86_64
                printf("\n RMC");
#endif
                res = gnss_parse_rmc(nmea_msg, &gps_ctx->rmc);
                if(res) {
                    gps_ctx->rmc.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "GLL", 3)) {
#ifdef X86_64
                printf("\n GLL");
#endif
                /*here lat and lon*/
                res = gnss_parse_gll(nmea_msg, &gps_ctx->gll);
                if(res) {
                    gps_ctx->gll.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "GSV", 3)) {
                res = gnss_parse_gsv(nmea_msg, &gps_ctx->gsv);
                if(res) {
                    gps_ctx->gsv.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "VTG", 3)) {
                res = gnss_parse_vtg(nmea_msg, &gps_ctx->vtg);
                if(res) {
                    gps_ctx->vtg.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "GSA", 3)) {
                res = gnss_parse_gsa(nmea_msg, &gps_ctx->gsa);
                if(res) {
                    gps_ctx->gsa.cnt++;
                }
            } else if(!strncmp(nmea_msg + 3, "TXT", 3)) {
            } else if(!strncmp(nmea_msg + 3, "PUBX", 4)) {
                res = gnss_parse_pbux_pos(nmea_msg, &gps_ctx->pbux);
                if(res) {
                    gps_ctx->pbux.cnt++;
                }
            } else {
                res = false;
            }
        } else {
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
    // static uint8_t prev_rx_byte = 0;
    if(('\n' == rx_byte) || ('\r' == rx_byte)) {
        if((0 < NmeaProto.pos) && (NmeaProto.pos < NMEA_MSG_SIZE)) {
            memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
            memcpy(NmeaProto.fix_message, NmeaProto.message, NmeaProto.pos);
            memset(NmeaProto.message, 0x00, NMEA_MSG_SIZE);
            NmeaProto.msg_cnt++;
            NmeaProto.pos = 0;
            NmeaProto.got_massege = true;
            res = nmea_parse(NmeaProto.fix_message, &NmeaData);
            if(true == res) {
                NmeaProto.proc_msg_cnt++;
            }
            memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
            NmeaProto.got_massege = false;
            res = true;
        }
    } else {
        if(NmeaProto.pos < NMEA_MSG_SIZE) {
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

bool nmea_proc(void) {
    bool res = false;
    static uint32_t prev_rmc_cnt = 0;
    static uint32_t prev_gga_cnt = 0;

    if(prev_rmc_cnt < NmeaData.rmc.cnt) {
        NmeaData.coordinate_dd = encode_gnss_coordinates(NmeaData.rmc.coordinate_ddmm);
        res = true;
    }

    if(prev_gga_cnt < NmeaData.gga.cnt) {
        NmeaData.coordinate_dd = encode_gnss_coordinates(NmeaData.gga.coordinate_ddmm);
        res = true;
    }

    prev_gga_cnt = NmeaData.gga.cnt;
    prev_rmc_cnt = NmeaData.rmc.cnt;

    return res;
}
