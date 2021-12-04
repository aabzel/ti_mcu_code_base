/* SX1262 22 dBm power output
 * sub-GHz band (e.g. 868 or 915 MHz)
 * */

#ifndef SX1262_DRV_H
#define SX1262_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "bit_utils.h"
#include "board_layout.h"
#include "gfsk_defs.h"
#include "lora_defs.h"
#include "sx1262_config.h"
#include "sx1262_op_codes.h"

#define PACK_SIZE_BYTES 16

#define FIFO_SIZE 256

#define RX_SIZE 256
#define TX_SIZE 256

#define RC_FREQ_HZ 13000000U
#define XTAL_FREQ_HZ 32000000U
#define SX1262_PERIOD_US 300U

#define FREQ_DIV 33554432U // 0x02000000 ( double )pow( 2.0, 25.0 )
#define SX1262_REG_CNT 26U
#define OPCODE_SIZE 1
#define MHZ_TO_FRF 1048576 /* ((float)XTAL_FREQ_HZ))*10000000.0f)*/

/* Regiser Name       Address*/
#define SYNC_WORD_0 0x06C0
#define SYNC_WORD_1 0x06C1
#define SYNC_WORD_2 0x06C2
#define SYNC_WORD_3 0x06C3
#define SYNC_WORD_4 0x06C4
#define SYNC_WORD_5 0x06C5
#define SYNC_WORD_6 0x06C6
#define SYNC_WORD_7 0x06C7
#define RAND_NUM_GEN_0 0x0819
#define RAND_NUM_GEN_1 0x081A
#define RAND_NUM_GEN_2 0x081B
#define RAND_NUM_GEN_3 0x081C

#define IRQ_BIT_TXDONE 0           /* Packet transmission completed All   */
#define IRQ_BIT_RXDONE 1           /* Packet received All */
#define IRQ_BIT_PREAMBLEDETECTED 2 /* Preamble detected All   */
#define IRQ_BIT_SYNCWORDVALID 3    /* Valid sync word detected FSK   */
#define IRQ_BIT_HEADERVALID 4      /* Valid LoRa header received LoRa®   */
#define IRQ_BIT_HEADERERR 5        /* LoRa header CRC error LoRa®  */
#define IRQ_BIT_CRCERR 6           /* Wrong CRC received All  */
#define IRQ_BIT_CADDONE 7          /* Channel activity detection finished LoRa® */
#define IRQ_BIT_CADDETECTED 8      /* Channel activity detected LoRa®   */
#define IRQ_BIT_TIMEOUT 9          /* Rx or Tx timeout All */

#define IQR_ALL_INT MASK_10BIT

/*!
 * Represents the interruption masks available for the radio
 *
 * Note that not all these interruptions are available for all packet types
 */
typedef enum eRadioIrqMasks_t {
    IRQ_MASK_RADIO_NONE = 0x0000,
    IRQ_MASK_TX_DONE = 0x0001,
    IRQ_MASK_RX_DONE = 0x0002,
    IRQ_MASK_PREAMBLE_DETECTED = 0x0004,
    IRQ_MASK_SYNCWORD_VALID = 0x0008,
    IRQ_MASK_HEADER_VALID = 0x0010,
    IRQ_MASK_HEADER_ERROR = 0x0020,
    IRQ_MASK_CRC_ERROR = 0x0040,
    IRQ_MASK_CAD_DONE = 0x0080,
    IRQ_MASK_CAD_ACTIVITY_DETECTED = 0x0100,
    IRQ_MASK_RX_TX_TIMEOUT = 0x0200,
    IRQ_MASK_RADIO_ALL = 0xFFFF
} RadioIrqMasks_t;

/*Status Bytes Definition*/
#define COM_STAT_DATA_AVAIL 0x2 /*Data is available to host*/
#define COM_STAT_COM_TIMEOUT 0x3 /*Command timeout2 */
#define COM_STAT_COM_PROC_ERR 0x4 /*Command processing error3*/
#define COM_STAT_EXE_ERR 0x5 /*Failure to execute command4*/
#define COM_STAT_COM_TX_DONE 0x6 /*Command TX done5 */

/* OpError Bits */
#define OP_ERR_BIT_RC64K_CALIB_ERR 0 /*RC64k calibration failed */
#define OP_ERR_BIT_RC13M_CALIB_ERR 1 /*RC13M calibration failed */
#define OP_ERR_BIT_PLL_CALIB_ERR 2 /*PLL calibration failed   */
#define OP_ERR_BIT_ADC_CALIB_ERR 3 /*ADC calibration failed   */
#define OP_ERR_BIT_IMG_CALIB_ERR 4 /*IMG calibration failed   */
#define OP_ERR_BIT_XOSC_START_ERR 5 /*XOSC failed to start     */
#define OP_ERR_BIT_PLL_LOCK_ERR 6 /*PLL failed to lock       */
#define OP_ERR_BIT_PA_RAMP_ERR 8 /*PA ramping failed        */

// Table 13-41: RampTime Definition
// RampTime Value RampTime (μs)
#define SET_RAMP_10U 0x00 /*10  */
#define SET_RAMP_20U 0x01 /*20  */
#define SET_RAMP_40U 0x02 /*40  */
#define SET_RAMP_80U 0x03 /*80  */
#define SET_RAMP_200U 0x04 /*200 */
#define SET_RAMP_800U 0x05 /*800 */
#define SET_RAMP_1700U 0x06 /*1700*/
#define SET_RAMP_3400U 0x07 /*3400*/

#define DEV_SEL_SX1262 0
#define DEV_SEL_SX1261 1

/*StdbyConfig Value Description*/
typedef enum eStandbyMode_t {
    STDBY_RC = 0,   /* Device running on RC13M, set STDBY_RC mode       */
    STDBY_XOSC = 1, /* Device running on XTAL 32MHz, set STDBY_XOSC mode*/
} StandbyMode_t;

#define REG_MODE_ONLY_LDO 0x00 /*used for all modes*/
#define REG_MODE_DC_DC_LDO 0x01 /*used for STBY_XOSC,FS, RX and TX modes*/

/*PacketType Definition*/
typedef enum eRadioPacketTypes_t {
    PACKET_TYPE_GFSK = 0x00, /*GFSK packet type*/
    PACKET_TYPE_LORA = 0x01, /*LORA mode*/
    PACKET_TYPE_NONE = 0x0F,
    PACKET_TYPE_UNDEF = 0xFF
} RadioPacketType_t;

typedef enum eOutputPower_t { OP_14_DBM = 0, OP_17_DBM = 1, OP_20_DBM = 2, OP_22_DBM = 3, OP_UNDEF = 4 } OutputPower_t;

typedef enum eChipMode_t {
    CHP_MODE_UNDEF = 0x0,
    CHP_MODE_STBY_RC = 0x2,
    CHP_MODE_STBY_XOSC = 0x3,
    CHP_MODE_FS = 0x4,
    CHP_MODE_RX = 0x5,
    CHP_MODE_TX = 0x6
} ChipMode_t;

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

typedef union uPacketParamProto_t {
    GfskPacketParam_t gfsk;
    LoRaPacketParam_t lora;
} PacketParamProto_t;

typedef struct sPacketParam_t {
    RadioPacketType_t packet_type;
    PacketParamProto_t proto;
} PacketParam_t;

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
    uint16_t op_error;
    uint16_t irq_stat;
    uint8_t status;
    uint8_t dev_status;
    uint8_t com_stat;
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
    bool debug;
    bool sync_reg;
    bool show_bin;
    bool show_ascii;
    bool is_packet;
    uint32_t rx_done_cnt;
    uint32_t tx_done_cnt;
    ChipMode_t chip_mode;
    PaketStat_t gfsk;
    PaketStat_t lora;
    Sx1262IrqCnt_t irq_cnt;
    ModulationParams_t mod_params;
    PacketParam_t packet_param;
    RadioPacketType_t packet_type;
} Sx1262_t;

extern Sx1262_t Sx1262Instance;

extern const xSx1262Reg_t RegMap[SX1262_REG_CNT];

float dbm2watts(int32_t dbm);
bool is_valid_bandwidth(BandWidth_t bandwidth);
bool is_power_valid(int8_t power);

bool sx1262_get_dev_err(uint16_t* op_error);
bool sx1262_get_irq_status(uint16_t* irq_stat);
bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, int8_t* RssiPkt, uint8_t* SnrPkt,
                              int8_t* SignalRssiPkt);
bool sx1262_get_packet_type(RadioPacketType_t* packet_type);
bool sx1262_get_rand(uint32_t* rand_num);
bool sx1262_get_rssi_inst(int8_t* rssi_inst);
bool sx1262_get_rx_payload(uint8_t* payload, uint8_t* size, uint16_t max_size);
bool sx1262_get_rxbuff_status(uint8_t* PayloadLengthRx, uint8_t* RxStartBufferPointer);
bool sx1262_get_statistic(PaketStat_t* gfsk, PaketStat_t* lora);
bool sx1262_get_status(uint8_t* out_status);
bool sx1262_get_sync_word(uint64_t* sync_word);
bool sx1262_init(void);
bool sx1262_int_isr(Sx1262_t* sx1262Instance);
bool sx1262_is_connected(void);
bool sx1262_process(void);
bool sx1262_read_reg(uint16_t reg_addr, uint8_t* reg_val);
bool sx1262_reset(void);
bool sx1262_reset_stats(void);
bool sx1262_clear_dev_error(void);
bool sx1262_clear_fifo(void);
bool sx1262_clear_irq(uint16_t clear_irq_param);
bool sx1262_chip_select(bool state);
bool sx1262_conf_rx(void);
bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len);
bool sx1262_set_buffer_base_addr(uint8_t tx_addr, uint8_t rx_addr);
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
