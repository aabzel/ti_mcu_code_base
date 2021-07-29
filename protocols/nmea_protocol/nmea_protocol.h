#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include <stdbool.h>
#include <stdint.h>

/* Global Positioning System Fix Data */
typedef struct xGga_t {
    uint32_t utc; /* UTC hour in hhmmss format */
    double lat;   /* Latitude (degrees) */
    char lat_dir; /* Latitude direction */
    double lon;   /* Longitude (degrees) */
    char lon_dir; /* Longitude direction */
    uint32_t quality;
    uint32_t nb_sat;
    float hdop;
    float height;
    char height_unit;
    float geoid_separation;
    char geoid_unit;
    float diff_gps_age;
    uint32_t station_ID;
} gga_t;

/* Geographic Position (latitude/longitude) and time */
typedef struct xgll_t {
    double lat;      /* Latitude (degrees) */
    char lat_dir;    /* Latitude direction */
    double lon;      /* Longitude (degrees) */
    char lon_dir;    /* Longitude direction */
    uint32_t utc;    /* UTC hour in hhmmss format */
    char data_valid; /* validity - A=OK */
} gll_t;

/* GNSS satellites in view */
typedef struct xgsv_t {
    uint32_t nb_gsv_msgs_in_cycle; /* Total number of GSV msgs in this cycle */
    uint32_t msg_number;
    uint32_t nb_sv_in_view;
    struct sv_infos_t {
        uint32_t prn;       /* SV PRN (identification number) */
        uint32_t elevation; /* Elevation in degrees, 90 maximum */
        uint32_t azimuth;   /* Azimuth, degrees from true north, 000 to 359 */
        uint32_t snr;       /* Signal/Noise 00-99 dB (null when no tracking) */
    } sv_info[4];
    struct sv_infos_t second_sv;
    struct sv_infos_t third_sv;
    struct sv_infos_t fourth_sv;
} gsv_t;

/* Recommended minimum specific GPS/Transit data */
typedef struct xRmc_t {
    uint32_t utc;       /* UTC hour in hhmmss format */
    char data_valid;    /* validity - A-ok, V-invalid */
    double lat;         /* Latitude (degrees) */
    char lat_dir;       /* Latitude direction */
    double lon;         /* Longitude (degrees) */
    char lon_dir;       /* Longitude direction */
    double speed_knots; /* Speed over ground (knots) */
    double true_course; /* Course Made Good, True */
    uint32_t date;      /* date in ddmmaa format */
    double mv;   /* Magnetic variation */
    char mv_ew; /* Magnetic variation direction */
    char pos_mode;      /* A = autonom, D=DGPS, E=DR */
    char nav_status;     /* */
    uint8_t checksum;
} rmc_t;

/* GNSS DOP and active satellites */
typedef struct xgsa_t {
    char selection;     /* Selection of 2D/3D fix (A=auto M=manual) */
    uint32_t fix_value; /* 1=No fix 2=2D fix 3=3D fix*/
    uint32_t prns[12];  /* PRN numbers */
    float pdop;         /* PDOP: 0.5 through 99.9 */
    float hdop;         /* HDOP: 0.5 through 99.9 */
    float vdop;         /* VDOP: 0.5 through 99.9 */
} gsa_t;

/* Track Made Good and Ground Speed */
typedef struct xvtg_t {
    double true_track;     /* track made good (degrees true) */
    char true_indic;       /* T: track is relative to true north */
    double track_magnetic; /* Track made good (degrees magnetic) */
    char magnetic_indic;   /* M: track is relative to magnetic north */
    double speed_knots;    /* Ground speed, knots */
    char knots_indic;      /* N: speed is measured in knots */
    double speed_kph;      /* Ground speed, Kilometers per hour */
    char kph_indic;        /* K: speed over ground is measured in kph */
    char data_valid;       /* validity - A=OK */
} vtg_t;

/* GNSS context. Used to keep last GNSS infos from GNSS module msgs*/
typedef struct xGps_ctx_t {
    uint8_t is_initialized;
    rmc_t rmc;
    gga_t gga;
    gll_t gll;
    gsv_t gsv;
    gsa_t gsa;
    vtg_t vtg;
} gps_ctx_t;

bool gnss_parse_rmc(char* nmea_msg, rmc_t* rmc);
bool nmea_gnss_parse(char* nmea_msg, gps_ctx_t* gps_ctx);

#endif /* NMEA_PARSER_H */
