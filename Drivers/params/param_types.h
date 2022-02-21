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


#endif /* PARAM_TYPES_H  */
