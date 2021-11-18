#ifndef LORA_DEFS_H
#define LORA_DEFS_H

#include <stdbool.h>
#include <stdint.h>

// BW Description Table 13-48: LoRa® ModParam2 - BW
typedef enum eBandWidth_t {
    LORA_BW_7 = 0x00,   /*7.81 kHz real */
    LORA_BW_10 = 0x08, /*10.42 kHz real*/
    LORA_BW_15 = 0x01,  /*15.63 kHz real*/
    LORA_BW_20 = 0x09, /*20.83 kHz real*/
    LORA_BW_31 = 0x02,  /*31.25 kHz real*/
    LORA_BW_41 = 0x0A,  /*41.67 kHz real*/
    LORA_BW_62 = 0x03,  /*62.50 kHz real*/
    LORA_BW_125 = 0x04, /*125 kHz real  */
    LORA_BW_250 = 0x05, /*250 kHz real  */
    LORA_BW_500 = 0x06, /*500 kHz real  */
    /*lack code 0x07*/
} BandWidth_t;

/*Table 13-49: LoRa® ModParam3 - Coding Rate*/
typedef enum eLoRaCodingRate_t {
    LORA_CR_4_5 = 0x01, /**/
    LORA_CR_4_6 = 0x02,
    LORA_CR_4_7 = 0x03,
    LORA_CR_4_8 = 0x04
} LoRaCodingRate_t;

/*Table 13-47: LoRa® ModParam1- spreading factor */
typedef enum eSpreadingFactor_t {
    SF5 = 0x05,  /*32 Chips / Symbol*/
    SF6 = 0x06,  /*64 Chips / Symbol*/
    SF7 = 0x07,  /*128 Chips / Symbol*/
    SF8 = 0x08,  /*256 Chips / Symbol*/
    SF9 = 0x09,  /*512 Chips / Symbol*/
    SF10 = 0x0A, /*1024 Chips / Symbol*/
    SF11 = 0x0B, /*2048 Chips / Symbol*/
    SF12 = 0x0C  /*4096 Chips / Symbol*/
} SpreadingFactor_t;

/*
Table 13-69: LoRa® PacketParam5 - CRCType
CRCType Description*/
#define LORA_CRC_OFF 0x00
#define LORA_CRC_ON 0x01

#define STANDARD_IQ_SETUP 0x00
#define INVERTED_IQ_SETUP 0x01

/*
 * Table 13-67: LoRa® PacketParam3 - HeaderType
 * HeaderType Description
 */
#define LORA_VAR_LEN_PACT 0x00 /*(explicit header)*/
#define LORA_FIX_LEN_PAC 0x01 /*(implicit header)*/

typedef struct xLoRaPacketParam_t {
    uint16_t preamble_length;
    uint8_t header_type;
    uint8_t payload_length;
    uint8_t crc_type;
    uint8_t invert_iq;
} LoRaPacketParam_t;

#endif /* LORA_DEFS_H  */
