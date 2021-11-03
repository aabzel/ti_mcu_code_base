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
    uint8_t version   ; /*0 Message version*/
    uint16_t reserved0; /*1*/
    uint32_t iTOW     ; /*4 GPS time of week of the navigation epoch.*/
    int32_t lon       ; /*8 Longitude*/
    int32_t  lat      ; /*12 Latitude*/
    int32_t  height   ; /*16 Height above ellipsoid*/
    int32_t  hMSL     ; /*20 Height above mean sea level*/
    int8_t   lonHp    ; /*24*/
    int8_t latHp      ; /*25*/
    int8_t  heightHp  ; /*26*/
    int8_t  hMSLHp    ; /*17*/
    uint32_t  hAcc    ; /*28*/
    uint32_t vAcc     ; /*32*/
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
        uint8_t utc_standard:7;     /*UTC standard identifier*/
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
    int32_t vel_north   ; /*4       cm/s North velocity component*/
    int32_t vel_east   ;  /*8       cm/s East velocity component*/
    int32_t  vel_down  ;  /*12      cm/s Down velocity component*/
    uint32_t speed ;      /*16      cm/s Speed (3-D)*/
    uint32_t speed_ground;/*20      cm/s Ground speed (2-D)*/
    int32_t heading;      /*24 1e-5 deg  Heading of motion 2-D*/
    uint32_t  speed_acc ; /*28      cm/s Speed accuracy Estimate*/
    uint32_t course_acc ; /*32 1e-5 deg  Course / Heading accuracy estimate*/
}NavVelNed_t;


 
#ifdef __cplusplus
}
#endif

#endif /* UBX_TYPES_H */
