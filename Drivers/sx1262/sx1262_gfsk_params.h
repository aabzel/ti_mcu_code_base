#ifndef SX1262_GFSK_PARAMS_H
#define SX1262_GFSK_PARAMS_H

#include "param_ids.h"
#include "param_types.h"

#define PARAMS_SX1262_GFSK                                   \
    {PAR_ID_SYNC_WORD, 8, UINT64, "SyncWord"},               \
    {PAR_ID_PRE_DET_LEN, 1, UINT8, "PreambleDetectorLength"},\
    {PAR_ID_ADDR_COMP, 1, UINT8, "AddrComp"},                \
    {PAR_ID_GFSK_BANDWIDTH, 1, UINT8, "Bandwidth"},          \
    {PAR_ID_NODE_ADDR, 1, UINT8, "NodeAddr"},                \
    {PAR_ID_GFSK_HEADER_TYPE, 1, UINT8, "FskHeadType"},      \
    {PAR_ID_BROADCAST_ADDR, 1, UINT8, "BroadCastAddr"},      \
    {PAR_ID_GFSK_CRC_TYPE, 1, UINT8, "CrcGfsk"},             \
    {PAR_ID_WHITENING, 1, UINT8, "Whitening"},               \
    {PAR_ID_WHITENING_INIT_VALUE, 2, UINT16, "WhiteningInit"},

#endif /* SX1262_GFSK_PARAMS_H  */
