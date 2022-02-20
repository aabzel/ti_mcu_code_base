#ifndef SX1262_PARAMS_H
#define SX1262_PARAMS_H

#include "param_ids.h"
#include "param_types.h"

#ifdef HAS_GFSK
#include "sx1262_gfsk_params.h"
#else
#define PARAMS_SX1262_GFSK
#endif

#ifdef HAS_LORA
#include "sx1262_lora_params.h"
#else
#define PARAMS_SX1262_LORA
#endif

#define PARAMS_SX1262       \
    PARAMS_SX1262_LORA      \
    PARAMS_SX1262_GFSK      \
    {PAR_ID_LORA_FREQ, 4, UINT32, "Freq"},   /*Hz*/                              \
    {PAR_ID_TX_MUTE, 1, UINT8, "TxMute"},                                        \
    {PAR_ID_PAYLOAD_LENGTH, 1, UINT8, "PAY_LEN"},   /*bytes*/                    \
    {PAR_ID_LORA_CRC_INIT, 2, UINT16, "CrcInit"},   /*CRC Initial Value*/        \
    {PAR_ID_LORA_CRC_POLY, 2, UINT16, "CrcPoly"},   /*CRC Polynomial*/           \
    {PAR_ID_PREAMBLE_LENGTH, 2, UINT16, "PRE_LEN"}, /*bytes*/                    \
    {PAR_ID_LORA_OUT_POWER, 1, INT8, "outPower"}, /*loRa output power*/          \
    {PAR_ID_LORA_MAX_LINK_DIST, 8, DOUBLE, "MaxLinkDist"}, /*Max Link Distance*/ \
    {PAR_ID_LORA_MAX_BIT_RATE, 8, DOUBLE, "MaxBitRate"}, /*Max bit/rate*/   \
    {PAR_ID_RETX_CNT, 1, UINT8, "ReTxCnt"},


#endif /* SX1262_PARAMS_H  */
