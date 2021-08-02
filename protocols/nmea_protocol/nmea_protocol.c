#include "nmea_protocol.h"

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
    // 140213.00,5540.70555,N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2uint32(ptr, 6, &gga->utc) && res;

    ptr = strchr(ptr, ',') + 1;
    // 5540.70555,N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 10, &gga->lat) && res;

    ptr = strchr(ptr, ',') + 1;
    // N,03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    gga->lat_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    // 03737.93436,E,1,12,0.58,201.4,M,13.3,M,,*42
    res = try_strl2double(ptr, 11, &gga->lon) && res;

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

//Latitude and longitude, with time of position fix and status
//$GNGLL,5540.70588,N,03737.93396,E,140121.00,A,A*70
bool gnss_parse_gll(char* nmea_msg, gll_t* gll) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;

    //5540.70588,N,03737.93396,E,140121.00,A,A*70
    res = try_strl2double(ptr, 10, &gll->lat) && res;

    ptr = strchr(ptr, ',') + 1;
    //N,03737.93396,E,140121.00,A,A*70
    gll->lat_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    //03737.93396,E,140121.00,A,A*70
    res = try_strl2double(ptr, 11, &gll->lon) && res;

    ptr = strchr(ptr, ',') + 1;
    //E,140121.00,A,A*70
    gll->lon_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    //140121.00,A,A*70
    res = try_strl2uint32(ptr, 6, &gll->time) && res;

    ptr = strchr(ptr, ',') + 1;
    //A,A*70
    gll->status = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    //A*70
    gll->pos_mode= ptr[0];
    return res;
}

/*
$GNRMC,072316.27,A,5551.85875,N,03755.65965,E,0.010,,290721,11.73,E,A,V*76
*/
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc) {
    bool res = true;
    char* ptr = strchr(nmea_msg, ',') + 1;
    res = try_strl2uint32(ptr, 6, &rmc->utc) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->data_valid = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 10, &rmc->lat) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->lat_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 11, &rmc->lon) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->lon_dir = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 5, &rmc->speed_knots) && res;

    ptr = strchr(ptr, ',') + 1;
    // res = try_str2double(ptr, &rmc->true_course)&&res;

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2uint32(ptr, 6, &rmc->date) && res;

    ptr = strchr(ptr, ',') + 1;
    res = try_strl2double(ptr, 5, &rmc->mv) && res;

    ptr = strchr(ptr, ',') + 1;
    rmc->mv_ew = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    rmc->pos_mode = ptr[0];

    ptr = strchr(ptr, ',') + 1;
    rmc->nav_status = ptr[0];
    res = parse_time_from_val(rmc->utc, &rmc->time_date) && res;
    res = parse_date_from_val(rmc->date, &rmc->time_date) && res;

    return res;
}

uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len) {
    uint8_t crc = 0;
    uint16_t i;
    for(i = 0; i < len; i++) {
        crc ^= nmea_data[i];
    }
    return crc;
}

bool nmea_parse(char* nmea_msg, NmeaData_t* gps_ctx) {
    bool res = false;
    uint8_t read_crc;
    uint16_t len = strlen(nmea_msg) - 4;
    char* ptr = strchr(nmea_msg, '*') + 1;
    res = try_strl2uint8_hex(ptr, 2, &read_crc);
    if(true == res) {
        res = false;
        uint8_t calc_crc;
        calc_crc = nmea_calc_checksum(&nmea_msg[1], len);
        if(read_crc == calc_crc) {
            res = false;
            NmeaProto.crc_ok_cnt++;
            if(!strncmp(nmea_msg + 3, "GGA", 3)) {
                res = gnss_parse_gga(nmea_msg, &gps_ctx->gga);
            } else if(!strncmp(nmea_msg + 3, "RMC", 3)) {
                res = gnss_parse_rmc(nmea_msg, &gps_ctx->rmc);
            } else if(!strncmp(nmea_msg + 3, "GLL", 3)) {
                res = gnss_parse_gll(nmea_msg, &gps_ctx->gll);
            } else if(!strncmp(nmea_msg + 3, "GSV", 3)) {
            } else if(!strncmp(nmea_msg + 3, "VTG", 3)) {
            } else if(!strncmp(nmea_msg + 3, "GSA", 3)) {
            } else if(!strncmp(nmea_msg + 3, "TXT", 3)) {
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
        if(0 < NmeaProto.pos) {
            // memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
            memcpy(NmeaProto.fix_message, NmeaProto.message, NmeaProto.pos);
            memset(NmeaProto.message, 0x00, NMEA_MSG_SIZE);
            NmeaProto.msg_cnt++;
            NmeaProto.pos = 0;
            NmeaProto.got_massege = true;
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

bool nmea_proc_message(void) {
    bool res = false;
    if(NmeaProto.got_massege) {
        res = nmea_parse(NmeaProto.fix_message, &NmeaData);
        if(true == res) {
            NmeaProto.proc_msg_cnt++;
        }
        memset(NmeaProto.fix_message, 0x00, NMEA_MSG_SIZE);
        NmeaProto.got_massege = false;
    }
    return res;
}
