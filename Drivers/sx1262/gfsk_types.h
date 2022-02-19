#ifndef GFSK_CONST_H
#define GFSK_CONST_H

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
    uint8_t whitening;                /* packetParam9*/
} GfskPacketParam_t;

typedef struct xGfskModulationParams_t {
    uint32_t bit_rate;
    uint32_t frequency_deviation;
    uint8_t pulse_shape;
    uint8_t bandwidth;
} GfskModulationParams_t;


#endif /* GFSK_CONST_H  */
