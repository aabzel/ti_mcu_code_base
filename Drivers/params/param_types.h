#ifndef PARAM_TYPES_H
#define PARAM_TYPES_H

#include "param_constants.h"

typedef struct xParamItem_t {
    Id_t id;
    uint16_t len;
    ParamType_t type;
    char* name;
    /*char* units*/
} ParamItem_t;

typedef union uLinkInfoId_t {
    uint16_t id;
    struct {
        uint8_t spreading_factor :4;  /**/
        uint8_t band_width:4;         /**/
        uint8_t coding_rate:4;        /**/
        uint8_t type:4;               /**/
    };
} LinkInfoId_t;

#endif /* PARAM_TYPES_H  */
