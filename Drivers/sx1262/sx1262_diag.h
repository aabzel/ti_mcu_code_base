#ifndef SX1262_DIAG_H
#define SX1262_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "lora_defs.h"
#include "sx1262_drv.h"

uint32_t bandwidth2num(BandWidth_t bandwidth);
bool parse_dev_stat(uint8_t dev_stat);
bool parse_irq_stat(uint16_t irq_stat);
char* bit_rate2str(float bit_s);
bool parse_op_error(uint16_t op_error);
bool print_int_diag(Sx1262IrqCnt_t* irq_cnt);
bool printf_pack_stat(PaketStat_t* pack_stat, char* name);
uint32_t spreading_factor2num(SpreadingFactor_t spreading_factor);
const char* chip_mode2str(uint8_t chip_mode);
const char* cmd_stat2str(uint8_t cmd_stat);
const char* pack_type2str(RadioPacketType_t packet_type);
const char* coding_rate2str(LoRaCodingRate_t coding_rate);
const char* bandwidth2str(uint8_t bandwidth);
const char* spreading_factor2str(uint8_t spreading_factor);

#ifdef __cplusplus
}
#endif

#endif /* SX1262_DIAG_H */
