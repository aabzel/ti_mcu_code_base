/*

CPOL= 0 and CPHA = 0 in Motorola/Freescale nomenclature.
Bytes follow from senior byte to junior byte (Motorola byte order or network byte order)
speed up to 16 MHz
*/
#include "sx1262_drv.h"

#include <gpio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bit_utils.h"
#include "board_layout.h"
#include "byte_utils.h"
#include "clocks.h"
#include "data_utils.h"
#include "debug_info.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "none_blocking_pause.h"
#include "rtcm3_protocol.h"
#include "spi_drv.h"
#include "sx1262_diag.h"
#include "sys_config.h"

#ifndef HAS_SPI
#error "SX1262 requires SPI driver"
#endif

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif

#ifdef HAS_PARAM
#include "param_ids.h"
#endif

#ifdef HAS_SX1262_DEBUG
#include "sx1262_diag.h"
#endif

bool sx1262_chip_select(bool state) {
    bool res = false;
    if(true == state) {
        GPIO_writeDio(DIO_SX1262_SS, 0);
        GPIO_writeDio(DIO_CAN_SS, 1);
        res = true;
    } else if(false == state) {
        GPIO_writeDio(DIO_SX1262_SS, 1);
        GPIO_writeDio(DIO_CAN_SS, 1);
        res = true;
    } else {
        res = false;
    }
    return res;
}

#define SX1262_CHIP_SELECT(CALL_BACK)                                                                                  \
    do {                                                                                                               \
        res = false;                                                                                                   \
        res = sx1262_wait_on_busy(0);                                                                                  \
        if(true == res) {                                                                                              \
            res = true;                                                                                                \
            res = sx1262_chip_select(true);                                                                            \
            res = wait_ms(2);                                                                                          \
            res = CALL_BACK;                                                                                           \
            res = wait_ms(2);                                                                                          \
            res = sx1262_chip_select(false);                                                                           \
        } else {                                                                                                       \
            Sx1262Instance.busy_cnt++;                                                                                 \
            res = false;                                                                                               \
        }                                                                                                              \
    } while(0);

#define GET_4_BYTE_OPCODE(OP_CODE, OUT_VAL_16_BIT)                                                                     \
    do {                                                                                                               \
        if(OUT_VAL_16_BIT) {                                                                                           \
            uint8_t rx_array[4];                                                                                       \
            memset(rx_array, 0x00, sizeof(rx_array));                                                                  \
            res = sx1262_send_opcode(OP_CODE, NULL, 0, rx_array, sizeof(rx_array));                                    \
            if(res) {                                                                                                  \
                Sx1262Instance.status = rx_array[1];                                                                   \
                *OUT_VAL_16_BIT = copy_and_rev16(&rx_array[2]);                                                        \
            } else {                                                                                                   \
                *OUT_VAL_16_BIT = 0x0000;                                                                              \
            }                                                                                                          \
        }                                                                                                              \
    } while(0);

Sx1262_t Sx1262Instance;

const xSx1262Reg_t RegMap[SX1262_REG_CNT] = {{0x06B8, "WhiteningInitValMSB"},
                                             {0x06B9, "WhiteningInitValLSB"},
                                             {0x06BC, "CRCMSBInitVal0"},
                                             {0x06BD, "CRCLSBInitVal1"},
                                             {0x06BE, "CRCMSBpolynomVal0"},
                                             {0x06BF, "CRCLSBpolynomVal1"},
                                             {SYNC_WORD_0, "SyncWord0"},
                                             {SYNC_WORD_1, "SyncWord1"},
                                             {SYNC_WORD_2, "SyncWord2"},
                                             {SYNC_WORD_3, "SyncWord3"},
                                             {SYNC_WORD_4, "SyncWord4"},
                                             {SYNC_WORD_5, "SyncWord5"},
                                             {SYNC_WORD_6, "SyncWord6"},
                                             {SYNC_WORD_7, "SyncWord7"},
                                             {0x06CD, "NodeAddr"},
                                             {0x06CE, "BroadcastAddr"},
                                             {0x0740, "LoRaSyncWordMSB"},
                                             {0x0741, "LoRaSyncWordLSB"},
                                             {RAND_NUM_GEN_0, "RandNumGen0"},
                                             {RAND_NUM_GEN_1, "RandNumGen1"},
                                             {RAND_NUM_GEN_2, "RandNumGen2"},
                                             {RAND_NUM_GEN_3, "RandNumGen3"},
                                             {0x08AC, "RxGain"},
                                             {0x08E7, "OCPConfig"},
                                             {0x0911, "XTAtrim"},
                                             {0x0912, "XTBtrim"}};

bool sx1262_wait_on_busy(uint32_t time_out_ms) {
    uint32_t start_ms = 0U;
    uint32_t curr_ms = 0U;
    uint32_t diff_ms = 0U;
    bool res = false;
    bool loop = true;
    uint32_t busy_value = 0;
    start_ms = get_time_ms32();
    while(loop) {
        busy_value = GPIO_readDio(DIO_SX1262_BUSY);
        if(0 == busy_value) {
            res = true;
            loop = false;
            break;
        }
        curr_ms = get_time_ms32();
        diff_ms = curr_ms - start_ms;
        if(time_out_ms < diff_ms) {
            res = false;
            loop = false;
            break;
        }
    }
    return res;
}

/*
  WriteRegister

  The command WriteRegister(...) allows writing a block of bytes in a data memory
  space starting at a specific address. The address is auto incremented after each
  data byte so that data is stored in contiguous memory locations. The SPI data
  transfer is described in the following table.
*/
#define WR_REG_PAYLOAD_SZ (3)
bool sx1262_write_reg(uint16_t reg_addr, uint8_t reg_val) {
    bool res = true;
#if 0
    uint8_t rx_array[WR_REG_PAYLOAD_SZ];
    memset(rx_array, 0x00, sizeof(rx_array));
#endif
    uint8_t tx_array[WR_REG_PAYLOAD_SZ];
    memset(tx_array, 0x00, sizeof(tx_array));
    uint16_t reg_addr_be = reverse_byte_order_uint16(reg_addr);
    memcpy(&tx_array[0], &reg_addr_be, sizeof(reg_addr_be));
    tx_array[2] = reg_val;
    res = sx1262_send_opcode(OPCODE_WRITE_REGISTER, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}
#define CONNECTION_SYNC_WORD 0x0012345678abcdef
bool sx1262_is_connected(void) {
    bool res = false;
    uint64_t read_sync_word = 0;
    uint64_t orig_sync_word = 0;

    res = sx1262_get_sync_word(&orig_sync_word);

    res = sx1262_set_sync_word(CONNECTION_SYNC_WORD) && res;
    res = sx1262_get_sync_word(&read_sync_word) && res;
    if((CONNECTION_SYNC_WORD == read_sync_word) && (true == res)) {
        res = true;
        res = sx1262_set_sync_word(orig_sync_word);
    } else {
        res = false;
    }
    return res;
}

/*TODO: rewrite*/
#define READ_REG_HEADER_SZ 3
static bool sx1262_read_reg_proc(uint16_t reg_addr, uint8_t* reg_val) {
    bool res = false;
    if(NULL != reg_val) {
        res = true;
        *reg_val = 0xFF;
        uint8_t tx_array[4] = {0};
        memset(tx_array, 0x00, sizeof(tx_array));
        Type16Union_t temp_reg_val;
        temp_reg_val.u16 = 0;
        tx_array[0] = OPCODE_READ_REGISTER;
        uint16_t reg_addr_nbo = reverse_byte_order_uint16(reg_addr);
        memcpy(&tx_array[1], &reg_addr_nbo, sizeof(reg_addr_nbo));
        res = spi_write((SpiName_t)SX1262_SPI_NUM, tx_array, READ_REG_HEADER_SZ) && res;
        res = spi_read((SpiName_t)SX1262_SPI_NUM, (uint8_t*)&temp_reg_val.u16, sizeof(temp_reg_val.u16)) && res;
        temp_reg_val.u16 = reverse_byte_order_uint16(temp_reg_val.u16);
        *reg_val = temp_reg_val.u8[0];
    } else {
        res = false;
    }

    return res;
}

bool sx1262_read_reg(uint16_t reg_addr, uint8_t* reg_val) {
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_read_reg_proc(reg_addr, reg_val));
#if 0
    res = false;
    res = sx1262_wait_on_busy(0);
    if(true == res) {
        res = true;
        res = sx1262_chip_select(true);
        res = wait_ms(2);
        res = sx1262_read_reg_proc(reg_addr, reg_val);
        res = wait_ms(2);
        res = sx1262_chip_select(false);
    } else {
        Sx1262Instance.busy_cnt++;
        res = false;
    }
#endif
    return res;
}

static bool sx1262_send_opcode_proc(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* out_rx_array,
                                    uint16_t rx_array_len) {
    bool res = false;
    if((tx_array_len + OPCODE_SIZE) < 2 * FIFO_SIZE) {
        res = true;
        /*VLA prohibited here because heap can meet stack in unpredictable time*/
        static uint8_t tempTxArray[2 * FIFO_SIZE];
        tempTxArray[0] = op_code;
        uint16_t temp_tx_arr_len = tx_array_len + OPCODE_SIZE;

        if((NULL != tx_array) && (0 < tx_array_len)) {
            memcpy(&tempTxArray[1], tx_array, temp_tx_arr_len);
        }

        res = spi_write((SpiName_t)SX1262_SPI_NUM, tempTxArray, temp_tx_arr_len) && res;
        if((0 < rx_array_len) && (NULL != out_rx_array)) {
            res = spi_read((SpiName_t)SX1262_SPI_NUM, out_rx_array, rx_array_len) && res;
        }
    }

    return res;
}

bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len) {
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_send_opcode_proc(op_code, tx_array, tx_array_len, rx_array, rx_array_len));
    return res;
}
/*
  SetRegulatorMode
  By default only the LDO is used. This is useful in low cost applications where the cost
  of the extra self needed for a DC-DC converter is prohibitive.
  Using only a linear regulator implies that the RX or TX current is almost doubled.
  This function allows to specify if DC-DC or LDO is used for power regulation.
  The regulation mode is defined by parameter regModeParam.
 * */
bool sx1262_set_regulator_mode(uint8_t reg_mode_param) {
    bool res = true;
    uint8_t tx_array[1];
    tx_array[0] = reg_mode_param;
    res = sx1262_send_opcode(OPCODE_SET_REGULATOR_MODE, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/*
   SetRfFrequency
   The command SetRfFrequency(...) is used to set the frequency of the RF frequency mode.
*/
bool sx1262_set_rf_frequency(uint32_t rf_frequency_hz, uint32_t freq_xtal_hz) {
    bool res = false;
    double freq_step = ((double)((double)freq_xtal_hz / (double)FREQ_DIV));
    LOG_INFO(LORA, "freq_step: %f", freq_step);
    uint32_t frf_code = (uint32_t)(((double)rf_frequency_hz) / freq_step);
    LOG_INFO(LORA, "rf_freq:%u freq_xtal_hz: %u frf_code: %u", rf_frequency_hz, freq_xtal_hz, frf_code);
    uint8_t tx_array[4] = {0};
    Type32Union_t u32val;
    u32val.u32 = frf_code;
    u32val.u32 = reverse_byte_order_uint32(u32val.u32);
    memcpy(tx_array, &u32val.u32, sizeof(u32val.u32));
    res = sx1262_send_opcode(OPCODE_SET_RF_FREQUENCY, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}
/*
  SetBufferBaseAddress

  This command sets the base addresses in the data buffer in all modes of operations
  for the packet handing operation in TX and RX mode. The usage and definition of those
  parameters are described in the different packet type sections.
*/
bool sx1262_set_buffer_base_addr(uint8_t tx_base_addr, uint8_t rx_base_addr) {
    bool res = false;
    uint8_t tx_array[2];
    tx_array[0] = tx_base_addr;
    tx_array[1] = rx_base_addr;
    res = sx1262_send_opcode(OPCODE_SET_BUFFER_BASE_ADDR, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}
/*
  GetRxBufferStatus

  This command returns the length of the last received packet (PayloadLengthRx)
  and the address of the first byte received (rx_start_buffer_pointer).
  It is applicable to all modems. The address is an offset relative to the first byte of the data buffer.
  */
bool sx1262_get_rxbuff_status(uint8_t* out_payload_length_rx, uint8_t* out_rx_start_buffer_pointer) {
    bool res = false;
    uint8_t rx_array[4];
    memset(rx_array, 0xFF, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_RX_BUFFER_STATUS, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[1];
        if(out_payload_length_rx) {
            *out_payload_length_rx = rx_array[2];
        }
        if(out_rx_start_buffer_pointer) {
            *out_rx_start_buffer_pointer = rx_array[3];
        }
    }
    return res;
}
/*
  SetRx
  The command SetRx() sets the device in receiver mode.
 * */
bool sx1262_start_rx(uint32_t timeout_s) {
    bool res = true;
    res = sx1262_clear_fifo() && res;
    res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;
    // SX126xHal_WriteReg( REG_RX_GAIN, (uint8_t *)0x96 ); // max LNA gain, increase current by ~2mA for around ~3dB in
    // sensivity
    uint8_t tx_array[3];
    /*from senior byte to junior byte*/
    tx_array[0] = MASK_8BIT & (timeout_s >> 16);
    tx_array[1] = MASK_8BIT & (timeout_s >> 8);
    tx_array[2] = MASK_8BIT & (timeout_s);

    res = sx1262_send_opcode(OPCODE_SET_RX, tx_array, sizeof(tx_array), NULL, 0) && res;

    return res;
}
/*
   SetTxParams
   Set output power and ramp time for the PA
   This command sets the TX output power by using the parameter power and the TX ramping
   time by using the parameter RampTime. This command is available for all protocols selected.
*/
bool sx1262_set_tx_params(int8_t power, uint8_t ramp_time) {
    bool res = false;
    uint8_t tx_array[2];
    tx_array[0] = (uint8_t)power;
    tx_array[1] = ramp_time;
    res = sx1262_send_opcode(OPCODE_SET_TX_PARAMS, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/* SetPacketParams
 * This command is used to set the parameters of the packet handling block.
 * */
bool sx1262_set_packet_params(PacketParam_t* packParam) {
    bool res = false;
    uint8_t tx_array[9];
    memset(tx_array, 0xFF, sizeof(tx_array));
    if(PACKET_TYPE_LORA == packParam->packet_type) {
        packParam->proto.lora.preamble_length = reverse_byte_order_uint16(packParam->proto.lora.preamble_length);
        memcpy(tx_array, &packParam->proto.lora.preamble_length, 2);
        tx_array[2] = packParam->proto.lora.header_type;
        tx_array[3] = packParam->proto.lora.payload_length;
        tx_array[4] = packParam->proto.lora.crc_type;
        tx_array[5] = packParam->proto.lora.invert_iq;
        res = sx1262_send_opcode(OPCODE_SET_PACKET_PARAMS, tx_array, sizeof(tx_array), NULL, 0);
    } else if(PACKET_TYPE_GFSK == packParam->packet_type) {
        res = false;
    }
    return res;
}
/*
  ResetStats
  This command resets the value read by the command GetStats.
  To execute this command, the OpCode is 0x00 followed by 6 zeros (so 7 zeros in total).
*/
bool sx1262_reset_stats(void) {
    bool res = false;
    uint8_t tx_array[6];
    memset(tx_array, 0x00, sizeof(tx_array));
    res = sx1262_send_opcode(OPCODE_RESET_STATS, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/*
  SetStandby
  cli: sxo 0x80 0x01 0
  The command SetStandby(...) is used to set the device in a configuration mode which is at an
  intermediate level of consumption. In this mode, the chip is placed in halt mode waiting for
  instructions via SPI. This mode is dedicated to chip configuration using high level commands such as
  SetPacketType(...).

  By default, after battery insertion or reset operation (pin NRESET goes low), the chip will enter in STDBY_RC mode
  running with a 13 MHz RC clock.
*/
bool sx1262_set_standby(StandbyMode_t stdby_config) {
    bool res = true;
    uint8_t tx_array[1];
    tx_array[0] = (uint8_t)stdby_config;
    res = sx1262_send_opcode(OPCODE_SET_STANDBY, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/*
  SetPacketType

  The command SetPacketType(...) sets the SX1261 radio in LoRa� or in FSK mode.
  The command SetPacketType(...) must be the first of the radio configuration sequence.
  The parameter for this command is PacketType.
*/
bool sx1262_set_packet_type(RadioPacketType_t packet_type) {
    bool res = false;
    res = sx1262_set_standby(STDBY_RC);
    if(res) {
        uint8_t tx_array[1];
        tx_array[0] = packet_type;
        res = sx1262_send_opcode(OPCODE_SET_PACKET_TYPE, tx_array, sizeof(tx_array), NULL, 0);
    }
    return res;
}
/*
  SetDioIrqParams
  This command is used to set the IRQ flag.
*/
bool sx1262_set_dio_irq_params(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask) {
    bool res = false;
    Type64Union_t u64val;
    u64val.u16[3] = reverse_byte_order_uint16(irqMask);
    u64val.u16[2] = reverse_byte_order_uint16(dio1Mask);
    u64val.u16[1] = reverse_byte_order_uint16(dio2Mask);
    u64val.u16[0] = reverse_byte_order_uint16(dio3Mask);
    res = sx1262_send_opcode(OPCODE_SET_DIO_IRQ_PARAMS, &u64val.u8[0], sizeof(Type64Union_t), NULL, 0);
    return res;
}
/*
  ClearDeviceErrors
  This commands clears all the errors recorded in the device.
  The errors can not be cleared independently.
*/
bool sx1262_clear_dev_error(void) {
    bool res = false;
    uint8_t rx_array[2];
    memset(rx_array, 0x00, sizeof(rx_array));
    uint8_t tx_array[2];
    memset(tx_array, 0x00, sizeof(tx_array));
    res = sx1262_send_opcode(OPCODE_CLEAR_DEVICE_ERRORS, tx_array, sizeof(tx_array), rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[0];
    }
    return res;
}
/*
  ClearIrqStatus 0x02
  This command clears an IRQ flag in the IRQ register.
  */
bool sx1262_clear_irq(uint16_t clear_irq_param) {
    bool res = false;
    uint16_t clear_irq_param_be;
    uint8_t tx_array[2];
    memset(tx_array, 0x00, sizeof(tx_array));
    clear_irq_param_be = reverse_byte_order_uint16(clear_irq_param);
    memcpy(tx_array, &clear_irq_param_be, sizeof(uint16_t));
    res = sx1262_send_opcode(OPCODE_CLEAR_IRQ_STATUS, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

bool sx1262_clear_fifo(void) {
    bool res = false;
    uint8_t tx_array[TX_SIZE + 1];
    memset(tx_array, 0x00, sizeof(tx_array));
    res = sx1262_send_opcode(OPCODE_WRITE_BUFFER, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

static bool is_valid_spreading_factor(SpreadingFactor_t Spreading_factor) {
    bool res = false;
    switch(Spreading_factor) {
    case SF5:
        res = true;
        break;
    case SF6:
        res = true;
        break;
    case SF7:
        res = true;
        break;
    case SF8:
        res = true;
        break;
    case SF9:
        res = true;
        break;
    case SF10:
        res = true;
        break;
    case SF11:
        res = true;
        break;
    case SF12:
        res = true;
        break;
    default:
        res = false;
        break;
    }
    return res;
}

static bool is_valid_coding_rate(LoRaCodingRate_t coding_rate) {
    bool res = false;
    switch(coding_rate) {
    case LORA_CR_4_5:
        res = true;
        break;
    case LORA_CR_4_6:
        res = true;
        break;
    case LORA_CR_4_7:
        res = true;
        break;
    case LORA_CR_4_8:
        res = true;
        break;
    default:
        res = false;
        break;
    }
    return res;
}

static bool is_valid_bandwidth(BandWidth_t bandwidth) {
    bool res = false;
    switch(bandwidth) {
    case LORA_BW_7:
        res = true;
        break;
    case LORA_BW_10:
        res = true;
        break;
    case LORA_BW_20:
        res = true;
        break;
    case LORA_BW_41:
        res = true;
        break;
    case LORA_BW_15:
        res = true;
        break;
    case LORA_BW_31:
        res = true;
        break;
    case LORA_BW_62:
        res = true;
        break;
    case LORA_BW_125:
        res = true;
        break;
    case LORA_BW_250:
        res = true;
        break;
    case LORA_BW_500:
        res = true;
        break;
    default:
        res = false;
        break;
    }
    return res;
}
/*

  The command SetModulationParams(...) is used to configure the modulation parameters of the radio.
  Depending on the packet type selected prior to calling this function, the parameters will
  be interpreted differently by the chip.
*/
bool sx1262_set_modulation_params(ModulationParams_t* modParams) {
    bool res = false;
    res = is_valid_bandwidth((BandWidth_t)modParams->band_width);
    if(res) {
        uint8_t tx_array[8];
        memset(tx_array, 0xFF, sizeof(tx_array));
        tx_array[0] = modParams->spreading_factor;
        tx_array[1] = modParams->band_width;
        tx_array[2] = modParams->coding_rate;
        tx_array[3] = 0x00; // Low Data Rate Optimization (LDRO) LDRO LowDataRateOptimize 0:OFF; 1:ON;
        res = sx1262_send_opcode(OPCODE_SET_MODULATION_PARAMS, tx_array, sizeof(tx_array), NULL, 0);
    }
    return res;
}

bool sx1262_set_sync_word(uint64_t sync_word) {
    bool res = true;
    Type64Union_t var64bit;
    var64bit.u64 = sync_word;
    var64bit.u64 = reverse_byte_order_uint64(var64bit.u64);
    res = sx1262_write_reg(SYNC_WORD_0, var64bit.u8[0]) && res;
    res = sx1262_write_reg(SYNC_WORD_1, var64bit.u8[1]) && res;
    res = sx1262_write_reg(SYNC_WORD_2, var64bit.u8[2]) && res;
    res = sx1262_write_reg(SYNC_WORD_3, var64bit.u8[3]) && res;
    res = sx1262_write_reg(SYNC_WORD_4, var64bit.u8[4]) && res;
    res = sx1262_write_reg(SYNC_WORD_5, var64bit.u8[5]) && res;
    res = sx1262_write_reg(SYNC_WORD_6, var64bit.u8[6]) && res;
    res = sx1262_write_reg(SYNC_WORD_7, var64bit.u8[7]) && res;
    return res;
}

/*
  SetSleep
  The command SetSleep(...) is used to set the device in SLEEP mode with the lowest current consumption possible. This
  command can be sent only while in STDBY mode (STDBY_RC or STDBY_XOSC). After the rising edge of NSS, all blocks are
  switched OFF except the backup regulator if needed and the blocks specified in the parameter sleepConfig.
*/
bool sx1262_set_sleep(uint8_t sleep_config) {
    bool res = false;
    uint8_t tx_array[1];
    tx_array[0] = sleep_config;
    res = sx1262_send_opcode(OPCODE_SET_SLEEP, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/* SetPaConfig
 * SetPaConfig is the command which is used to differentiate the SX1261 from the SX1262.
 * When using this command, the user selects the PA to be used by the device as well as
 * its configuration.
 */
bool sx1262_set_pa_config(uint8_t pa_duty_cycle, uint8_t hp_max, uint8_t device_sel, uint8_t pa_lut) {
    bool res = false;
    uint8_t tx_array[4];
    memset(tx_array, 0x00, sizeof(tx_array));
    tx_array[0] = pa_duty_cycle;
    tx_array[1] = hp_max;
    tx_array[2] = device_sel;
    tx_array[3] = pa_lut;
    res = sx1262_send_opcode(OPCODE_SET_PA_CONFIG, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

bool sx1262_write_buffer(uint8_t offset, uint8_t* payload, uint16_t payload_len) {
    bool res = false;
    if((NULL != payload) && (payload_len <= FIFO_SIZE)) {
        static uint8_t tx_array[FIFO_SIZE + 1];
        memset(tx_array, 0x00, sizeof(tx_array));
        tx_array[0] = offset;
        memcpy(&tx_array[1], payload, payload_len);
        res = sx1262_send_opcode(OPCODE_WRITE_BUFFER, tx_array, payload_len + 1, NULL, 0);
    }
    return res;
}

bool sx1262_set_payload(uint8_t* payload, uint8_t size) {
    bool res = false;
    res = sx1262_write_buffer(TX_BASE_ADDRESS, payload, size);
    return res;
}

bool sx1262_conf_tx(void) {
    // page 100
    // 14.3 Circuit Configuration for Basic Rx Operation
    bool res = true;
    // char payload[128];
    res = sx1262_set_pa_config(0x03, 0x05, DEV_SEL_SX1262, 0x01) && res;
    res = sx1262_set_tx_params(22, SET_RAMP_10U) && res;
    res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;

    return res;
}

bool sx1262_conf_rx(void) {
    // page 100
    // 14.3 Circuit Configuration for Basic Rx Operation
    bool res = true;
    // res = sx1262_start_rx(0xFFFFFF) && res;
    return res;
}

bool sx1262_wakeup(void) {
    uint8_t status = 0;
    bool res = true;
    res = sx1262_get_status(&status) && res;
    res = sx1262_set_standby(STDBY_RC) && res;
    return res;
}

static bool sx1262_init_gpio(void) {
    bool res = true;
    GPIO_setConfig(CONF_GPIO_LORA_RST, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONF_GPIO_LORA_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONF_GPIO_LORA_INT, GPIO_CFG_IN_NOPULL);
    GPIO_setConfig(CONF_GPIO_LORA_BSY, GPIO_CFG_IN_NOPULL);

    GPIO_writeDio(DIO_SX1262_SS, 1);
    GPIO_writeDio(DIO_SX1262_RST, 1);

    return res;
}
bool sx1262_reset(void) {
    bool res = true;
    GPIO_writeDio(DIO_SX1262_RST, 1);
    wait_ms(20);
    GPIO_writeDio(DIO_SX1262_RST, 0);
    wait_ms(50);
    GPIO_writeDio(DIO_SX1262_RST, 1);
    wait_ms(20);
    return res;
}

bool sx1262_load_params(Sx1262_t* sx1262Instance) {
    bool res = true;
    sx1262Instance->rf_frequency_hz = DFLT_FREQ_MHZ;
    sx1262Instance->mod_params.band_width = DFLT_LORA_BW;
    sx1262Instance->mod_params.coding_rate = DFLT_LORA_CR;
    sx1262Instance->mod_params.spreading_factor = DFLT_SF;
#ifdef HAS_FLASH_FS
    uint16_t file_len = 0;
    res = mm_get(PAR_ID_LORA_CR, (uint8_t*)&sx1262Instance->mod_params.coding_rate,
                 sizeof(sx1262Instance->mod_params.coding_rate), &file_len);
    if((true == res) && (1 == file_len)) {
        if(true == is_valid_coding_rate(sx1262Instance->mod_params.coding_rate)) {
            LOG_INFO(LORA, "Set coding_rate from params [%u] %s", sx1262Instance->mod_params.coding_rate,
                     coding_rate2str(sx1262Instance->mod_params.coding_rate));
        } else {
            res = false;
        }
    } else {
        res = false;
    }
    if(false == res) {
        LOG_WARNING(LORA, "Set default coding rate [%u] %s", DFLT_LORA_CR, coding_rate2str(DFLT_LORA_CR));
        sx1262Instance->mod_params.coding_rate = DFLT_LORA_CR;
        res = true;
    }

    res = mm_get(PAR_ID_LORA_BW, (uint8_t*)&sx1262Instance->mod_params.band_width,
                 sizeof(sx1262Instance->mod_params.band_width), &file_len);
    if((true == res) && (1 == file_len)) {
        if(true == is_valid_bandwidth(sx1262Instance->mod_params.band_width)) {
            LOG_INFO(LORA, "Set bandwidth from params [%u] %7.3f kHz", sx1262Instance->mod_params.band_width,
                     ((float)bandwidth2num(sx1262Instance->mod_params.band_width)) / 100.0f);
        } else {
            res = false;
        }
    } else {
        res = false;
    }
    if(false == res) {
        LOG_WARNING(LORA, "Set default bandwidth [%u] %7.3f kHz", DFLT_LORA_BW,
                    ((float)bandwidth2num(DFLT_LORA_BW)) / 100.0f);
        sx1262Instance->mod_params.band_width = DFLT_LORA_BW;
        res = true;
    }

    res = mm_get(PAR_ID_LORA_SF, (uint8_t*)&sx1262Instance->mod_params.spreading_factor,
                 sizeof(sx1262Instance->mod_params.spreading_factor), &file_len);
    if((true == res) && (1 == file_len)) {
        if(true == is_valid_spreading_factor(sx1262Instance->mod_params.spreading_factor)) {
            LOG_INFO(LORA, "Set spreading_factor from params [%u] %u Chips/Symbol",
                     sx1262Instance->mod_params.spreading_factor,
                     spreading_factor2num(sx1262Instance->mod_params.spreading_factor));
        } else {
            res = false;
        }
    } else {
        res = false;
    }

    if(false == res) {
#ifdef HAS_SX1262_DEBUG
        LOG_WARNING(LORA, "Set default spreading_factor [%u] %u Chips/Symbol", DFLT_SF, spreading_factor2num(DFLT_SF));
#endif
        sx1262Instance->mod_params.spreading_factor = DFLT_SF;
        res = true;
    }

    res = mm_get(PAR_ID_LORA_FREQ, (uint8_t*)&sx1262Instance->rf_frequency_hz, sizeof(sx1262Instance->rf_frequency_hz),
                 &file_len);
    if((true == res) && (4 == file_len)) {
#ifdef HAS_SX1262_DEBUG
        LOG_INFO(LORA, "Set rf freq from params %u Hz", sx1262Instance->rf_frequency_hz);
#endif
    } else {
#ifdef HAS_SX1262_DEBUG
        LOG_WARNING(LORA, "Set default freq %u Hz", DFLT_FREQ_MHZ);
#endif
        sx1262Instance->rf_frequency_hz = DFLT_FREQ_MHZ;
        res = true;
    }

    res = mm_get(PAR_ID_LORA_OUT_POWER, (uint8_t*)&sx1262Instance->output_power, sizeof(sx1262Instance->output_power),
                 &file_len);
    if((true == res) && (1 == file_len)) {
#ifdef HAS_SX1262_DEBUG
        LOG_INFO(LORA, "Set output power from params [%u] %d dBm", sx1262Instance->output_power,
                 sx1262Instance->output_power);
#endif
    } else {
#ifdef HAS_SX1262_DEBUG
        LOG_WARNING(LORA, "Set default output power [%u] %d dBm", DFLT_FREQ_MHZ, DFLT_FREQ_MHZ);
#endif
        sx1262Instance->output_power = DFLT_OUT_POWER;
    }

#endif /*HAS_FLASH_FS*/
    return res;
}

bool sx1262_init(void) {
    bool res = true;
    LOG_INFO(LORA, "Init SX1262");
    static uint8_t call_cnt = 0;
    if(0 == call_cnt) {
        memset(&Sx1262Instance, 0x00, sizeof(Sx1262Instance));
    }
    call_cnt = 1;
    Sx1262Instance.tx_done = true;
    res = sx1262_load_params(&Sx1262Instance) && res;
    GPIO_writeDio(DIO_SX1262_SS, 1);
    res = set_log_level(LORA, LOG_LEVEL_NOTICE);
    res = sx1262_init_gpio() && res;
    res = sx1262_reset() && res;

    res = sx1262_is_connected() && res;
    if(true == res) {
        res = sx1262_wakeup() && res;

        res = sx1262_set_packet_type(PACKET_TYPE_LORA) && res;
        res = sx1262_set_regulator_mode(REG_MODE_DC_DC_LDO) && res;

        res = sx1262_clear_fifo() && res;
        res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;

        res = sx1262_clear_dev_error() && res;

        res = sx1262_set_packet_type(PACKET_TYPE_LORA) && res;

        res = sx1262_set_standby(STDBY_XOSC);

        res = sx1262_set_rf_frequency(Sx1262Instance.rf_frequency_hz, XTAL_FREQ_HZ) && res;

        res = sx1262_set_modulation_params(&Sx1262Instance.mod_params) && res;

        Sx1262Instance.packet_param.packet_type = PACKET_TYPE_LORA;
        Sx1262Instance.packet_param.proto.lora.preamble_length = 8;
        Sx1262Instance.packet_param.proto.lora.header_type = LORA_VAR_LEN_PACT;
        Sx1262Instance.packet_param.proto.lora.payload_length = 255;
        Sx1262Instance.packet_param.proto.lora.crc_type = LORA_CRC_ON;
        Sx1262Instance.packet_param.proto.lora.invert_iq = STANDARD_IQ_SETUP;
        res = sx1262_set_packet_params(&Sx1262Instance.packet_param) && res;

        res = sx1262_conf_tx() && res;
        res = sx1262_conf_rx() && res;

        res = sx1262_set_dio_irq_params(IQR_ALL_INT, IQR_ALL_INT, IQR_ALL_INT, IQR_ALL_INT) && res;

        Sx1262Instance.set_sync_word = SYNC_WORD;
        res = sx1262_set_sync_word(Sx1262Instance.set_sync_word) && res;

        res = sx1262_start_rx(0xFFFFFF) && res;
    } else {
        LOG_ERROR(LORA, "SX1262 link error");
    }
    return res;
}

bool sx1262_set_tx(uint32_t timeout_s) {
    bool res = true;
    uint8_t buff[3];
    /*from senior byte to junior byte*/
    buff[0] = MASK_8BIT & (timeout_s >> 16);
    buff[1] = MASK_8BIT & (timeout_s >> 8);
    buff[2] = MASK_8BIT & (timeout_s);
    res = sx1262_send_opcode(OPCODE_SET_TX, buff, 3, NULL, 0) && res;
    return res;
}

bool sx1262_start_tx(uint8_t* tx_buf, uint8_t tx_buf_len, uint32_t timeout_s) {
    bool res = true;
    if((NULL != tx_buf) && (0 < tx_buf_len)) {
        /* res = sx1262_clear_fifo() && res;*/
        res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;
        res = sx1262_set_payload(tx_buf, tx_buf_len) && res;
        // res = sx1262_write_buffer(offset, tx_buf, tx_buf_len) && res;
        Sx1262Instance.tx_done = false;
        res = sx1262_set_tx(timeout_s) && res;
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_sync_word(uint64_t* sync_word) {
    bool res = true;
    if(sync_word) {
        res = true;
        Type64Union_t var64bit = {0};
        res = sx1262_read_reg(SYNC_WORD_0, &var64bit.u8[0]) && res;
        res = sx1262_read_reg(SYNC_WORD_1, &var64bit.u8[1]) && res;
        res = sx1262_read_reg(SYNC_WORD_2, &var64bit.u8[2]) && res;
        res = sx1262_read_reg(SYNC_WORD_3, &var64bit.u8[3]) && res;
        res = sx1262_read_reg(SYNC_WORD_4, &var64bit.u8[4]) && res;
        res = sx1262_read_reg(SYNC_WORD_5, &var64bit.u8[5]) && res;
        res = sx1262_read_reg(SYNC_WORD_6, &var64bit.u8[6]) && res;
        res = sx1262_read_reg(SYNC_WORD_7, &var64bit.u8[7]) && res;
        *sync_word = reverse_byte_order_uint64(var64bit.u64);
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_rand(uint32_t* rand_num) {
    bool res = true;
    if(rand_num) {
        res = true;
        Type32Union_t var32bit = {0};
        res = sx1262_read_reg(RAND_NUM_GEN_0, &var32bit.u8[0]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_1, &var32bit.u8[1]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_2, &var32bit.u8[2]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_3, &var32bit.u8[3]) && res;
        *rand_num = var32bit.u32;
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_irq_status(uint16_t* out_irq_stat) {
    bool res = false;
    GET_4_BYTE_OPCODE(OPCODE_GET_IRQ_STATUS, out_irq_stat);
    return res;
}

/*
  GetDeviceErrors
  This commands returns possible errors flag that could occur
  during different chip operation as described below.
 */
bool sx1262_get_dev_err(uint16_t* op_error) {
    bool res = false;
    GET_4_BYTE_OPCODE(OPCODE_GET_DEVICE_ERRORS, op_error);
    return res;
}
/*
  GetPacketType
  The command GetPacketType() returns the current operating packet type of the radio.
*/

bool sx1262_get_packet_type(RadioPacketType_t* packet_type) {
    bool res = false;
    uint8_t rx_array[3];
    memset(rx_array, 0x00, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_PACKET_TYPE, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[1];
        *packet_type = (RadioPacketType_t)rx_array[2];
    } else {
        *packet_type = PACKET_TYPE_NONE;
    }
    return res;
}

/*GetStatus*/
bool sx1262_get_status(uint8_t* out_status) {
    bool res = false;
    if(NULL != out_status) {
        uint8_t rx_array[2] = {0xFF, 0xFF};
        res = sx1262_send_opcode(OPCODE_GET_STATUS, NULL, 0, rx_array, sizeof(rx_array));
        *out_status = rx_array[1];
    }
    return res;
}

bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, uint8_t* RssiPkt, uint8_t* SnrPkt,
                              uint8_t* SignalRssiPkt) {
    bool res = false;
    if((NULL != RxStatus) && (NULL != RssiSync) && (NULL != RssiAvg) && (NULL != RssiPkt) && (NULL != SnrPkt) &&
       (NULL != SignalRssiPkt)) {

        uint8_t rx_array[10];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_PACKET_STATUS, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            *RxStatus = rx_array[2];
            *RssiSync = rx_array[3];
            *RssiAvg = rx_array[4];
            *RssiPkt = rx_array[7];
            *SnrPkt = rx_array[8];
            *SignalRssiPkt = rx_array[9];
        }
    }
    return res;
}

bool sx1262_get_rssi_inst(int8_t* rssi_inst) {
    bool res = false;
    if((NULL != rssi_inst)) {
        uint8_t rx_array[3];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_RSSIINST, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            *rssi_inst = -(rx_array[2] >> 1);
        }
    }
    return res;
}

bool sx1262_get_statistic(PaketStat_t* gfsk, PaketStat_t* lora) {
    bool res = false;
    if(gfsk && lora) {
        uint8_t rx_array[16];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_STATS, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            gfsk->nb_pkt_received = copy_and_rev16(&rx_array[2]);
            gfsk->nb_pkt_crc_error = copy_and_rev16(&rx_array[4]);
            gfsk->nb_pkt_length_error = copy_and_rev16(&rx_array[6]);

            lora->nb_pkt_received = copy_and_rev16(&rx_array[10]);
            lora->nb_pkt_crc_error = copy_and_rev16(&rx_array[12]);
            lora->nb_pkt_header_err = copy_and_rev16(&rx_array[14]);
        }
    }
    return res;
}

#define COM_STAT_COM_TX_DONE 0x6 /*Command TX done5 */

static bool sx1262_proc_irq_status(uint16_t irq_status) {
    bool res = false;
    if(0 < (MASK_10BIT & irq_status)) {
        Sx1262Instance.irq_cnt.total += (uint16_t)count_set_bits((uint32_t)MASK_10BIT & irq_status);
        res = true;
    }

    if((1 << IRQ_BIT_TXDONE) == (irq_status & (1 << IRQ_BIT_TXDONE))) {
        Sx1262Instance.irq_cnt.tx_done++;
        res = sx1262_clear_irq(1 << IRQ_BIT_TXDONE);
    }
    if((1 << IRQ_BIT_RXDONE) == (irq_status & (1 << IRQ_BIT_RXDONE))) {
        Sx1262Instance.irq_cnt.rx_done++;
        res = sx1262_clear_irq(1 << IRQ_BIT_RXDONE);
    }
    if((1 << IRQ_BIT_PREAMBLEDETECTED) == (irq_status & (1 << IRQ_BIT_PREAMBLEDETECTED))) {
        Sx1262Instance.irq_cnt.preamble_detected++;
        res = sx1262_clear_irq(1 << IRQ_BIT_PREAMBLEDETECTED);
    }
    if((1 << IRQ_BIT_SYNCWORDVALID) == (irq_status & (1 << IRQ_BIT_SYNCWORDVALID))) {
        Sx1262Instance.irq_cnt.syncword_valid++;
        res = sx1262_clear_irq(1 << IRQ_BIT_SYNCWORDVALID);
    }
    if((1 << IRQ_BIT_HEADERVALID) == (irq_status & (1 << IRQ_BIT_HEADERVALID))) {
        Sx1262Instance.irq_cnt.header_valid++;
        res = sx1262_clear_irq(1 << IRQ_BIT_HEADERVALID);
    }
    if((1 << IRQ_BIT_CRCERR) == (irq_status & (1 << IRQ_BIT_CRCERR))) {
        Sx1262Instance.irq_cnt.crc_err++;
        res = sx1262_clear_irq(1 << IRQ_BIT_CRCERR);
    }
    if((1 << IRQ_BIT_CADDONE) == (irq_status & (1 << IRQ_BIT_CADDONE))) {
        Sx1262Instance.irq_cnt.cad_done++;
        res = sx1262_clear_irq(1 << IRQ_BIT_CADDONE);
    }
    if((1 << IRQ_BIT_CADDETECTED) == (irq_status & (1 << IRQ_BIT_CADDETECTED))) {
        Sx1262Instance.irq_cnt.cad_detected++;
        res = sx1262_clear_irq(1 << IRQ_BIT_CADDETECTED);
    }
    if((1 << IRQ_BIT_TIMEOUT) == (irq_status & (1 << IRQ_BIT_TIMEOUT))) {
        Sx1262Instance.irq_cnt.timeout++;
        res = sx1262_clear_irq(1 << IRQ_BIT_TIMEOUT);
    }
    return res;
}

/*
  ReadBuffer
  cli: sxrf offset len
  This function allows reading (n-3) bytes of payload received starting at offset.
  Note that the NOP must be sent after sending
  the offset.
 */
bool sx1262_read_buffer(int16_t offset, uint8_t* out_payload, uint16_t payload_len) {
    bool res = false;

    if((out_payload) && (payload_len <= FIFO_SIZE) && (0 <= offset) && (offset <= (FIFO_SIZE - 1))) {
        static uint8_t rx_array[FIFO_SIZE + 3];
        memset(rx_array, 0xFF, sizeof(rx_array));

        uint8_t tx_array[1];
        tx_array[0] = (uint8_t)offset;
        res = sx1262_send_opcode(OPCODE_READ_BUFFER, tx_array, 1, rx_array, payload_len + 3);
        if(res) {
            memcpy(out_payload, &rx_array[3], payload_len);
        }
    }
    return res;
}

bool sx1262_get_rx_payload(uint8_t* out_payload, uint8_t* out_size, uint16_t max_size) {
    bool res = false;
    uint8_t rx_payload_len = 0;
    uint8_t rx_start_buffer_pointer = 0;
    res = sx1262_get_rxbuff_status(&rx_payload_len, &rx_start_buffer_pointer);
    if(rx_payload_len <= max_size) {
#ifdef HAS_SX1262_DEBUG_RX
        LOG_INFO(LORA, "rx_len %u start %u", rx_payload_len, rx_start_buffer_pointer);
#endif
        res = sx1262_read_buffer(rx_start_buffer_pointer - 1, out_payload, (uint16_t)rx_payload_len);
        *out_size = rx_payload_len;
    } else {
        *out_size = 0;
    }
    return res;
}

static bool sx1262_proc_chip_mode(ChipMode_t chip_mode) {
    bool res = false;
    static ChipMode_t prev_chip_mode = CHP_MODE_UNDEF;
    static uint32_t chip_mode_rc = 0;
    static uint32_t chip_mode_xosc = 0;
    static uint32_t chip_mode_fs = 0;
    static uint32_t chip_mode_tx = 0;

    switch(chip_mode) {
    case CHP_MODE_STBY_RC: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_rc++;
            res = false;
        } else {
            chip_mode_rc = 0;
        }
        if(50 < chip_mode_rc) {
            chip_mode_rc = 0;
            LOG_WARNING(LORA, "Hang on in STBY_RC");
            res = sx1262_init();
        }
    } break;
    case CHP_MODE_STBY_XOSC: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_xosc++;
            res = false;
        } else {
            chip_mode_xosc = 0;
        }
        if(100 < chip_mode_xosc) {
            chip_mode_xosc = 0;
            LOG_WARNING(LORA, "Hang on in STBY_XOSC");
            res = sx1262_init();
        }
    } break;
    case CHP_MODE_FS: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_fs++;
            res = false;
        } else {
            chip_mode_fs = 0;
        }
        if(100 < chip_mode_fs) {
            chip_mode_fs = 0;
            LOG_WARNING(LORA, "Hang on in FS");
            res = sx1262_init();
        }
    } break;
    case CHP_MODE_RX:
        res = true;
        break;
    case CHP_MODE_TX: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_tx++;
            res = false;
        } else {
            chip_mode_tx = 0;
        }
        if(1000 < chip_mode_tx) {
            chip_mode_tx = 0;
            LOG_WARNING(LORA, "Hang on in TX");
            res = sx1262_init();
        }
    } break;

    default:
        res = false;
        break;
    }
    prev_chip_mode = chip_mode;
    return res;
}

/* poll sx1262 registers. Move data from transceiver REG to MCU RAM.
 * verify transceiver and notify user if needed
 * */
bool sx1262_process(void) {
    bool res = false;
    Sx1262_t tempSx1262Instance = {0};
    memset(&tempSx1262Instance, 0x00, sizeof(tempSx1262Instance));

    if(BUSY_CNT_LIMIT < Sx1262Instance.busy_cnt) {
        Sx1262Instance.busy_cnt = 0;
        res = sx1262_init();
    }

    uint32_t cur_time_stamp_ms = get_time_ms32();
    uint32_t tx_time_diff_ms = cur_time_stamp_ms - Sx1262Instance.tx_done_time_stamp_ms;
    if((DFLT_TX_PAUSE_MS < tx_time_diff_ms) && (true == Sx1262Instance.tx_done)) {
        Array_t txNode = {.size = 0, .pArr = NULL};
        res = fifo_arr_pull(&LoRaInterface.FiFoLoRaTx, &txNode);
        if((true == res)) {
            if((0 < txNode.size) && (txNode.pArr)) {
                res = sx1262_start_tx(txNode.pArr, txNode.size, 0);
                if(res) {
                    LoRaInterface.tx_ok_cnt++;
                }
            }
            if(txNode.pArr) {
                free(txNode.pArr);
            }
        }
    }

    res = sx1262_get_status(&tempSx1262Instance.dev_status);
    if(res) {
        res = true;
        Sx1262Instance.dev_status = tempSx1262Instance.dev_status;

        Sx1262Instance.com_stat = extract_subval_from_8bit(tempSx1262Instance.dev_status, 3, 1);
        uint8_t rx_payload[RX_SIZE] = {0};
        memset(rx_payload, 0x00, sizeof(rx_payload));
        uint8_t rx_size = 0;
        switch(Sx1262Instance.com_stat) {
        case COM_STAT_DATA_AVAIL: {
            Sx1262Instance.data_aval_cnt++;
#ifdef HAS_SX1262_DEBUG_RX
            LOG_INFO(LORA, "Data available!");
#endif /*HAS_SX1262_DEBUG_RX*/
            res = sx1262_get_rx_payload(rx_payload, &rx_size, RX_SIZE);
            if(res) {
#ifdef HAS_SX1262_DEBUG_RX
                res = print_mem(rx_payload, rx_size, false, true);
#endif /*HAS_SX1262_DEBUG_RX*/
                res = lora_proc_payload(rx_payload, rx_size);
            }
        } break;
        case COM_STAT_COM_TIMEOUT:
            LOG_WARNING(LORA, "time out");
            res = false;
            break;
        case COM_STAT_COM_PROC_ERR:
            /*Too frequent call*/
            // LOG_ERROR(LORA,"Error");
            res = false;
            break;
        case COM_STAT_EXE_ERR:
            LOG_ERROR(LORA, "Failure to execute command"); /**/
            res = false;
            break;
        case COM_STAT_COM_TX_DONE:
#ifdef HAS_SX1262_DEBUG_TX
            LOG_INFO(LORA, "TX done");
#endif
            Sx1262Instance.tx_done_time_stamp_ms = get_time_ms32();
            Sx1262Instance.tx_done = true;
            LoRaInterface.tx_done_cnt++;
            res = sx1262_start_rx(0xFFFFFF);
            break;
        default:
            res = false;
            break;
        }
        Sx1262Instance.chip_mode = (ChipMode_t)extract_subval_from_8bit(tempSx1262Instance.dev_status, 6, 4);

        res = sx1262_proc_chip_mode(Sx1262Instance.chip_mode);

        res = sx1262_reset_stats();
    }

    tempSx1262Instance.rx_payload_len = 0;
    tempSx1262Instance.rx_buffer_pointer = 0;
    res = sx1262_get_rxbuff_status(&tempSx1262Instance.rx_payload_len, &tempSx1262Instance.rx_buffer_pointer);
    if(res) {
        Sx1262Instance.rx_payload_len = tempSx1262Instance.rx_payload_len;
        Sx1262Instance.rx_buffer_pointer = tempSx1262Instance.rx_buffer_pointer;
    }
#ifdef HAS_SX1262_POLL

    tempSx1262Instance.rx_status = 0;
    tempSx1262Instance.rssi_sync = 0;
    tempSx1262Instance.rssi_avg = 0;
    tempSx1262Instance.rssi_pkt = 0;
    tempSx1262Instance.snr_pkt = 0;
    tempSx1262Instance.signal_rssi_pkt = 0;
    res = sx1262_get_packet_status(&tempSx1262Instance.rx_status, &tempSx1262Instance.rssi_sync,
                                   &tempSx1262Instance.rssi_avg, &tempSx1262Instance.rssi_pkt,
                                   &tempSx1262Instance.snr_pkt, &tempSx1262Instance.signal_rssi_pkt);
    if(res) {
        Sx1262Instance.rx_status = tempSx1262Instance.rx_status;
        Sx1262Instance.rssi_sync = tempSx1262Instance.rssi_sync;
        Sx1262Instance.rssi_avg = tempSx1262Instance.rssi_avg;
        Sx1262Instance.rssi_pkt = tempSx1262Instance.rssi_pkt;
        Sx1262Instance.snr_pkt = tempSx1262Instance.snr_pkt;
        Sx1262Instance.signal_rssi_pkt = tempSx1262Instance.signal_rssi_pkt;
    }

    tempSx1262Instance.rssi_inst = 0;
    res = sx1262_get_rssi_inst(&tempSx1262Instance.rssi_inst);
    if(res) {
        Sx1262Instance.rssi_inst = tempSx1262Instance.rssi_inst;
    }

    memset(&tempSx1262Instance.gfsk, 0x0, sizeof(tempSx1262Instance.gfsk));
    memset(&tempSx1262Instance.lora, 0x0, sizeof(tempSx1262Instance.lora));
    res = sx1262_get_statistic(&tempSx1262Instance.gfsk, &tempSx1262Instance.lora);
    if(res) {
        Sx1262Instance.gfsk = tempSx1262Instance.gfsk;
        Sx1262Instance.lora = tempSx1262Instance.lora;
    }

    tempSx1262Instance.op_error = 0;
    res = sx1262_get_dev_err(&tempSx1262Instance.op_error);
    if(res) {
        // proc_dev_err(op_error);
        Sx1262Instance.op_error = tempSx1262Instance.op_error;
    }
#endif /*HAS_SX1262_POLL*/

    tempSx1262Instance.irq_stat = 0;
    res = sx1262_get_irq_status(&tempSx1262Instance.irq_stat);
    if(res) {
        Sx1262Instance.irq_stat = tempSx1262Instance.irq_stat;
        sx1262_proc_irq_status(tempSx1262Instance.irq_stat);
    }

    tempSx1262Instance.get_sync_word = 0;
    res = sx1262_get_sync_word(&tempSx1262Instance.get_sync_word);
    if(res) {
        Sx1262Instance.get_sync_word = tempSx1262Instance.get_sync_word;
    }

    Sx1262Instance.wire_int = (uint8_t)GPIO_readDio(DIO_SX1262_INT);
    Sx1262Instance.wire_busy = (uint8_t)GPIO_readDio(DIO_SX1262_BUSY);
    Sx1262Instance.wire_nss = (uint8_t)GPIO_readDio(DIO_SX1262_SS);
    Sx1262Instance.wire_rst = (uint8_t)GPIO_readDio(DIO_SX1262_RST);

    tempSx1262Instance.packet_type = PACKET_TYPE_UNDEF;
    res = sx1262_get_packet_type(&tempSx1262Instance.packet_type);
    if(res) {
        Sx1262Instance.packet_type = tempSx1262Instance.packet_type;
    }
    tempSx1262Instance.rand_num = 0;
    res = sx1262_get_rand(&tempSx1262Instance.rand_num);
    if(res) {
        Sx1262Instance.rand_num = tempSx1262Instance.rand_num;
    }

    return res;
}
