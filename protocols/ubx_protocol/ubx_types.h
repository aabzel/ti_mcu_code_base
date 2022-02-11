#ifndef UBX_TYPES_H
#define UBX_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/*UBX-NAV-POSLLH (0x01 0x02)*/
typedef struct xNavPosllh_t{
    uint32_t iTOW;          /*0  ms*/
    int32_t lon;           /*4  1e-7 deg*/
    int32_t lat;           /*8  1e-7 deg*/
    int32_t height;        /*12 mm*/
    int32_t hMSL;          /*16 mm*/
    uint32_t hAcc;          /*20 mm*/
    uint32_t vAcc;          /*24 mm*/
}NavPosllh_t;

/*UBX-NAV-HPPOSLLH (0x01 0x14)*/
typedef struct xNavHpPosllh_t{
    uint8_t version; /*0 Message version*/
    uint16_t reserved0; /*1*/
    uint32_t iTOW; /*4 GPS time of week of the navigation epoch.*/
    int32_t lon; /*8 Longitude*/
    int32_t  lat; /*12 Latitude*/
    int32_t  height; /*16 Height above ellipsoid*/
    int32_t  hMSL; /*20 Height above mean sea level*/
    int8_t   lonHp; /*24*/
    int8_t latHp; /*25*/
    int8_t  heightHp; /*26*/
    int8_t  hMSLHp; /*17*/
    uint32_t  hAcc; /*28*/
    uint32_t vAcc; /*32*/
}NavHpPosllh_t;


//UBX-SEC-UNIQID (0x27 0x03)
typedef struct xSecUniqId_t{
    uint8_t version;         /*0 Message version (0x01 for this version)*/
    uint8_t reserved[3];     /*1*/
    uint8_t UniqueChipId[5]; /*4 Unique chip ID*/
}SecUniqId_t;

typedef union uTimeUtcValidityFlags_t{
    uint8_t byte;
    struct{
        uint8_t valid_tow :1;       /*Valid Time of Week*/
        uint8_t valid_wkn:1;        /*Valid Week Number*/
        uint8_t valid_utc:1;        /*Valid UTC Time*/
        uint8_t utc_standard:4;     /*UTC standard identifier*/
    };
}TimeUtcValidityFlags_t;

//UBX-NAV-TIMEUTC (0x01 0x21)
typedef struct xNavTimeUtc_t{
    uint32_t  iTOW;               /*0 GPS time of week of the navigation epoch.*/
    uint32_t tAcc;                /*4 Time accuracy estimate (UTC)*/
    int32_t nano;                 /*8 Fraction of second, range -1e9 .. 1e9 (UTC)*/
    uint16_t year;                /*12 Year, range 1999..2099 (UTC)*/
    uint8_t month;                /*14 Month, range 1..12 (UTC)*/
    uint8_t day;                  /*15 Day of month, range 1..31 (UTC)*/
    uint8_t hour;                 /*16 Hour of day, range 0..23 (UTC)*/
    uint8_t min;                  /*17 Minute of hour, range 0..59 (UTC)*/
    uint8_t sec;                  /*18 Seconds of minute, range 0..60 (UTC)*/
    TimeUtcValidityFlags_t ValidityFlags;/*19 Validity Flags*/
}NavTimeUtc_t;


//UBX-NAV-VELNED 0x01 0x12
typedef struct xNavVelNed_t{
    uint32_t itow;        /*0       ms   GPS time of week of the navigation epoch*/
    int32_t vel_north; /*4       cm/s North velocity component*/
    int32_t vel_east;  /*8       cm/s East velocity component*/
    int32_t  vel_down;  /*12      cm/s Down velocity component*/
    uint32_t speed;      /*16      cm/s Speed (3-D)*/
    uint32_t speed_ground;/*20      cm/s Ground speed (2-D)*/
    int32_t heading;      /*24 1e-5 deg  Heading of motion 2-D*/
    uint32_t  speed_acc; /*28      cm/s Speed accuracy Estimate*/
    uint32_t course_acc; /*32 1e-5 deg  Course / Heading accuracy estimate*/
}NavVelNed_t;

// UBX-NAV-PVT (0x01 0x07)
typedef struct xNavPvt_t{
    uint32_t iTOW;           /* GPS time of week of the navigation epoch.          */
    uint16_t year;           /* Year          */
    uint8_t month;           /* Month, range 1..12          */
    uint8_t day;             /* Day of month, range 1..31          */
    uint8_t hour;            /* Hour of day, range 0..23          */
    uint8_t  min;            /* Minute of hour, range 0..59          */
    uint8_t  sec;            /* Seconds of minute, range 0..60          */
    uint8_t  ValidityFlags;  /*           */
    uint32_t tAcc;           /* Time accuracy estimate          */
    int32_t nano;            /*           */
    uint8_t fixType;         /*20 GNSSfix Type          */
    uint8_t FixStatusFlags;  /*           */
    uint8_t AdditionalFlags; /*           */
    uint8_t numSV;           /*           */
    int32_t lon;             /*           */
    int32_t lat;             /*           */
    int32_t height;          /*           */
    int32_t hMSL;            /*           */
    uint32_t hAcc;           /*           */
    uint32_t vAcc;           /*           */
    int32_t velN;            /*           */
    int32_t velE;            /*           */
    int32_t velD;            /*           */
    int32_t gSpeed;          /*           */
    int32_t headMot;         /*           */
    uint32_t sAcc;           /*           */
    uint32_t headAcc;        /*           */
    uint32_t pDOP;           /*           */
    uint8_t flags3;          /*           */
    uint8_t reserved[5];     /*           */
    int32_t headVeh;         /*           */
    int16_t magDec;          /*           */
    uint16_t magAcc;         /*           */
}NavPvt_t;
 


typedef struct xLayers_t {
    union {
        uint8_t byte;
        struct {
            uint8_t ram : 1;   /* Update configuration in the RAM layer*/
            uint8_t bbr : 1;   /* Update configuration in the BBR layer Battery-backed RAM*/
            uint8_t flash : 1; /* Update configuration in the Flash layer*/
            uint8_t rsrv : 5;
        };
    };
} Layers_t;

/*3.10.21 UBX-CFG-TMODE3 (0x06 0x71)*/
typedef struct xUbxCfgTmode3Data_t {
    uint8_t version;      /*0  Message version (0x00 for this version)*/
    uint8_t reserved0;    /*1  Reserved*/
    uint8_t mode;         /*2  Receiver Mode:*/
    uint8_t lla;          /*3  Position is given in LAT/LON/ALT (default is ECEF)*/
    int32_t ecefXOrLat;   /*4  WGS84 ECEF X coordinate (or latitude) of the ARP position, depending on flags above*/
    int32_t ecefYOrLon;   /*8  WGS84 ECEF Y coordinate (or longitude) of the ARP position, depending on flags above*/
    int32_t ecefZOrAlt;   /*12 WGS84 ECEF Z coordinate (or altitude) of the ARP position, depending on flags above*/
    int8_t ecefXOrLatHP;  /*16 High-precision WGS84 ECEF X coordinate (or latitude) of the ARP position, depending on
                             flags above.*/
    int8_t ecefYOrLonHP;  /*17 High-precision WGS84 ECEF Y coordinate (or longitude) of the ARP position, depending on
                             flags above.*/
    int8_t ecefZOrAltHP;  /*18 High-precision WGS84 ECEF Z coordinate (or altitude) of the ARP position, depending on
                             flags above. Must be in the range -99..+99.*/
    uint8_t reserved1;    /*19 Fixed position 3D accuracy*/
    uint32_t fixedPosAcc; /*20 Fixed position 3D accuracy*/
    uint32_t svin_min_dur_s; /*24 Survey-in minimum duration in seconds*/
    uint32_t svinAccLimit;   /*28 Survey-in position accuracy limit*/
    uint8_t reserved2[8];    /*32 Fixed position 3D accuracy*/
} UbxCfgTmode3Data_t;

/*3.10.2 UBX-CFG-CFG (0x06 0x09)*/
typedef struct xUbxCfgCfg_t {
    uint32_t clearMask; /*Mask for configuration to clear*/
    uint32_t saveMask;  /*Mask for configuration to save*/
    uint32_t loadMask;  /*Mask for configuration to load*/
    uint8_t deviceMask; /*deviceMask*/
} __attribute__((__packed__)) UbxCfgCfg_t;

/*3.10.17 UBX-CFG-RATE (0x06 0x08)*/
typedef struct xUbxCfgRate_t {
    uint16_t meas_rate_ms; /*0 ms The elapsed time between GNSS measurements*/
    uint16_t navRate; /*2 cycles The ratio between the number of measurements and the number of navigation solutions*/
    uint16_t timeRef; /*4 */
} __attribute__((__packed__)) UbxCfgRate_t;

typedef struct xConfigurationKeyID_t {
    union {
        uint32_t word;
        struct {
            uint16_t
                id_within_group : 12; /* Twelve bits that define a unique item ID within a group (range 0x001-0xffe)*/
            uint16_t rsvd1 : 4;       /*Currently unused. Reserved for future use.*/
            uint16_t group : 8;       /*Eight bits that define a unique group ID (range 0x01-0xfe)*/
            uint16_t rsvd2 : 4;       /*Currently unused. Reserved for future use.*/
            uint16_t size : 3;  /*Three bits that indicate the storage size of a Configuration Value (range 0x01-0x05,*/
            uint16_t rsvd3 : 1; /*Currently unused. Reserved for future use.*/
        };
    };
} __attribute__((__packed__)) ConfigurationKeyID_t;

typedef struct xCfgValGetHeader_t {
    uint8_t version;
    Layers_t layer;
    uint16_t position;
} CfgValGetHeader_t;

typedef struct xUbloxProtocol_t {
    uint32_t rx_time_stamp;
    uint32_t rx_pkt_cnt;
    uint32_t tx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t load_len;
    uint16_t exp_len;
#ifdef HAS_DEBUG
    uint16_t min_len;
    uint16_t max_len;
    uint32_t sync_cnt;
#endif
    uint16_t read_crc;
    volatile uint32_t ack_cnt;
    volatile bool ack;
    uint8_t rx_state;
    uint8_t rx_frame[UBX_RX_FRAME_SIZE];
    uint8_t fix_frame[UBX_RX_FRAME_SIZE];
    bool unproc_frame;
    bool diag;
} UbloxProtocol_t;

typedef struct xUbxHeader_t {
    uint8_t class;
    uint8_t id;
} UbxHeader_t;

typedef struct xStatClass_t {
    uint8_t class_val;
    uint16_t cnt;
} StatClass_t;


#ifdef __cplusplus
}
#endif

#endif /* UBX_TYPES_H */
