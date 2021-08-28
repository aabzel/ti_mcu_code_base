#ifndef GFSK_DEFS_H
#define GFSK_DEFS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct xGfskPacketParam_t {
    uint16_t preamble_length;         /* packetparam1 & packetparam2*/
    uint8_t preamble_detector_length; /* packetparam3*/
    uint8_t syncword_length;          /* packetparam4*/
    uint8_t addr_comp;                /* packetparam5*/
    uint8_t packet_type;              /* packetparam6*/
    uint8_t payload_length;           /* packetparam7*/
    uint8_t crc_type;                 /* packetparam8*/
} GfskPacketParam_t;

#endif /* GFSK_DEFS_H  */
