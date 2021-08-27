/*
 * */

#ifndef SX1262_DRV_H
#define SX1262_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "board_layout.h"

#define PACK_SIZE_BYTES 16

#define RX_SIZE 256
#define TX_SIZE 256

#define XTAL_FREQ_HZ 32000000U
#define SX1262_SPI_NUM 0
#define SX1262_REG_CNT 26U
#define OPCODE_SIZE 1
#define MHZ_TO_FRF 1048576 // = ()( ((float)(1<<25) ) / ((float)XTAL_FREQ_HZ))*10000000.0f)

#define TX_BASE_ADDRESS 0x00
#define RX_BASE_ADDRESS 0x80 // 128

/*Operational Modes Commands*/
#define OPCODE_RESET_STATS 0x00
#define OPCODE_CLEAR_IRQ_STATUS 0x02
#define OPCODE_CLEAR_DEVICE_ERRORS 0x07
#define OPCODE_SET_DIO_IRQ_PARAMS 0x08
#define OPCODE_WRITE_REGISTER 0x0D
#define OPCODE_WRITE_BUFFER 0x0E
#define OPCODE_GET_STATS 0x10
#define OPCODE_GET_PACKET_TYPE 0x11
#define OPCODE_GET_IRQ_STATUS 0x12
#define OPCODE_GET_RX_BUFFER_STATUS 0x13
#define OPCODE_GET_PACKET_STATUS 0x14
#define OPCODE_GET_RSSIINST 0x15
#define OPCODE_GET_DEVICE_ERRORS 0x17
#define OPCODE_READ_REGISTER 0x1D
#define OPCODE_READ_BUFFER 0x1E
#define OPCODE_SET_STANDBY 0x80
#define OPCODE_SET_RX 0x82
#define OPCODE_SET_TX 0x83
#define OPCODE_SET_SLEEP 0x84
#define OPCODE_SET_RF_FREQUENCY 0x86
#define OPCODE_SET_CAD_PARAM 0x88
#define OPCODE_CALIBRATE 0x89
#define OPCODE_SET_PACKET_TYPE 0x8A
#define OPCODE_SET_MODULATION_PARAMS 0x8b
#define OPCODE_SET_PACKET_PARAMS 0x8c
#define OPCODE_SET_TX_PARAMS 0x8e
#define OPCODE_SET_BUFFER_BASE_ADDR 0x8F
#define OPCODE_SET_FALLBACK_MODE 0x93
#define OPCODE_SET_RX_DUTY_CYCLE 0x94
#define OPCODE_SET_PA_CONFIG 0x95
#define OPCODE_SET_REGULATOR_MODE 0x96
#define OPCODE_SET_DIO3_AS_TCXO_CTRL 0x97
#define OPCODE_CALIBRATE_IMAGE 0x98
#define OPCODE_SET_DIO2_AS_RFSWITCH 0x9d
#define OPCODE_STOP_TIMER_ON_PREAMBLE 0x9f
#define OPCODE_SET_LORA_SYMBOL_TIMEOUT 0xa0
#define OPCODE_GET_STATUS 0xC0
#define OPCODE_SET_FS 0xc1
#define OPCODE_SET_CAD 0xc5
#define OPCODE_SET_TX_CARRIER 0xd1
#define OPCODE_SET_TX_PREAMBLE 0xd2

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

/*Status Bytes Definition*/
#define COM_STAT_DATA_AVAIL 0x2 /*Data is available to host*/
#define COM_STAT_COM_TIMEOUT 0x3 /*Command timeout2 */
#define COM_STAT_COM_PROC_ERR 0x4 /*Command processing error3*/
#define COM_STAT_EXE_ERR 0x5 /*Failure to execute command4*/
#define COM_STAT_COM_TX_DONE 0x6 /*Command TX done5 */

/* OpError Bits */
#define  OP_ERR_BIT_RC64K_CALIB_ERR 0/*RC64k calibration failed */
#define  OP_ERR_BIT_RC13M_CALIB_ERR 1/*RC13M calibration failed */
#define  OP_ERR_BIT_PLL_CALIB_ERR   2/*PLL calibration failed   */
#define  OP_ERR_BIT_ADC_CALIB_ERR   3/*ADC calibration failed   */
#define  OP_ERR_BIT_IMG_CALIB_ERR   4/*IMG calibration failed   */
#define  OP_ERR_BIT_XOSC_START_ERR  5/*XOSC failed to start     */
#define  OP_ERR_BIT_PLL_LOCK_ERR    6/*PLL failed to lock       */
#define  OP_ERR_BIT_PA_RAMP_ERR     8/*PA ramping failed        */

typedef enum eChipMode_t { CHIPMODE_NONE = 0, CHIPMODE_RX = 1, CHIPMODE_TX = 2, CHIPMODE_UNDEF = 3 } ChipMode_t;

typedef enum eRadioPacketTypes_t {
    PACKET_TYPE_GFSK = 0x00,
    PACKET_TYPE_LORA = 0x01,
    PACKET_TYPE_NONE = 0x0F,
    PACKET_TYPE_UNDEF = 0xFF
} RadioPacketTypes_t;


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

typedef struct xSx1262_t {
    uint32_t busy_cnt;
    uint16_t op_error;
    uint16_t irq_stat;
    uint8_t status;
    uint8_t com_stat;
    uint8_t chip_mode;
    uint8_t rx_payload_length;
    uint8_t rx_start_buffer_pointer;
    uint8_t rx_status;
    uint8_t rssi_sync;
    uint8_t rssi_avg;
    uint8_t rssi_pkt;
    uint8_t snr_pkt;
    uint8_t signal_rssi_pkt;
    int8_t rssi_inst;
    PaketStat_t gfsk;
    PaketStat_t lora;
    ChipMode_t tx_mode;
} Sx1262_t;

extern Sx1262_t Sx1262Instance;

extern const xSx1262Reg_t RegMap[SX1262_REG_CNT];

bool sx1262_init(void);
bool sx1262_get_irq_status(uint16_t* irq_stat);
bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, uint8_t* RssiPkt, uint8_t* SnrPkt,
                              uint8_t* SignalRssiPkt);
bool sx1262_get_packet_type(RadioPacketTypes_t* packet_type);
bool sx1262_get_payload(uint8_t* payload, uint8_t* size, uint8_t max_size);
bool sx1262_get_rand(uint32_t* rand_num);
bool sx1262_get_rxbuff_status(uint8_t* PayloadLengthRx, uint8_t* RxStartBufferPointer);
bool sx1262_get_rssi_inst(int8_t* rssi_inst);
bool sx1262_get_status(uint8_t* out_status);
bool sx1262_get_statistic(PaketStat_t* gfsk, PaketStat_t* lora);
bool sx1262_get_sync_word(uint64_t* sync_word);
bool sx1262_get_dev_err(uint16_t* op_error);
bool sx1262_process(void);
bool sx1262_read_reg(uint16_t reg_addr, uint8_t* reg_val);
bool sx1262_reset(void);
// void sx1262_set_TxParams( int8_t power, RadioRampTimes_t rampTime );
// void sx1262_set_ModulationParams( ModulationParams_t *modParams );
bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len);
bool sx1262_set_base_addr(uint8_t tx_addr, uint8_t rx_addr);
void sx1262_set_crc_polynomial(uint16_t polynomial);
bool sx1262_set_crc_seed(uint16_t seed);
bool sx1262_set_dio_irq_params(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask);
bool sx1262_set_packet_type(RadioPacketTypes_t packet_type);
bool sx1262_set_rffrequency(float rf_frequency_mhz);
bool sx1262_set_sync_word(uint64_t sync_word);
void sx1262_set_whitening_seed(uint16_t seed);
bool sx1262_start_tx(uint8_t offset, uint8_t* tx_buf, uint8_t pktLen, uint32_t timeout_s);
bool sx1262_start_rx(uint32_t timeout_s);
bool sx1262_wait_on_busy(uint32_t time_out_ms);
bool sx1262_write_reg(uint16_t reg_addr, uint8_t reg_val);

#endif /* SX1262_DRV_H  */
