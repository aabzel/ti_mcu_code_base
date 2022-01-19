#ifndef SX1262_DIAG_H
#define SX1262_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "lora_constants.h"
#include "sx1262_drv.h"

char* HexWord2Str(uint16_t word);
const char* LowDataRateOpt2Str(uint8_t packet_type);
char* dbm2wattsStr(int32_t dbm);
char* LinkDist2str(double bit_s);
char* PayloadLen2Str(uint8_t size);
const char* IqSetUp2Str(uint8_t iq_setup);
char* SyncWord2Str(uint64_t syn_word);
char* RfFreq2Str(uint32_t rf_freq);
char* PreambleLen2Str(uint16_t pre_len);
char* BitRate2Str(double bit_rate);
const char* PacketType2Str(uint8_t packet_type);
const char* LoraHeaderType2Str(uint8_t header_type);
const char* LoraCrcType2Str(uint8_t crc_type);
bool parse_dev_stat(uint8_t dev_stat);
bool parse_irq_stat(Sx1262IRQs_t irq_stat);
bool parse_op_error(uint16_t op_error);
bool print_int_diag(Sx1262IrqCnt_t* irq_cnt);
bool printf_pack_stat(PaketStat_t* pack_stat, char* name);
uint32_t spreading_factor2num(SpreadingFactor_t spreading_factor);
const char* chip_mode2str(uint8_t chip_mode);
const char* cmd_stat2str(uint8_t cmd_stat);
const char* pack_type2str(RadioPacketType_t packet_type);
const char* coding_rate2str(LoRaCodingRate_t coding_rate);
char* bandwidth2str(uint8_t bandwidth);
const char* spreading_factor2str(uint8_t spreading_factor);

#ifdef __cplusplus
}
#endif

#endif /* SX1262_DIAG_H */
