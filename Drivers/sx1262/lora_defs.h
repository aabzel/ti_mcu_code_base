

#ifndef LORA_DEFS_H
#define LORA_DEFS_H

#include <stdbool.h>
#include <stdint.h>

// BW Description Table 13-48: LoRa® ModParam2 - BW
#define LORA_BW_7 0x00   /*7.81 kHz real */
#define LORA_BW_10 0x08  /*10.42 kHz real*/
#define LORA_BW_15 0x01  /*15.63 kHz real*/
#define LORA_BW_20 0x09  /*20.83 kHz real*/
#define LORA_BW_31 0x02  /*31.25 kHz real*/
#define LORA_BW_41 0x0A  /*41.67 kHz real*/
#define LORA_BW_62 0x03  /*62.50 kHz real*/
#define LORA_BW_125 0x04 /*125 kHz real  */
#define LORA_BW_250 0x05 /*250 kHz real  */
#define LORA_BW_500 0x06 /*500 kHz real  */

/*Table 13-49: LoRa® ModParam3 - CR */
#define LORA_CR_4_5 0x01
#define LORA_CR_4_6 0x02
#define LORA_CR_4_7 0x03
#define LORA_CR_4_8 0x04

/*Table 13-47: LoRa® ModParam1- SF */
#define SF5 0x05
#define SF6 0x06
#define SF7 0x07
#define SF8 0x08
#define SF9 0x09
#define SF10 0x0A
#define SF11 0x0B
#define SF12 0x0C

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
#define LORA_FIX_LEN_PAC 0x01  /*(implicit header)*/

typedef struct xLoRaPacketParam_t {
    uint16_t preamble_length;
    uint8_t header_type;
    uint8_t payload_length;
    uint8_t crc_type;
    uint8_t invert_iq;
} LoRaPacketParam_t;

#endif /* LORA_DEFS_H  */
