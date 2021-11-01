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

 
#ifdef __cplusplus
}
#endif

#endif /* UBX_TYPES_H */
