#ifndef SX1262_LORA_PARAMS_H
#define SX1262_LORA_PARAMS_H

#include "param_ids.h"
#include "param_types.h"

#define PARAMS_SX1262_LORA                                                       \
    {PAR_ID_LORA_SF, 1, UINT8, "SF"},            /*Chips / Symbol*/              \
    {PAR_ID_LORA_CR, 1, UINT8, "CR"},           /*in raw bits/total bits*/       \
    {PAR_ID_LORA_BW, 1, UINT8, "BW"},            /*Hz*/                          \
    {PAR_ID_LOW_DATA_RATE, 1, UINT8, "LowDataRate"},                             \
    {PAR_ID_LORA_HEADER_TYPE, 1, UINT8, "LoRaHeadType"},                         \
    {PAR_ID_LORA_SYNC_WORD, 2, UINT16, "LoRaSyncWord"},                          \
    {PAR_ID_CRC_TYPE, 1, UINT8, "CRC_T"},                                        \
    {PAR_ID_IQ_SETUP, 1, UINT8, "IQSetUp"}, 

#endif /* SX1262_LORA_PARAMS_H  */
