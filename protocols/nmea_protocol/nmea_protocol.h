#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef HAS_MCU
#include "clocks.h"
#endif

#include "gnss_utils.h"

#define NUM_OF_PARSED_SAT 5
#define NMEA_MSG_SIZE 100U
#define NMEA_PERIOD_US S_2_US(2)

typedef struct xNmeaProtocol_t {
    uint16_t pos;
    uint32_t msg_cnt;
    uint32_t crc_err_cnt;
    uint32_t err_cnt;
    uint32_t crc_read_cnt;
    uint32_t crc_ok_cnt;
    uint32_t overflow;
    uint32_t proc_msg_cnt;
    bool got_massege;
    char message[NMEA_MSG_SIZE];
    char fix_message[NMEA_MSG_SIZE];
} NmeaProtocol_t;

/* Recommended minimum specific GPS/Transit data */
typedef struct xRmc_t {
    uint32_t cnt;
    struct tm time_date;
    char data_valid; /* validity - A-ok, V-invalid */
    GnssCoordinate_t coordinate;
    char lat_dir;       /* Latitude direction */
    char lon_dir;       /* Longitude direction */
    double speed_knots; /* Speed over ground (knots) */
    double true_course; /* Course Made Good, True */
    double mv;          /* Magnetic variation */
    char mv_ew;         /* Magnetic variation direction */
    char pos_mode;      /* A = autonom, D=DGPS, E=DR */
    char nav_status;    /* */
} rmc_t;

/* Global Positioning System Fix Data */
typedef struct xGga_t {
    uint32_t cnt;
    uint32_t utc; /* UTC hour in hhmmss format */
    struct tm time_date;
    GnssCoordinate_t coordinate;
    char lat_dir;            /* Latitude direction */
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

/* Latitude and longitude, with time of position fix and status */
typedef struct xGll_t {
    struct tm time_date;
    char lat_dir; /* North/South indicator */
    GnssCoordinate_t coordinate;
    char lon_dir;  /* Longitude direction East/West indicator */
    char status;   /*Data validity status*/
    char pos_mode; /*Positioning mode, see position fix flags description*/
    uint32_t cnt;
} gll_t;

typedef struct xGsa_t {
    char opMode;      /* Operation mode:*/
    uint8_t navMode;  /* Navigation mode  */
    double PDOP;      /* Position dilution of precision */
    double HDOP;      /* Horizontal dilution of precision */
    double VDOP;      /* Vertical dilution of precision */
    uint8_t systemId; /* NMEA-defined GNSS system ID*/
    uint32_t cnt;
} gsa_t;

/*Course over ground and ground speed*/
typedef struct xVtg_t {
    double cogt;   /*Course over ground (true)*/
    double cogm;   /*Course over ground (magnetic)*/
    double sogn;   /*Speed over ground*/
    double sogk;   /*Speed over ground*/
    char cogtUnit; /*Course over ground units: T (degrees true, fixed field)*/
    char cogmUnit; /*Course over ground units: M (degrees magnetic, fixed field)*/
    char sognUnit; /*Speed over ground units: N (knots, fixed field)*/
    char sogkUnit; /*Speed over ground units: K*/
    char posMode;  /*Mode indicator*/
    uint32_t cnt;
} vtg_t;

typedef struct xPbux_t {
    uint8_t msg_id;
    struct tm time_date;
    uint32_t cnt;
} pbux_t;

typedef struct xSatellite_t {
    uint16_t svid; /*Satellite ID*/
    uint8_t elv;   /*Elevation (<= 90)*/
    uint16_t az;   /*Azimuth (range: 0-359)*/
    uint8_t cno;   /*Signal strength (C/N0, range: 0-99), null when not tracking*/
} Satellite_t;

/*GNSS satellites in view*/
typedef struct xGsv_t {
    uint8_t numMsg;   /*Number of messages, total number of GSV messages being output (range: 1-9)*/
    uint8_t msgNum;   /*Number of this message*/
    uint8_t signalId; /*NMEA-defined GNSS signal ID*/
    Satellite_t sat[NUM_OF_PARSED_SAT];
    uint16_t numSV; /*Number of known satellites in view regarding both the talker ID and the signalId*/
    uint32_t cnt;
} gsv_t;

/* GNSS context. Used to keep last GNSS infos from GNSS module msgs*/
typedef struct xNmeaData_t {
    uint8_t is_initialized;
    rmc_t rmc;   /*Recommended minimum data*/
    gll_t gll;   /*Latitude and longitude, with time of position fix and status*/
    vtg_t vtg;   /*Course over ground and ground speed*/
    gga_t gga;   /*Global positioning system fix data*/
    gsa_t gsa;   /*GNSS DOP and active satellites*/
    gsv_t gsv;   /*GNSS satellites in view*/
    pbux_t pbux; /*Proprietary NMEA messages for u-blox positioning receivers*/
    // rlm_t rlm; /*Return link message (RLM) */
    // ths_t ths; /*True heading and status*/
    // txt_t txt; /*Text transmission*/
    // zda_t zda Time and date
    // dtm_t dtm Datum reference
    // grs_t grs GNSS range residuals
    // gst_t GNSS pseudorange error statistics
    // gqq_t Poll a standard message (Talker ID GQ)
    // gns_t GNSS fix data
    // glq_t Poll a standard message
    // gaq_t Poll a standard message (Talker ID GA)
    // gpq_t Poll a standard message (Talker ID GP)
    // gbq_t Poll a standard message (Talker ID GB)
    // gbs_t GNSS satellite fault detection
} NmeaData_t;

extern NmeaProtocol_t NmeaProto;
extern NmeaData_t NmeaData;

bool nmea_init(void);
bool nmea_proc_byte(uint8_t rx_byte);
uint8_t nmea_calc_checksum(char* nmea_data, uint16_t len);
bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc);
bool gnss_parse_vtg(char* nmea_msg, vtg_t* vtg);
bool gnss_parse_gga(char* nmea_msg, gga_t* gga);
bool gnss_parse_gsa(char* nmea_msg, gsa_t* gsa);
bool nmea_parse(char* nmea_msg, NmeaData_t* gps_ctx);
bool gnss_parse_pbux_pos(char* nmea_msg, pbux_t* const pbux);
bool gnss_parse_gll(char* nmea_msg, gll_t* gll);
bool nmea_proc(void);
#endif /* NMEA_PARSER_H */
