#ifndef SX1262_PARAMS_H
#define SX1262_PARAMS_H

#include "param_ids.h"
#include "param_types.h"

#define PARAMS_SX1262   \
    {PAR_ID_LORA_FREQ, 4, UINT32, "LoRaFreq"},   /*Hz*/                          \
    {PAR_ID_LORA_SF, 1, UINT8, "SF"},            /*Chips / Symbol*/              \
    {PAR_ID_LORA_CR, 1, UINT8, "CR"},           /*in raw bits/total bits*/       \
    {PAR_ID_LORA_BW, 1, UINT8, "BW"},            /*Hz*/                          \
    {PAR_ID_HEADER_TYPE, 1, UINT8, "HEAD_TYPE"},                                 \
    {PAR_ID_PACKET_TYPE, 1, UINT8, "PktType"},               /* GFSK / LoRa*/    \
    {PAR_ID_LORA_SYNC_WORD, 2, UINT16, "LoRaSyncWord"},                          \
    {PAR_ID_SYNC_WORD, 8, UINT64, "SyncWord"},                                   \
    {PAR_ID_LORA_OUT_POWER, 1, INT8, "outPower"}, /*loRa output power*/          \
    {PAR_ID_LOW_DATA_RATE, 1, UINT8, "LowDataRate"},                             \
    {PAR_ID_PREAMBLE_LENGTH, 2, UINT16, "PRE_LEN"}, /*bytes*/                    \
    {PAR_ID_LORA_CRC_INIT, 2, UINT16, "CrcInit"},   /*CRC Initial Value*/        \
    {PAR_ID_LORA_CRC_POLY, 2, UINT16, "CrcPoly"},   /*CRC Polynomial*/           \
    {PAR_ID_PAYLOAD_LENGTH, 1, UINT8, "PAY_LEN"},   /*bytes*/                    \
    {PAR_ID_CRC_TYPE, 1, UINT8, "CRC_T"},                                        \
    {PAR_ID_LORA_MAX_LINK_DIST, 8, DOUBLE, "MaxLinkDist"}, /*Max Link Distance*/ \
    {PAR_ID_IQ_SETUP, 1, UINT8, "IQSetUp"},                                      \
    {PAR_ID_LORA_MAX_BIT_RATE, 8, DOUBLE, "MaxBitRate"}, /*Max LoRa bit/rate*/   \
    {PAR_ID_RETX_CNT, 1, UINT8, "ReTxCnt"},


#endif /* SX1262_PARAMS_H  */
