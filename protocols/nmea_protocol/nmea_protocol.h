#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define NMEA_MSG_SIZE 80U
typedef struct xNmeaProtocol_t {
    uint16_t pos;
    uint32_t msg_cnt;
    uint32_t crc_err_cnt;
    uint32_t crc_ok_cnt;
    uint32_t overflow;
    uint32_t proc_msg_cnt;
    bool got_massege;
    char message[NMEA_MSG_SIZE];
    char fix_message[NMEA_MSG_SIZE];
} NmeaProtocol_t;

/* Recommended minimum specific GPS/Transit data */
typedef struct xRmc_t {
    uint32_t utc; /* UTC hour in hhmmss format */
    struct tm time_date;
    char data_valid;    /* validity - A-ok, V-invalid */
    double lat;         /* Latitude (degrees) */
    char lat_dir;       /* Latitude direction */
    double lon;         /* Longitude (degrees) */
    char lon_dir;       /* Longitude direction */
    double speed_knots; /* Speed over ground (knots) */
    double true_course; /* Course Made Good, True */
    uint32_t date;      /* date in ddmmaa format */
    double mv;          /* Magnetic variation */
    char mv_ew;         /* Magnetic variation direction */
    char pos_mode;      /* A = autonom, D=DGPS, E=DR */
    char nav_status;    /* */
} rmc_t;

/* Global Positioning System Fix Data */
typedef struct xGga_t {
    uint32_t utc;            /* UTC hour in hhmmss format */
    double lat;              /* Latitude (degrees) */
    char lat_dir;            /* Latitude direction */
    double lon;              /* Longitude (degrees) */
    char lon_dir;            /* Longitude direction */
    uint16_t quality;        /*Quality indicator for position fix*/
    uint16_t nb_sat;         /*Number of satellites used*/
    double hdop;             /*Horizontal Dilution of Precision*/
    double height;           /*Altitude above mean sea level*/
    char height_unit;        /*Altitude units: M (meters, fixed field*/
    double geoid_separation; /*difference between ellipsoid and mean sea level*/
    char geoid_unit;         /*Geoid separation units: M (meters, fixed field)*/
    double diff_gps_age;     /*Age of differential corrections (null when DGPS is notused)*/
    uint32_t diffStation;    /*ID of station providing differential corrections*/
} gga_t;

/* GNSS context. Used to keep last GNSS infos from GNSS module msgs*/
typedef struct xNmeaData_t {
    uint8_t is_initialized;
    rmc_t rmc;
    gga_t gga;
} NmeaData_t;

extern NmeaProtocol_t NmeaProto;
extern NmeaData_t NmeaData;

bool nmea_init(void);
bool nmea_proc_byte(uint8_t rx_byte);
uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len);
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc);
bool gnss_parse_gga(char* nmea_msg, gga_t* gga);
bool nmea_parse(char* nmea_msg, NmeaData_t* gps_ctx);
bool nmea_proc_message(void);

#endif /* NMEA_PARSER_H */
