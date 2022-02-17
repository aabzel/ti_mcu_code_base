#ifndef RTK_PARAMS_H
#define RTK_PARAMS_H

#include "param_ids.h"
#include "param_types.h"

#define PARAMS_RTK                                                          \
    {PAR_ID_BASE_LOCATION, 16, STRUCT, "BaseLocat"},                            \
    {PAR_ID_RTK_FIX_LONG, 4, UINT32, "FTKFixedTime"}, /*RTK fixed max duration*/\
    {PAR_ID_BASE_ACC, 4, UINT32, "BaseAcc"},                                    \
    {PAR_ID_GPS, 1, BOOL, "GpsCpr"},                                            \
    {PAR_ID_GLONASS, 1, BOOL, "GlonassCor"},                                    \
    {PAR_ID_GALILEO, 1, BOOL, "GalileoCor"},                                    \
    {PAR_ID_BEI_DOU, 1, BOOL, "BeiDouCor"},                                     \
    {PAR_ID_RTK_CHANNEL, 1, UINT8, "RTKchannel"},                               \
    {PAR_ID_GNSS_PERIOD, 2, UINT16, "GnssPer"},                                 \
    {PAR_ID_BASE_ALT, 8, DOUBLE, "BaseAlt"}, 

#endif /* RTK_PARAMS_H  */
