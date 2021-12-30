#ifndef SX1262_TYPES_H
#define SX1262_TYPES_H

#include <stdint.h>

#include "sx1262_constants.h"

typedef struct xPaketStat_t {
    uint16_t nb_pkt_received;
    uint16_t nb_pkt_crc_error;
    uint16_t nb_pkt_length_error;
    uint16_t nb_pkt_header_err;
} PaketStat_t;

typedef struct xSx1262Reg_t {
    uint16_t addr;
    char* reg_name;
} xSx1262Reg_t;

typedef struct xModulationParams_t {
    BandWidth_t band_width;
    SpreadingFactor_t spreading_factor;
    LoRaCodingRate_t coding_rate;
    uint8_t low_data_rate_optimization; //(LDRO)
} ModulationParams_t;


typedef struct xLoRaPacketParam_t {
    uint16_t preamble_length;/*PacketParam1 PacketParam2 */
    uint8_t header_type; /*PacketParam3*/
    uint8_t payload_length;/*PacketParam4*/
    uint8_t crc_type;/*PacketParam5*/
    uint8_t invert_iq; /*PacketParam6*/
} LoRaPacketParam_t;


typedef union uPacketParamProto_t {
    GfskPacketParam_t gfsk;
    LoRaPacketParam_t lora;
} PacketParamProto_t;

typedef struct xSx1262IrqCnt_t {
    uint16_t total;
    uint16_t tx_done;
    uint16_t rx_done;
    uint16_t preamble_detected;
    uint16_t syncword_valid;
    uint16_t header_valid;
    uint16_t header_err;
    uint16_t crc_err;
    uint16_t cad_done;
    uint16_t cad_detected;
    uint16_t timeout;
} Sx1262IrqCnt_t;

typedef struct sPacketParam_t {
    RadioPacketType_t packet_type;
    PacketParamProto_t proto;
} PacketParam_t;

typedef union uSx1262Status_t{
      uint8_t byte;
      struct {
          uint8_t reserved1 :1;      /*bit 0  :*/
          uint8_t command_status:3;  /*bit 3-1:*/
          uint8_t chip_mode:3;       /*bit 6-4:*/
          uint8_t reserved7:1;       /*bit 7:  */
      };
} Sx1262Status_t;

typedef struct xSx1262_t {
    uint64_t set_sync_word;
    uint64_t get_sync_word;
    uint32_t int_cnt;
    uint32_t rf_frequency_hz;
    uint32_t data_aval_cnt;
    float bit_rate;
#ifdef HAS_SX1262_BIT_RATE
    uint32_t tx_start_time_stamp_ms;
    uint32_t tx_last_size;
    double tx_max_bit_rate;
#endif /*HAS_SX1262_BIT_RATE*/
    uint32_t tx_done_time_stamp_ms;
    uint32_t busy_cnt;
    uint32_t rand_num;
    uint16_t lora_sync_word_set;
    uint16_t lora_sync_word_get;
    uint16_t op_error;
    uint16_t irq_stat;
    uint8_t status;
    Sx1262Status_t dev_status;
   // uint8_t com_stat;
    uint8_t rx_payload_len;
    uint8_t rx_buffer_pointer;
    uint8_t rx_status;
    uint8_t rssi_sync;
    uint8_t rssi_avg;
    uint8_t wire_int;
    uint8_t wire_rst;
    uint8_t wire_nss;
    uint8_t wire_busy;
    uint32_t tx_time_out_ms;
    int8_t rssi_pkt;
    uint8_t snr_pkt;
    int8_t signal_rssi_pkt;
    int8_t output_power;
    int8_t rssi_inst;
    bool tx_done;
    bool tx_mute;
    bool debug;
    bool sync_reg;
    bool show_bin;
    bool show_ascii;
    bool is_packet;
    uint32_t rx_done_cnt;
    uint8_t rx_size_max;
    uint8_t tx_size_max;
    uint32_t tx_done_cnt;
    //ChipMode_t chip_mode;
    PaketStat_t gfsk;
    PaketStat_t lora;
    Sx1262IrqCnt_t irq_cnt;
    ModulationParams_t mod_params;
    PacketParam_t packet_param;
    RadioPacketType_t packet_type;
} Sx1262_t;


#endif /* SX1262_TYPES_H  */
