/* SX1262 22 dBm power output
 * sub-GHz band (e.g. 868 or 915 MHz)
 * */

#ifndef SX1262_DRV_H
#define SX1262_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "bit_utils.h"
#include "board_layout.h"
#include "gfsk_types.h"
#ifdef HAS_LOG
#include "log.h"
#endif
#include "lora_constants.h"
#include "sx1262_config.h"
#include "sx1262_op_codes.h"
#include "sx1262_registers.h"
#include "sx1262_types.h"

#define PACK_SIZE_BYTES 16

#define FIFO_SIZE 256

#define RX_SIZE 256
#define TX_SIZE 256

#define RC_FREQ_HZ 13000000U
#define XTAL_FREQ_HZ 32000000U
#define SX1262_PERIOD_US 300U

#define FREQ_DIV 33554432U // 0x02000000 ( double )pow( 2.0, 25.0 )
#define SX1262_REG_CNT 36U
#define OPCODE_SIZE 1
#define MHZ_TO_FRF 1048576 /* ((float)XTAL_FREQ_HZ))*10000000.0f)*/

extern Sx1262_t Sx1262Instance;
#ifdef HAS_SX1262_DEBUG
extern const xSx1262Reg_t RegMap[SX1262_REG_CNT];
#endif

#define SX1262_CHIP_SELECT(CALL_BACK)                                                                                  \
    do {                                                                                                               \
        res = false;                                                                                                   \
        res = sx1262_wait_on_busy(1000);                                                                               \
        if(true == res) {                                                                                              \
            res = true;                                                                                                \
            res = sx1262_chip_select(true);                                                                            \
            res = wait_ms(2);                                                                                          \
            res = CALL_BACK;                                                                                           \
            res = wait_ms(2);                                                                                          \
            res = sx1262_chip_select(false);                                                                           \
        } else {                                                                                                       \
            Sx1262Instance.busy_cnt++;                                                                                 \
            /*LOG_ERROR(LORA,"SX1262Busy");*/                                                                              \
            res = false;                                                                                               \
        }                                                                                                              \
    } while(0);

float dbm2watts(int32_t dbm);
bool is_valid_bandwidth(BandWidth_t bandwidth);
bool is_valid_spreading_factor(SpreadingFactor_t Spreading_factor);
bool is_valid_coding_rate(LoRaCodingRate_t coding_rate);
bool is_power_valid(int8_t power);

bool sx1262_get_dev_err(uint16_t* op_error);
bool sx1262_get_irq_status(Sx1262IRQs_t* irq_stat);
bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, int8_t* RssiPkt, uint8_t* SnrPkt,
                              int8_t* SignalRssiPkt);
bool sx1262_get_packet_type(RadioPacketType_t* const packet_type);
bool sx1262_get_rssi_inst(int8_t* rssi_inst);
bool sx1262_get_rx_payload(uint8_t* payload, uint16_t* size, uint16_t max_size, uint8_t* crc8);
bool sx1262_get_rxbuff_status(uint8_t* PayloadLengthRx, uint8_t* RxStartBufferPointer);
bool sx1262_get_statistic(PaketStat_t* gfsk, PaketStat_t* lora);
bool sx1262_get_status(uint8_t* out_status);
bool sx1262_init(void);
bool sx1262_int_isr(Sx1262_t* sx1262Instance);
bool sx1262_is_connected(void);
bool sx1262_process(void);
bool sx1262_reset(void);
bool sx1262_reset_stats(void);
bool sx1262_set_dio2_as_rf_switch_ctrl(Dio2Mode_t mode);
bool sx1262_clear_dev_error(void);
bool sx1262_clear_fifo(void);
bool sx1262_clear_irq(uint16_t clear_irq_param);
bool sx1262_chip_select(bool state);
bool sx1262_conf_rx(void);
bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len);
bool sx1262_set_buffer_base_addr(uint8_t tx_addr, uint8_t rx_addr);
bool sx1262_set_cad(void);
bool sx1262_set_cad_params(CadNunSym_t cad_symbol_num, uint8_t cad_det_peak, /*See App Application note AN1200.48 */
                           uint8_t cad_det_min,                              /*See App Application note AN1200.48 */
                           CadExtMode_t cad_exit_mode, uint16_t cad_timeout);
bool sx1262_set_crc_polynomial(uint16_t polynomial);
bool sx1262_set_crc_seed(uint16_t seed);
bool sx1262_set_dio_irq_params(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask);
bool sx1262_set_modulation_params(ModulationParams_t* modParams);
bool sx1262_set_pa_config(uint8_t pa_duty_cycle, uint8_t hp_max, uint8_t device_sel, uint8_t pa_lut);
bool sx1262_set_packet_params(PacketParam_t* packParam);
bool sx1262_set_packet_type(RadioPacketType_t packet_type);
bool sx1262_set_payload(uint8_t* payload, uint8_t size);
bool sx1262_set_regulator_mode(uint8_t reg_mode_param);
bool sx1262_set_rf_frequency(uint32_t rf_frequency_hz, uint32_t freq_xtal_hz);
bool sx1262_set_sleep(uint8_t sleep_config);

bool sx1262_set_standby(StandbyMode_t stdby_config);
bool sx1262_set_sync_word(uint64_t sync_word);
bool sx1262_set_tx_params(int8_t power, uint8_t ramp_time);
bool sx1262_set_whitening_seed(uint16_t seed);
bool sx1262_start_rx(uint32_t timeout_s);
bool sx1262_start_tx(uint8_t* tx_buf, uint8_t pktLen, uint32_t timeout_s);
bool sx1262_wait_on_busy(uint32_t time_out_ms);
bool sx1262_wakeup(void);
bool sx1262_write_buffer(uint8_t offset, uint8_t* payload, uint16_t payload_len);
bool sx1262_read_buffer(int16_t offset, uint8_t* payload, uint16_t payload_len);
bool sx1262_write_reg(uint16_t reg_addr, uint8_t reg_val);
float lora_calc_data_rate(uint8_t sf_code, uint8_t bw_code, uint8_t cr_code);
#ifdef HAS_DEBUG
float lora_calc_max_frame_tx_time(uint8_t sf_code, uint8_t bw_code, uint8_t cr_code, uint16_t n_preamble,
                                  uint8_t header, uint8_t low_data_rate_opt, float* Tsym, float* t_preamble);
#endif
#endif /* SX1262_DRV_H  */
