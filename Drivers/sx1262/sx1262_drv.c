/*

CPOL= 0 and CPHA = 0 in Motorola/Freescale nomenclature.
Bytes follow from senior byte to junior byte (Motorola byte order or network byte order)
speed up to 16 MHz
*/
#include "sx1262_drv.h"

#include <gpio.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAS_FREE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#endif /*HAS_FREE_RTOS*/

#include "array.h"
#include "bit_utils.h"
#include "board_layout.h"
#include "byte_utils.h"
#include "clocks.h"
#ifdef HAS_LED
#include "led_drv.h"
#endif
#include "crc8_sae_j1850.h"
#include "data_utils.h"
#include "debug_info.h"
#include "float_utils.h"
#include "gpio_drv.h"
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif
#ifdef HAS_LORA
#include "lora_drv.h"
#endif
#include "none_blocking_pause.h"
#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif
#include "spi_drv.h"
#include "sx1262_diag.h"
#include "sx1262_registers.h"
#include "sys_config.h"
#include "task_info.h"
#ifndef HAS_SPI
#error "SX1262 requires SPI driver"
#endif

#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif


#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif

#ifdef HAS_PARAM
#include "param_ids.h"
#endif

#ifdef HAS_LEGAL_BAND_CHECK
#include "legal_band.h"
#endif

#ifdef HAS_SX1262_DEBUG
#include "sx1262_diag.h"
#endif


#define WAIT_SX1262_MUTEX 1000
bool sx1262_chip_select(bool state) {
    bool res = true;
#ifdef HAS_FREE_RTOS
    if(Sx1262Instance.mutex){
        if(true==state){
            if( pdTRUE == xSemaphoreTake( Sx1262Instance.mutex, WAIT_SX1262_MUTEX ) ){
                res = true;
            }else{
                res = false;
                LOG_ERROR(LORA, "MutexBusy");
            }
        }
    }else{
        res = false;
        LOG_ERROR(LORA, "MutexInitError");
    }
#endif

    if((true == state) && (true==res)) {
#ifdef HAS_SPI_SW_CHIP_SELECT
        gpio_set_state(DIO_SX1262_SS, 0);
#ifdef HAS_CAN
        gpio_set_state(DIO_CAN_SS, 1);
#endif
#endif /*HAS_SPI_SW_CHIP_SELECT*/
        res = true;
    } else if(false == state) {
#ifdef HAS_SPI_SW_CHIP_SELECT
        gpio_set_state(DIO_SX1262_SS, 1);
#ifdef HAS_CAN
        gpio_set_state(DIO_CAN_SS, 1);
#endif
#endif /*HAS_SPI_SW_CHIP_SELECT*/
        res = true;
    } else {
        res = false;
    }

#ifdef HAS_FREE_RTOS
    if(Sx1262Instance.mutex){
        if(false==state){
            xSemaphoreGive(Sx1262Instance.mutex);
            res = true;
        }
    }else{
        res = false;
        LOG_ERROR(LORA, "MutexInitError");
    }
#endif
    return res;
}

Sx1262_t Sx1262Instance = {0};

#ifdef HAS_SX1262_DEBUG
const xSx1262Reg_t RegMap[] = {
    {0x0580, "DIOxOutputEnable"},
    {0x0583, "DIOxInputEnable"},
    {0x0584, "DIOxPullUpControl"},
    {0x0585, "DIOxPullDownControl"},
    {0x06B8, "WhiteningInitValMSB"},
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
    {0x0736, "IQpolaritySetup"},
    {0x0740, "LoRaSyncWordMSB"},
    {0x0741, "LoRaSyncWordLSB"},
    {RAND_NUM_GEN_0, "RandNumGen0"},
    {RAND_NUM_GEN_1, "RandNumGen1"},
    {RAND_NUM_GEN_2, "RandNumGen2"},
    {RAND_NUM_GEN_3, "RandNumGen3"},
    {0x0889, "TxModulation"},
    {0x08AC, "RxGain"},
    {0x08D8, "TxClampConfig"},
    {0x08E7, "OCPConfig"},
    {0x0902, "RTCcontrol"},
    {0x0911, "XTAtrim"},
    {0x0912, "XTBtrim"},
    {0x920, "DIO3outputVoltageControl"},
    {0x944, "EventMask"},
};
#endif /*HAS_SX1262_DEBUG*/

bool sx1262_wait_on_busy(uint32_t time_out_ms) {
    uint32_t start_ms = 0U;
    uint32_t curr_ms = 0U;
    uint32_t diff_ms = 0U;
    bool res = false;
    bool loop = true;
    uint32_t busy_value = 0;
    start_ms = get_time_ms32();
    while(loop) {
        busy_value = gpio_read(DIO_SX1262_BUSY);
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
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "WaitBusyErr");
#endif
    }
    return res;
}

bool sx1262_is_connected(void) {
    bool res = false;
    uint64_t read_sync_word = 0;

    RadioPacketType_t packet_type = PACKET_TYPE_UNDEF;
    res = sx1262_get_packet_type(&packet_type, &Sx1262Instance.dev_status.byte);
    if(res) {
        if(packet_type != PACKET_TYPE_UNDEF) {
            res = true;
        } else {
#ifdef HAS_LOG
            LOG_ERROR(LORA, "PaketTypeErr");
#endif
        }
    }

    res = sx1262_get_sync_word(&read_sync_word);
    if((Sx1262Instance.set_sync_word == read_sync_word) && (true == res)) {
#ifdef HAS_LOG
        LOG_PARN(LORA, "SyncWordMatch");
#endif
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "SyncWordMisMatch 0x%llx!=0x%llx", Sx1262Instance.set_sync_word, read_sync_word);
#endif
        res = false;
    }

    return res;
}

static bool check_sync_word(uint64_t sync_word) {
    bool res = false;
    uint64_t read_sync_word = 0;
    res = sx1262_set_sync_word(sync_word);
    if(res) {
        res = sx1262_get_sync_word(&read_sync_word);
        if(res) {
            if(sync_word == read_sync_word) {
#ifdef HAS_LOG
                LOG_INFO(LORA, "Sx1262 Connected!");
#endif
                res = true;
            } else {
#ifdef HAS_LOG
                LOG_ERROR(LORA, "Set:0x%llx Read:0x%llx", sync_word, read_sync_word);
#endif
                res = false;
            }
        }
    }else{
#ifdef HAS_LOG
       LOG_ERROR(LORA, "Set:0x%llx Err", sync_word);
#endif
    }
    return res;
}

bool sx1262_is_exist(void) {
    bool res = false;
    res = check_sync_word(0x0012345678abcdef);
    if(true == res) {
#ifdef HAS_LOG
        LOG_INFO(LORA, "ChipExist");
#endif
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "ChipLack");
#endif
        res = false;
    }
    return res;
}

static bool sx1262_send_opcode_proc(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* out_rx_array,
                                    uint16_t rx_array_len) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s() OpCode:0x%x %s", __FUNCTION__, op_code, OpCode2Str(op_code));
#endif
    bool res = false;
    if((tx_array_len + OPCODE_SIZE) < (2 * FIFO_SIZE)) {
        res = true;
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
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "OpCodeProckError 0x%02x", op_code);
#endif
    }

    return res;
}

/*SendOpCode*/
bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "SendOpCode 0x%02x %s", op_code, OpCode2Str(op_code));
#endif
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_send_opcode_proc(op_code, tx_array, tx_array_len, rx_array, rx_array_len));
#ifdef HAS_LOG
    if(false == res) {
        LOG_ERROR(LORA, "OpCodeError: 0x%02x %s", op_code, OpCode2Str(op_code));
    }
#endif
    return res;
}


/*SetRxTxFallbackMode*/


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
#ifdef HAS_LOG
    LOG_INFO(LORA, "freq_step: %f", freq_step);
#endif
    uint32_t frf_code = (uint32_t)(((double)rf_frequency_hz) / freq_step);
#ifdef HAS_LOG
    LOG_INFO(LORA, "rf_freq:%u freq_xtal_hz: %u frf_code: %u", rf_frequency_hz, freq_xtal_hz, frf_code);
#endif
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
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s()", __FUNCTION__);
#endif
    bool res = false;
    uint8_t tx_array[2] = {0};
    tx_array[0] = tx_base_addr;
    tx_array[1] = rx_base_addr;
    res = sx1262_send_opcode(OPCODE_SET_BUFFER_BASE_ADDR, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

static uint8_t get_prev_circular(uint8_t addr, uint8_t inter){
    uint8_t val=addr;
    uint8_t i = 0;
    for(i = 0; i < 2; i++) {
        if(0 < val) {
            val -= 1; /*subtraction matters*/
        } else {
            val = 255;
        }
    }
    return val;
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
#if 1
            *out_rx_start_buffer_pointer= get_prev_circular(*out_rx_start_buffer_pointer,2);
#endif
        }
    }
    return res;
}
/*
  SetRx
  The command SetRx() sets the device in receiver mode.
 * */
bool sx1262_start_rx(uint32_t timeout_s) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s()", __FUNCTION__);
#endif
    bool res = true;
    res = sx1262_clear_fifo() && res;
    res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;
    res = sx1262_set_rx_gain(RXGAIN_BOOSTED) && res;
    uint8_t tx_array[3];
    /*from senior byte to junior byte*/
    tx_array[0] = MASK_8BIT & (timeout_s >> 16);
    tx_array[1] = MASK_8BIT & (timeout_s >> 8);
    tx_array[2] = MASK_8BIT & (timeout_s);

    res = sx1262_send_opcode(OPCODE_SET_RX, tx_array, sizeof(tx_array), NULL, 0) && res;
#ifdef HAS_LOG
    LOG_DEBUG(LORA, "StartRx");
#endif
    return res;
}

bool is_power_valid(int8_t power) {
    bool res = false;
    if((-7 <= power) && (power <= 22)) {
        res = true;
    }
    return res;
}
/*
   SetTxParams
   Set output power and ramp time for the PA
   This command sets the TX output power by using the parameter power and the TX ramping
   time by using the parameter RampTime. This command is available for all protocols selected.
*/
bool sx1262_set_tx_params(int8_t power, uint8_t ramp_time) {
#ifdef HAS_LOG
    LOG_DEBUG(LORA, "SetTxParams");
#endif
    bool res = false;
    res = is_power_valid(power);
    if(false == res) {
        power = DFLT_OUT_POWER;
    }
    uint8_t tx_array[2] = {0};
    tx_array[0] = (uint8_t)power;
    tx_array[1] = ramp_time;
    res = sx1262_send_opcode(OPCODE_SET_TX_PARAMS, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/*SetDIO2AsRfSwitchCtrl*/
bool sx1262_set_dio2_as_rf_switch_ctrl(Dio2Mode_t mode) {
    bool res = false;
    uint8_t tx_array[1] = {0};
    tx_array[0] = (uint8_t)mode;
    res = sx1262_send_opcode(OPCODE_SET_DIO2_AS_RFSWITCH, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}
/* SetPacketParams
 * This command is used to set the parameters of the packet handling block.
 * */
bool sx1262_set_packet_params(PacketParam_t* packParam) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s()", __FUNCTION__);
#endif
    bool res = false;
    uint8_t tx_array[9];
    memset(tx_array, 0xFF, sizeof(tx_array));
    if(PACKET_TYPE_LORA == packParam->packet_type) {
        uint16_t preamble_length_be = reverse_byte_order_uint16(packParam->proto.lora.preamble_length);
        memcpy(tx_array, &preamble_length_be, 2);
        tx_array[2] = packParam->proto.lora.header_type;
        tx_array[3] = packParam->proto.lora.payload_length;
        tx_array[4] = packParam->proto.lora.crc_type;
        tx_array[5] = packParam->proto.lora.invert_iq;
        res = sx1262_send_opcode(OPCODE_SET_PACKET_PARAMS, tx_array, 6, NULL, 0);
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
    uint8_t tx_array[6] = {0};
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
#ifdef HAS_LOG
    LOG_DEBUG(LORA, "SetStandby");
#endif
    bool res = true;
    uint8_t tx_array[1];
    tx_array[0] = (uint8_t)stdby_config;
    res = sx1262_send_opcode(OPCODE_SET_STANDBY, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

/*
  SetPacketType

  The command SetPacketType(...) sets the SX1261 radio in LoRa® or in FSK mode.
  The command SetPacketType(...) must be the first of the radio configuration sequence.
  The parameter for this command is PacketType.
*/
bool sx1262_set_packet_type(RadioPacketType_t packet_type) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s()", __FUNCTION__);
#endif
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
    uint8_t tx_array = 0x00;
    res = sx1262_send_opcode(OPCODE_CLEAR_DEVICE_ERRORS, &tx_array, 1, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[0];
    }
    return res;
}
/*
  ClearIrqStatus 0x02
  sxo 0x02 0xFFFF 0
  This command clears an IRQ flag in the IRQ register.
  */
bool sx1262_clear_irq(uint16_t clear_irq_param) {
    bool res = false;
    uint16_t clear_irq_param_be;
    uint8_t tx_array[2];
    memset(tx_array, 0xFF, sizeof(tx_array));
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

bool is_valid_spreading_factor(SpreadingFactor_t Spreading_factor) {
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

/*SetCadParams*/
bool sx1262_set_cad_params(CadNunSym_t cad_symbol_num, uint8_t cad_det_peak, /*See App Application note AN1200.48 */
                           uint8_t cad_det_min,                              /*See App Application note AN1200.48 */
                           CadExtMode_t cad_exit_mode, uint16_t cad_timeout) {
    bool res = false;
    uint8_t tx_array[7]; /**/
    memset(tx_array, 0x00, sizeof(tx_array));
    tx_array[0] = (uint8_t)cad_symbol_num;
    tx_array[1] = (uint8_t)cad_det_peak;
    tx_array[2] = (uint8_t)cad_det_min;
    tx_array[3] = (uint8_t)cad_exit_mode;
    memcpy(&tx_array[4], &cad_timeout, 2);
    res = sx1262_send_opcode(OPCODE_SET_CAD_PARAM, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

bool sx1262_set_cad(void) {
    bool res = false;
    res = sx1262_send_opcode(OPCODE_SET_CAD, NULL, 0, NULL, 0);
    return res;
}

bool is_valid_coding_rate(LoRaCodingRate_t coding_rate) {
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

bool is_valid_bandwidth(BandWidth_t bandwidth) {
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
    bool res = false, res1 = false, res2 = false, res3 = false;
    res1 = is_valid_bandwidth(modParams->band_width);
    if(false == res1) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "BandWidthError %u", modParams->band_width);
#endif
    }
    res2 = is_valid_coding_rate(modParams->coding_rate);
    if(false == res2) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "CodingRateError %u", modParams->coding_rate);
#endif
    }
    res3 = is_valid_spreading_factor(modParams->spreading_factor);
    if(false == res3) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "SpreadingFactorError %u", modParams->spreading_factor);
#endif
    }
    if(res1 && res2 && res3) {
        uint8_t tx_array[8]; /**/
        memset(tx_array, 0x00, sizeof(tx_array));
        tx_array[0] = modParams->spreading_factor;
        tx_array[1] = modParams->band_width;
        tx_array[2] = modParams->coding_rate;
        tx_array[3] = modParams->low_data_rate_optimization; // Low Data Rate Optimization (LDRO) LDRO
                                                             // LowDataRateOptimize 0:OFF; 1:ON; TODO
        res = sx1262_send_opcode(OPCODE_SET_MODULATION_PARAMS, tx_array, sizeof(tx_array), NULL, 0);
    }
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
#ifdef HAS_LOG
    LOG_DEBUG(LORA, "SetPaConfig");
#endif
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
/*WriteBuffer*/
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

static bool calc_power_param(uint8_t output_power_dbm, uint8_t* pa_duty_cycle, uint8_t* hp_max) {
    bool res = false;
    switch(output_power_dbm) {
    case 22:
        *pa_duty_cycle = 0x04;
        *hp_max = 0x07;
        res = true;
        break;
    case 20:
        *pa_duty_cycle = 0x03;
        *hp_max = 0x05;
        res = true;
        break;
    case 17:
        *pa_duty_cycle = 0x02;
        *hp_max = 0x03;
        res = true;
        break;
    case 14:
        *pa_duty_cycle = 0x02;
        *hp_max = 0x01;// was 2
        res = true;
        break;
    default:
        *pa_duty_cycle = 0x04;
        *hp_max = 0x07;
        res = false;
        break;
    }
    return res;
}

static bool sx1262_conf_tx(int8_t output_power_dbm) {
#ifdef HAS_LOG
    LOG_PARN(LORA, "%s()", __FUNCTION__);
#endif
    // page 100
    // 14.3 Circuit Configuration for Basic Rx Operation
    bool res = true;
    res = is_power_valid(output_power_dbm);
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "InvaOutPwr: %d dBm %6.3f W", output_power_dbm, dbm2watts(output_power_dbm));
#endif
        output_power_dbm = DFLT_OUT_POWER;
    }

    uint8_t pa_duty_cycle = 0;
    uint8_t hp_max = 0;
    res = calc_power_param(output_power_dbm, &pa_duty_cycle, &hp_max);
    if(res) {
#ifdef HAS_LOG
        LOG_INFO(LORA, "OutPwr: %d dBm %6.3f W", output_power_dbm, dbm2watts(output_power_dbm));
#endif
    } else {
#ifdef HAS_LOG
        LOG_WARNING(LORA, "DfltOutPwr: %d dBm %6.3f W", DFLT_OUT_POWER, dbm2watts(DFLT_OUT_POWER));
#endif
    }
    res = sx1262_set_pa_config(pa_duty_cycle, hp_max, DEV_SEL_SX1262, 0x01) && res;
    res = sx1262_set_tx_params(output_power_dbm, SET_RAMP_3400US) && res;
    res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;

    return res;
}
#if 0
bool sx1262_conf_rx(void) {
    // page 100
    // 14.3 Circuit Configuration for Basic Rx Operation
    bool res = true;
    // res = sx1262_start_rx(0xFFFFFF) && res;
    return res;
}
#endif

bool sx1262_wakeup(void) {
    uint8_t status = 0;
    bool res = true;
    res = sx1262_get_status(&status) && res;
    res = sx1262_set_standby(STDBY_RC) && res;
    return res;
}

static bool sx1262_init_gpio(void) {
    bool res = true;
#ifdef CC26X2
    GPIO_setConfig(CONF_GPIO_LORA_RST, gpio_get_cfg_dio(DIO_SX1262_RST));
    GPIO_setConfig(CONF_GPIO_LORA_CS, gpio_get_cfg_dio(DIO_SX1262_SS));
    GPIO_setConfig(CONF_GPIO_LORA_BSY, gpio_get_cfg_dio(DIO_SX1262_BUSY));
    GPIO_setConfig(CONF_GPIO_LORA_INT, gpio_get_cfg_dio(DIO_SX1262_INT));

    GPIO_setCallback(CONF_GPIO_LORA_INT, dio28_fall_call_back);

    GPIO_enableInt(CONF_GPIO_LORA_INT);
#endif

    gpio_set_state(DIO_SX1262_SS, 1);
    gpio_set_state(DIO_SX1262_RST, 1);
    return res;
}

bool sx1262_reset(void) {
    bool res = true;
    gpio_set_state(DIO_SX1262_RST, 1);
    wait_ms(20);
    gpio_set_state(DIO_SX1262_RST, 0);
    wait_ms(50);
    gpio_set_state(DIO_SX1262_RST, 1);
    wait_ms(20);
    return res;
}

bool sx1262_int_isr(Sx1262_t* sx1262Instance) {
    bool res = false;
    if(sx1262Instance) {
        res = true;
        sx1262Instance->int_cnt++;
    }

    return res;
}

float dbm2watts(int32_t dbm) {
    float watts = 0.0f;
    watts = powf(10.0f, ((float)dbm) / 10.0f) / 1000.0f;
    return watts;
}
#if 0
#ifdef HAS_LOG
#define LOAD_PARAM(PAR_ID, VARIABLE, EXP_LEN, VAR_NAME, DEF_VAL, PARSER_FUNC)                                          \
    do {                                                                                                               \
        uint16_t file_len = 0;                                                                                         \
        res = mm_get(PAR_ID, (uint8_t*)&VARIABLE, sizeof(VARIABLE), &file_len);                                        \
        if((true == res) && ((EXP_LEN) == file_len)) {                                                                 \
            LOG_INFO(LORA, "Set" VAR_NAME "FromParams %u [%s]", VARIABLE, PARSER_FUNC(VARIABLE));                      \
        } else {                                                                                                       \
            LOG_WARNING(LORA, "SetDflt" VAR_NAME " %u [%s]", VARIABLE, PARSER_FUNC(DEF_VAL));                          \
            VARIABLE = DEF_VAL;                                                                                        \
            res = false;                                                                                               \
            out_res = false;                                                                                           \
        }                                                                                                              \
    } while(0);
#else
#define LOAD_PARAM(PAR_ID, VARIABLE, EXP_LEN, VAR_NAME, DEF_VAL, PARSER_FUNC)                                          \
    do {                                                                                                               \
        uint16_t file_len = 0;                                                                                         \
        res = mm_get(PAR_ID, (uint8_t*)&VARIABLE, sizeof(VARIABLE), &file_len);                                        \
        if(!((true == res) && ((EXP_LEN) == file_len))) {                                                              \
            VARIABLE = DEF_VAL;                                                                                        \
            res = false;                                                                                               \
            out_res = false;                                                                                           \
        }                                                                                                              \
    } while(0);
#endif
#endif

static bool sx1262_load_params(Sx1262_t* sx1262Instance) {
    bool out_res = true;
    sx1262Instance->lora_sync_word_set = DFLT_LORA_SYNC_WORD;
    sx1262Instance->crc_init = 0x1D0F;
    sx1262Instance->crc_poly = 0x1021;
    sx1262Instance->mod_params.band_width = DFLT_LORA_BW;
    sx1262Instance->mod_params.coding_rate = DFLT_LORA_CR;
    sx1262Instance->mod_params.spreading_factor = DFLT_SF;
    sx1262Instance->mod_params.low_data_rate_optimization = LDRO_OFF;
    sx1262Instance->packet_param.packet_type = PACKET_TYPE_LORA;
    sx1262Instance->packet_param.proto.lora.header_type = LORA_VAR_LEN_PACT;
    sx1262Instance->packet_param.proto.lora.crc_type = LORA_CRC_ON;
    sx1262Instance->packet_param.proto.lora.invert_iq = IQ_SETUP_STANDARD;
    sx1262Instance->packet_param.proto.lora.preamble_length = DFLT_PREAMBLE_LEN;
    sx1262Instance->packet_param.proto.lora.payload_length = 255;
    sx1262Instance->output_power = DFLT_OUT_POWER;
    sx1262Instance->rf_frequency_hz = DFLT_FREQ_MHZ;
    sx1262Instance->set_sync_word = DFLT_SYNC_WORD;
#ifdef HAS_SX1262_BIT_RATE
    sx1262Instance->tx_max_bit_rate = 0.0;
#endif

#ifdef HAS_FLASH_FS
    bool res = true;
    LOAD_PARAM(LORA, PAR_ID_LORA_CRC_INIT, sx1262Instance->crc_init, 2, "CrcInit", 0x1D0F, HexWord2Str);
    LOAD_PARAM(LORA, PAR_ID_LORA_CRC_POLY, sx1262Instance->crc_poly, 2, "CRCPoly", 0x1021, HexWord2Str);

    LOAD_PARAM(LORA, PAR_ID_LOW_DATA_RATE, sx1262Instance->mod_params.low_data_rate_optimization, 1, "LowDataRateOpt",
               LDRO_OFF, LowDataRateOpt2Str);
    LOAD_PARAM(LORA, PAR_ID_PAYLOAD_LENGTH, sx1262Instance->packet_param.proto.lora.payload_length, 1, "PayLen", 255,
               PayloadLen2Str);
    LOAD_PARAM(LORA, PAR_ID_PACKET_TYPE, sx1262Instance->packet_param.packet_type, 1, "PktType", PACKET_TYPE_LORA,
               PacketType2Str);
    LOAD_PARAM(LORA, PAR_ID_HEADER_TYPE, sx1262Instance->packet_param.proto.lora.header_type, 1, "HeaderType",
               LORA_VAR_LEN_PACT, LoraHeaderType2Str);
    LOAD_PARAM(LORA, PAR_ID_CRC_TYPE, sx1262Instance->packet_param.proto.lora.crc_type, 1, "CrcType", LORA_CRC_ON,
               LoraCrcType2Str);
    LOAD_PARAM(LORA, PAR_ID_PREAMBLE_LENGTH, sx1262Instance->packet_param.proto.lora.preamble_length, 2, "PreamLen",
               DFLT_PREAMBLE_LEN, PreambleLen2Str);
    LOAD_PARAM(LORA, PAR_ID_LORA_SYNC_WORD, sx1262Instance->lora_sync_word_set, 2, "LoRaSyncWord", DFLT_LORA_SYNC_WORD,
               HexWord2Str);
    LOAD_PARAM(LORA, PAR_ID_IQ_SETUP, sx1262Instance->packet_param.proto.lora.invert_iq, 1, "IQSetUp",
               IQ_SETUP_STANDARD, IqSetUp2Str);
    LOAD_PARAM(LORA, PAR_ID_LORA_CR, sx1262Instance->mod_params.coding_rate, 1, "CodingRate", DFLT_LORA_CR,
               coding_rate2str);
    LOAD_PARAM(LORA, PAR_ID_LORA_BW, sx1262Instance->mod_params.band_width, 1, "BandWidth", DFLT_LORA_BW,
               bandwidth2str);
    LOAD_PARAM(LORA, PAR_ID_LORA_SF, sx1262Instance->mod_params.spreading_factor, 1, "SpreadingFactor", DFLT_SF,
               spreading_factor2str);
    LOAD_PARAM(LORA, PAR_ID_LORA_FREQ, sx1262Instance->rf_frequency_hz, 4, "RfFreq", DFLT_FREQ_MHZ, RfFreq2Str);
    LOAD_PARAM(LORA, PAR_ID_LORA_OUT_POWER, sx1262Instance->output_power, 1, "OutputPwr", DFLT_OUT_POWER, dbm2wattsStr);
    // LOAD_PARAM(PAR_ID_SYNC_WORD, sx1262Instance->set_sync_word, 8, "SyncWord" ,DFLT_SYNC_WORD, SyncWord2Str);
    uint16_t file_len = 0;
    res = mm_get(PAR_ID_SYNC_WORD, (uint8_t*)&sx1262Instance->set_sync_word, sizeof(sx1262Instance->set_sync_word),
                 &file_len);
    if((true == res) && ((8) == file_len)) {
#ifdef HAS_LOG
        LOG_INFO(LORA, "SetSyncWordFromParams %llu [%s]", sx1262Instance->set_sync_word,
                 SyncWord2Str(sx1262Instance->set_sync_word));
#endif
    } else {
#ifdef HAS_LOG
        LOG_WARNING(LORA, "SetDfltSyncWord %llu [%s]", sx1262Instance->set_sync_word, SyncWord2Str(DFLT_SYNC_WORD));
#endif
        sx1262Instance->set_sync_word = DFLT_SYNC_WORD;
        res = false;
        out_res = false;
    }

#ifdef HAS_SX1262_BIT_RATE
    // LOAD_PARAM(PAR_ID_LORA_MAX_BIT_RATE, sx1262Instance->tx_max_bit_rate, 8, "BitRate" ,0.0, BitRate2Str);

    res = mm_get(PAR_ID_LORA_MAX_BIT_RATE, (uint8_t*)&sx1262Instance->tx_max_bit_rate,
                 sizeof(sx1262Instance->tx_max_bit_rate), &file_len);
    if((true == res) && (8 == file_len)) {
#ifdef HAS_LOG
        LOG_INFO(LORA, "SetBitRateFromParams [%s]", BitRate2Str(sx1262Instance->tx_max_bit_rate));
#endif
    } else {
#ifdef HAS_LOG
        LOG_WARNING(LORA, "SetDfltBitRate [%s]", BitRate2Str(0.0));
#endif
        sx1262Instance->tx_max_bit_rate = 0.0;
        out_res = false;
    }
#endif /*HAS_SX1262_BIT_RATE*/
#endif /*HAS_FLASH_FS*/
    return out_res;
}

/*SetTx*/
static bool sx1262_set_tx(uint32_t timeout_s) {
    bool res = true;
    uint8_t buff[3];
    /*from senior byte to junior byte*/
    buff[0] = MASK_8BIT & (timeout_s >> 16);
    buff[1] = MASK_8BIT & (timeout_s >> 8);
    buff[2] = MASK_8BIT & (timeout_s);
    res = sx1262_send_opcode(OPCODE_SET_TX, buff, 3, NULL, 0) && res;
    return res;
}

bool sx1262_start_tx(uint8_t* tx_buf, uint8_t tx_len, uint32_t timeout_s) {
    bool res = true;
    if(false == Sx1262Instance.tx_mute) {
        if(Sx1262Instance.tx_done) {
            Sx1262Instance.tx_done = false;
            if((NULL != tx_buf) && (0 < tx_len) && (tx_len < SX1262_MAX_FRAME_SIZE)) {
                // res = sx1262_clear_fifo();
                res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;
                res = sx1262_set_payload(tx_buf, tx_len) && res;
                Sx1262Instance.packet_param.proto.lora.payload_length =tx_len;
                res = sx1262_set_packet_params(&Sx1262Instance.packet_param);
#ifdef HAS_LOG
                LOG_DEBUG(LORA, "TxLen:%u", tx_len);
                if(Sx1262Instance.debug) {
                    print_mem(tx_buf, tx_len, true, false, true, true);
                }
#endif
            } else {
                LOG_ERROR(LORA, "TxLenErr:%u Max%u", tx_len, SX1262_MAX_FRAME_SIZE);
                res = false;
            }
            if(res) {
                // res = sx1262_write_buffer(offset, tx_buf, tx_len) && res;
#ifdef HAS_SX1262_BIT_RATE
                Sx1262Instance.tx_last_size = tx_len;
                Sx1262Instance.tx_start_time_stamp_ms = get_time_ms32();
#endif          /*HAS_SX1262_BIT_RATE*/
                /*TODO: Set Red Led on*/
#ifdef LED_INDEX_RED
                led_on(&Led[LED_INDEX_RED]);
#endif
                res = sx1262_set_tx(timeout_s);
                if(res) {
                    Sx1262Instance.tx_size_max = max8u(Sx1262Instance.tx_size_max, tx_len);
                }
            }
        } else {
#ifdef HAS_LOG
            LOG_ERROR(LORA, "TxBusy");
#endif
            res = false;
        }
    } else {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "TxMute");
#endif
        res = false;
    }
    return res;
}

/*GetIrqStatus
 * sxo 0x12 0 3
 * sxo 0x12 0x000000 3*/
bool sx1262_get_irq_status(Sx1262IRQs_t* out_irq_stat) {
    bool res = false;
    uint8_t rx_array[4];
    memset(rx_array, 0xFF, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_IRQ_STATUS, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        uint16_t irq_stat = 0;
        Sx1262Instance.status = rx_array[1];/*Diag Status*/
        memcpy(&irq_stat, &rx_array[2], 2);
        // irq_stat = reverse_byte_order_uint16(irq_stat);
        out_irq_stat->word = irq_stat;
    }
    return res;
}

/*
  GetDeviceErrors
  This commands returns possible errors flag that could occur
  during different chip operation as described below.
 */
bool sx1262_get_dev_err(uint16_t* op_error) {
    bool res = false;
    uint8_t rx_array[4];
    memset(rx_array, 0x00, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_DEVICE_ERRORS, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[1];
        *op_error = copy_and_rev16(&rx_array[2]);
    } else {
        *op_error = 0x0000;
    }

    return res;
}
/*
  GetPacketType
  The command GetPacketType() returns the current operating packet type of the radio.
*/
bool sx1262_get_packet_type(RadioPacketType_t* const packet_type, uint8_t* status) {
    bool res = false;

    uint8_t rx_array[2];
    memset(rx_array, 0xFF, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_PACKET_TYPE, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        *status = rx_array[0];
        *packet_type = (RadioPacketType_t)rx_array[1];
    } else {
        *packet_type = PACKET_TYPE_NONE;
    }
    return res;
}

/*GetStatus (page 95 in datasheet)
sxo 0xC0 0 2      2a2a
sxo 0xC0 0 1
sxo 0xC0 0x00 1   2a
sxo 0xC0 0x00 2
sxgs
*/
bool sx1262_get_status(uint8_t* out_status) {
    bool res = false;
    if(NULL != out_status) {
#ifdef CC26X2
        uint8_t rx_array[2] = {0xFF, 0xFF};
        uint8_t tx_array = 0x00;
        res = sx1262_send_opcode(OPCODE_GET_STATUS, &tx_array, 1, rx_array, sizeof(rx_array));
        *out_status = rx_array[1];
#endif
#ifdef ESP32
        uint8_t rx_array[2] = {0xFF, 0xFF};
        uint8_t tx_array = 0x00;
        res = sx1262_send_opcode(OPCODE_GET_STATUS, &tx_array, 1, rx_array, sizeof(rx_array));
        *out_status = rx_array[1];

        //uint8_t rx_array = 0x00;
        //res = sx1262_send_opcode(OPCODE_GET_STATUS, NULL, 0, &rx_array, 1);
        //*out_status = rx_array;
#endif

#ifdef USE_HAL_DRIVER
        uint8_t rx_array[1] = {0x00};
        uint8_t tx_array = 0xFF;
        res = sx1262_send_opcode(OPCODE_GET_STATUS, NULL, 0, &rx_array[0], sizeof(rx_array));
        *out_status = rx_array[0];
#endif
    }
    return res;
}

/*
 GetPacketStatus
 * */
bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, int8_t* RssiPkt, uint8_t* SnrPkt,
                              int8_t* SignalRssiPkt) {
    bool res = false;
    if((NULL != RxStatus) && (NULL != RssiSync) && (NULL != RssiAvg) && (NULL != RssiPkt) && (NULL != SnrPkt) &&
       (NULL != SignalRssiPkt)) {

        uint8_t rx_array[5];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_PACKET_STATUS, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            int8_t tRssiPkt = -rx_array[2] / 2;
            uint8_t tSnrPkt = rx_array[3] / 4;
            int8_t tSignalRssiPkt = -rx_array[4] / 2;
            *RssiPkt = tRssiPkt;
            *SnrPkt = tSnrPkt;
            *SignalRssiPkt = tSignalRssiPkt;
        }
    }
    return res;
}

/*GetRssiInst
  Returns the instantaneous measured RSSI while in Rx mode
  */
bool sx1262_get_rssi_inst(int8_t* out_rssi_inst) {
    bool res = false;
    int8_t rssi_inst = 0;
    uint8_t rx_array[3];
    memset(rx_array, 0xFF, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_RSSIINST, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[1];
        rssi_inst = -(rx_array[2] >> 1); /*Signal power in dBm*/
        if(NULL != out_rssi_inst) {
            *out_rssi_inst = rssi_inst;
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
            /*
            gfsk->nb_pkt_received = copy_and_rev16(&rx_array[1]);
            gfsk->nb_pkt_crc_error = copy_and_rev16(&rx_array[3]);
            gfsk->nb_pkt_length_error = copy_and_rev16(&rx_array[5]);
            */
            lora->nb_pkt_received = copy_and_rev16(&rx_array[2]);
            lora->nb_pkt_crc_error = copy_and_rev16(&rx_array[4]);
            lora->nb_pkt_header_err = copy_and_rev16(&rx_array[6]);
        }
    }
    return res;
}

static bool sx1262_proc_irq_status(Sx1262IRQs_t* irq_status) {
    bool res = false;
    // Sx1262IRQs_t irq;
    // (void) irq;
    // irq.word = 0;
    if(0 < (MASK_10BIT & irq_status->word)) {
        Sx1262Instance.irq_cnt.total += (uint16_t)count_set_bits((uint32_t)MASK_10BIT & irq_status->word);
        res = true;
    }

    if(irq_status->TxDone) {
        Sx1262Instance.irq_cnt.tx_done++;
        //  irq.TxDone =1;
    }
    if(irq_status->RxDone) {
        Sx1262Instance.irq_cnt.rx_done++;
        //  irq.RxDone = 1;
    }
    if(irq_status->PreambleDetected) {
        Sx1262Instance.irq_cnt.preamble_detected++;
        //  irq.PreambleDetected=1;
    }
    if(irq_status->SyncWordValid) {
        Sx1262Instance.irq_cnt.syncword_valid++;
        //  irq.SyncWordValid=1;
    }
    if(irq_status->HeaderValid) {
        Sx1262Instance.irq_cnt.header_valid++;
        // irq.HeaderValid=1;
    }
    if(irq_status->CrcErr) {
        Sx1262Instance.irq_cnt.crc_err++;
        //  irq.CrcErr=1;
    }
    if(irq_status->CadDone) {
        Sx1262Instance.irq_cnt.cad_done++;
        //  irq.CadDone =1;
    }
    if(irq_status->CadDetected) {
        Sx1262Instance.irq_cnt.cad_detected++;
        //  irq.CadDetected=1;
    }
    if(irq_status->Timeout) {
        Sx1262Instance.irq_cnt.timeout++;
        //  irq.Timeout=1;
    }
    res = sx1262_clear_irq(0xFFFF);
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

bool sx1262_get_rx_payload(uint8_t* out_payload, uint16_t* out_size, uint16_t max_size, uint8_t* crc8) {
    bool res = false;
    uint16_t rx_payload_len = 0;
    uint8_t len = 0;
    uint8_t calc_crc8 = 0;
    uint8_t rx_start_buffer_pointer = 0;
    res = sx1262_get_rxbuff_status(&len, &rx_start_buffer_pointer);
#ifdef HAS_LOG
    if(res) {
        LOG_DEBUG(LORA, "Start %u rxLen %u", rx_start_buffer_pointer, len);
    }
#endif
    //rx_payload_len = 256; // One LoRa Frame one-multiple TBFP frame
    rx_payload_len = len; // Just for stream of bytes
    if(rx_payload_len <= max_size) {
        res = sx1262_read_buffer(rx_start_buffer_pointer, out_payload, (uint16_t)rx_payload_len);
        *out_size = rx_payload_len;
        calc_crc8 = crc8_sae_j1850_calc(out_payload, rx_payload_len);
        *crc8 = calc_crc8;
    } else {
        *out_size = 0;
    }
    return res;
}

static bool sx1262_proc_chip_mode(ChipMode_t chip_mode) {
    bool res = false;
#ifdef HAS_LOG
    LOG_PARN(LORA, "ChipMode %u %s", chip_mode, chip_mode2str(chip_mode));
#endif
    static ChipMode_t prev_chip_mode = CHP_MODE_UNDEF;
    static uint32_t chip_mode_rc = 0;
    static uint32_t chip_mode_xosc = 0;
    static uint32_t chip_mode_fs = 0;
    static uint32_t chip_mode_tx = 0;
    if(chip_mode != prev_chip_mode) {
#ifdef HAS_LOG
        LOG_DEBUG(LORA, "ChipMode:%u %s", chip_mode, chip_mode2str(chip_mode));
#endif
    }

    switch(chip_mode) {
    case CHP_MODE_STBY_RC: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_rc++;
            res = false;
        } else {
            chip_mode_rc = 0;
        }
        if(40 < chip_mode_rc) {
            chip_mode_rc = 0;
#ifdef HAS_LOG
            LOG_WARNING(LORA, "Hang on in STBY_RC");
#endif
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
#ifdef HAS_LOG
            LOG_WARNING(LORA, "Hang on in STBY_XOSC");
#endif
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
        if(100000 < chip_mode_fs) {
            chip_mode_fs = 0;
#ifdef HAS_LOG
            LOG_WARNING(LORA, "Hang on in FS");
#endif
            res = sx1262_init();
        }
    } break;
    case CHP_MODE_RX: {
        int8_t rssi_inst = 0;
        Sx1262Instance.tx_done=true;
        res = sx1262_get_rssi_inst(&rssi_inst);
        if(res) {
            Sx1262Instance.rssi_inst = rssi_inst;
        }
    } break;
    case CHP_MODE_TX: {
        if(prev_chip_mode == chip_mode) {
            chip_mode_tx++;
            res = false;
        } else {
            chip_mode_tx = 0;
        }
        if(1000 < chip_mode_tx) {
            chip_mode_tx = 0;
#ifdef HAS_LOG
            LOG_WARNING(LORA, "Hang on in TX");
#endif
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

#ifdef HAS_SX1262_BIT_RATE
static bool sx1262_calc_bit_rate(uint32_t bytes, float* tx_real_bit_rate, uint32_t* tx_duration_ms) {
    bool res = false;
    float bit_rate = 0.0f;
    uint32_t duration_ms = 0;
    uint32_t tx_done_time_stamp_ms = get_time_ms32();
    duration_ms = tx_done_time_stamp_ms - Sx1262Instance.tx_start_time_stamp_ms;
    bit_rate = ((float)(bytes * 8 * 1000)) / (((float)duration_ms));
#ifdef HAS_FLASH_FS
    uint16_t file_len = 0;
    res = mm_get(PAR_ID_LORA_MAX_BIT_RATE, (uint8_t*)&Sx1262Instance.tx_max_bit_rate,
                 sizeof(Sx1262Instance.tx_max_bit_rate), &file_len);
#endif
    if(Sx1262Instance.tx_max_bit_rate < bit_rate) {
        Sx1262Instance.tx_max_bit_rate = bit_rate;
#ifdef HAS_FLASH_FS
        res = mm_set(PAR_ID_LORA_MAX_BIT_RATE, (uint8_t*)&Sx1262Instance.tx_max_bit_rate,
                     sizeof(Sx1262Instance.tx_max_bit_rate));
        if(false == res) {
#ifdef HAS_LOG
            LOG_ERROR(LORA, "SaveMaxLoRaBitRateErr");
#endif
        }
#endif
    }
    if(tx_real_bit_rate && tx_duration_ms) {
        *tx_real_bit_rate = bit_rate;
        *tx_duration_ms = duration_ms;
    }
    return res;
}
#endif /*HAS_SX1262_BIT_RATE*/

static bool sx1262_sync_rssi(void) {
    bool res = false;
    uint8_t rx_status = 0;
    uint8_t rssi_sync = 0;
    uint8_t rssi_avg = 0;
    int8_t rssi_pkt = 0;
    uint8_t snr_pkt = 0;
    int8_t signal_rssi_pkt = 0;
    res = sx1262_get_packet_status(&rx_status, &rssi_sync, &rssi_avg, &rssi_pkt, &snr_pkt, &signal_rssi_pkt);
    if(res) {
        Sx1262Instance.rx_status = rx_status;
        Sx1262Instance.rssi_sync = rssi_sync;
        Sx1262Instance.rssi_avg = rssi_avg;
        Sx1262Instance.rssi_pkt = rssi_pkt;
        Sx1262Instance.snr_pkt = snr_pkt;
        Sx1262Instance.signal_rssi_pkt = signal_rssi_pkt;
    }
    return res;
}

static inline bool sx1262_proc_data_aval(void) {
    bool res = false;

    uint16_t rx_size = 0;
    uint8_t rx_payload[RX_SIZE + 1] = {0};
    memset(rx_payload, 0x00, sizeof(rx_payload));
    Sx1262Instance.rx_done_cnt++;

    rx_size = 0;
    uint8_t cur_crc8 = 0;
    res = sx1262_get_rx_payload(rx_payload, &rx_size, RX_SIZE, &cur_crc8);
    if(res) {
        res = is_arr_pat(rx_payload, rx_size, 0x00);
        if(false == res) {
            res = sx1262_clear_fifo();

            Sx1262Instance.rx_size_max = max8u(Sx1262Instance.rx_size_max, rx_size);

            if(Sx1262Instance.debug) {
#ifdef HAS_LOG
                LOG_INFO(LORA, "rx %u byte", rx_size);
                res = print_mem(rx_payload, rx_size, Sx1262Instance.show_bin, Sx1262Instance.show_ascii, true,
                                Sx1262Instance.is_packet);
#endif
            }
#ifdef HAS_LORA
            res = lora_proc_payload(rx_payload, rx_size);
#endif /*HAS_LORA*/
            // led_blink(&Led[LED_INDEX_RED], 10);
        }

    } else {
#ifdef HAS_LOG
        LOG_INFO(LORA, "DataReadErr");
#endif
    }
    return res;
}

static inline bool sx1262_poll_status(void) {
    bool res = false;
#ifdef HAS_LOG
    LOG_PARN(LORA, "PollStatus");
#endif
    Sx1262_t tempSx1262Instance = {0};
    memset(&tempSx1262Instance, 0x00, sizeof(tempSx1262Instance));

    //RadioPacketType_t packet_type = PACKET_TYPE_UNDEF;
    // res = sx1262_get_packet_type(&packet_type, &tempSx1262Instance.dev_status.byte);
    res = sx1262_get_status(&tempSx1262Instance.dev_status.byte);
    if(res) {
#ifdef HAS_LOG
        LOG_PARN(LORA, "Status 0x%02x CmdStat:%s ChipMode:%s", tempSx1262Instance.dev_status.byte,
                  cmd_stat2str(tempSx1262Instance.dev_status.command_status),
                  chip_mode2str(tempSx1262Instance.dev_status.chip_mode));
#endif
        res = true;
        static uint8_t stat_byte_prev = 0;
        Sx1262Instance.dev_status.byte = tempSx1262Instance.dev_status.byte;
        if(Sx1262Instance.dev_status.byte != stat_byte_prev) {
#ifdef HAS_LOG
            LOG_DEBUG(LORA, "Status:0x%02x Mode:%s Cmd:%s",
                      Sx1262Instance.dev_status.byte,
                      chip_mode2str(Sx1262Instance.dev_status.chip_mode),
                      cmd_stat2str(Sx1262Instance.dev_status.command_status)); /**/
#endif
        }
        stat_byte_prev = Sx1262Instance.dev_status.byte;
        // Sx1262Instance.com_stat = extract_subval_from_8bit(tempSx1262Instance.dev_status, 3, 1);
        switch(Sx1262Instance.dev_status.command_status) {
        case COM_STAT_DATA_AVAIL: {
            res = sx1262_proc_data_aval();
            res = sx1262_start_rx(0xFFFFFF);
        } break;
        case COM_STAT_COM_TIMEOUT:
#ifdef HAS_LOG
            LOG_WARNING(LORA, "time out");
#endif
            res = false;
            break;
        case COM_STAT_COM_PROC_ERR:
            /*Too frequent call*/
            // LOG_ERROR(LORA,"Error");
            Sx1262Instance.processing_error++;
            res = false;
            break;
        case COM_STAT_EXE_ERR:
#ifdef HAS_LOG
            LOG_ERROR(LORA, "FailureToExecuteCommand %u", Sx1262Instance.dev_status.command_status); /**/
#endif
            res = false;
            break;
        case COM_STAT_COM_TX_DONE: {
            Sx1262Instance.tx_done = true;
#ifdef LED_INDEX_RED
            led_off(&Led[LED_INDEX_RED]);
#endif
#ifdef HAS_SX1262_BIT_RATE
            float tx_real_bit_rate = 0.0;
            uint32_t tx_duration_ms = 0;
            sx1262_calc_bit_rate(Sx1262Instance.tx_last_size, &tx_real_bit_rate, &tx_duration_ms);
#endif /*HAS_SX1262_BIT_RATE*/

#ifdef HAS_SX1262_BIT_RATE
#ifdef HAS_LOG
            LOG_DEBUG(LORA, "TxDone %7.1f bit/s=%7.1f byte/s duration: %u ms for %u bytes", tx_real_bit_rate,
                      tx_real_bit_rate / 8, tx_duration_ms, Sx1262Instance.tx_last_size);
#endif
#else
#ifdef HAS_LOG
            LOG_DEBUG(LORA, "TxDone");
#endif
#endif /*HAS_SX1262_BIT_RATE*/

            Sx1262Instance.tx_done_cnt++;
            LoRaInterface.tx_done_cnt++;
            res = sx1262_start_rx(0xFFFFFF);
        } break;
        default:
            res = false;
            break;
        }
        // Sx1262Instance.chip_mode = (ChipMode_t)extract_subval_from_8bit(tempSx1262Instance.dev_status, 6, 4);

        res = sx1262_proc_chip_mode((ChipMode_t)Sx1262Instance.dev_status.chip_mode);

        res = sx1262_reset_stats();
    }
    if(Sx1262Instance.sync_rssi) {
        sx1262_sync_rssi();
    }
    return res;
}

static inline bool sx1262_sync_registers(void) {
    bool res = false;
    Sx1262_t tempSx1262Instance = {0};
    memset(&tempSx1262Instance, 0x00, sizeof(tempSx1262Instance));

    tempSx1262Instance.rx_payload_len = 0;
    tempSx1262Instance.rx_buffer_pointer = 0;
    res = sx1262_get_rxbuff_status(&tempSx1262Instance.rx_payload_len, &tempSx1262Instance.rx_buffer_pointer);
    if(res) {
        Sx1262Instance.rx_payload_len = tempSx1262Instance.rx_payload_len;
        Sx1262Instance.rx_buffer_pointer = tempSx1262Instance.rx_buffer_pointer;
    }
#ifdef HAS_SX1262_POLL

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

    tempSx1262Instance.irq_stat.word = 0;
    res = sx1262_get_irq_status(&tempSx1262Instance.irq_stat);
    if(res) {
        Sx1262Instance.irq_stat = tempSx1262Instance.irq_stat;
        sx1262_proc_irq_status(&tempSx1262Instance.irq_stat);
        // Sx1262Instance.irq_stat.word = 0;
    }

    tempSx1262Instance.get_sync_word = 0;
    res = sx1262_get_sync_word(&tempSx1262Instance.get_sync_word);
    if(res) {
        Sx1262Instance.get_sync_word = tempSx1262Instance.get_sync_word;
    }

    Sx1262Instance.wire_int = (uint8_t)gpio_read(DIO_SX1262_INT);
    Sx1262Instance.wire_busy = (uint8_t)gpio_read(DIO_SX1262_BUSY);
    Sx1262Instance.wire_nss = (uint8_t)gpio_read(DIO_SX1262_SS);
    Sx1262Instance.wire_rst = (uint8_t)gpio_read(DIO_SX1262_RST);

    tempSx1262Instance.packet_type = PACKET_TYPE_UNDEF;
    res = sx1262_get_packet_type(&tempSx1262Instance.packet_type, &Sx1262Instance.dev_status.byte);
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


#ifdef HAS_TBFP
static bool sx1262_transmit_from_queue(Sx1262_t* instance) {
    bool res = false;

    if((DFLT_TX_PAUSE_MS < tx_time_diff_ms) &&
       (true == instance->tx_done) &&
       (SX1262_MAX_PAYLOAD_SIZE < fifo_get_count(&LoRaInterface.FiFoLoRaCharTx))) {
        uint8_t TxPayload[SX1262_MAX_PAYLOAD_SIZE] = {0};
        memset(TxPayload, 0, sizeof(TxPayload));

        fifo_index_t tx_len = 0;
        res = fifo_pull_array(&LoRaInterface.FiFoLoRaCharTx, (char*)TxPayload, sizeof(TxPayload), &tx_len);
        if(res) {
#ifdef HAS_LOG
            LOG_DEBUG(LORA, "FiFoPullErr Len:%u", tx_len);
#endif
        }
            /*Transmitt multiple RTCM3 packages in single LoRa frame*/
            fifo_index_t tx_len = 0;
            res = fifo_pull_array(&LoRaInterface.FiFoLoRaCharTx, (char*)tx_buf, sizeof(tx_buf), &tx_len);
            if(res) {
                if(0<tx_len){
    #ifdef HAS_LOG
                    LOG_DEBUG(LORA, "RxFiFoPull Len:%u", tx_len);
    #endif
                }else{
                    LOG_PARN(LORA, "TxFiFoEmpty");
                }
            }else{
                LOG_ERROR(LORA, "FiFoPullErr Len:%u", tx_len);
            }

    #ifdef HAS_LORA_FIFO_ARRAYS
            Array_t Node = {.size = 0, .pArr = NULL};
            res = fifo_arr_pull(&LoRaInterface.FiFoLoRaTx, &Node);
            if(res) {
                if(Node.pArr) {
                    if(0 < Node.size) {
                        if(Node.size <= sizeof(tx_buf)) {

                            memcpy(tx_buf, Node.pArr, Node.size);
                            tx_len = Node.size;
                        }
                    }
    #ifdef HAS_MCU
                    free(Node.pArr);
    #endif /*HAS_MCU*/
                }
            }
        }
#endif /*HAS_LORA_FIFO_ARRAYS*/
        if(res) {
            if((0 < tx_len) && (tx_len <= sizeof(TxPayload))) {
               res = tbfp_send_tunnel(TxPayload, tx_len, IF_SX1262);
               if(res) {
                   LoRaInterface.tx_ok_cnt++;
               } else {
                   LoRaInterface.tx_err_cnt++;
               }
            }
        }
    }else{
        LOG_PARN(LORA, "FiFoCnt:%u", count);
        res = true;
    }
    return res;
}
#endif /*HAS_TBFP*/

/* poll sx1262 registers. Move data from transceiver REG to MCU RAM.
 * verify transceiver and notify user if needed
 * */
bool sx1262_process(void) {
    bool res = false;
#ifdef HAS_LOG
    LOG_PARN(LORA, "CheckConnectivity=%u", Sx1262Instance.check_connectivity);
#endif

    if(Sx1262Instance.check_connectivity) {
        res = sx1262_is_connected();
    } else {
        res = true;
    }
    if(res) {
        if(BUSY_CNT_LIMIT < Sx1262Instance.busy_cnt) {
            Sx1262Instance.busy_cnt = 0;
            res = sx1262_init();
        }
#ifdef HAS_TBFP
        res = sx1262_transmit_from_queue(&Sx1262Instance);
#endif

        res = sx1262_poll_status();
        if(Sx1262Instance.sync_reg) {
            res = sx1262_sync_registers();
        }

    } else {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "Sx1262_SpiLinkLost");
#endif
        res = sx1262_init();
    }

    return res;
}

#ifdef HAS_FREE_RTOS
static void sx1262_thread(void* arg) {
    while(1) {
        if(Sx1262Instance.proc) {
            sx1262_process();
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
#endif /*HAS_FREE_RTOS*/

/*returns band_width in kHz multiplied by 100 in order to fit in 2 bytes*/
uint32_t bandwidth2num(BandWidth_t bandwidth) {
    uint32_t band_width = 0;
    switch(bandwidth) {
    case LORA_BW_7:
        band_width = 7810;
        break;
    case LORA_BW_10:
        band_width = 10420;
        break;
    case LORA_BW_15:
        band_width = 15630;
        break;
    case LORA_BW_20:
        band_width = 20830;
        break;
    case LORA_BW_31:
        band_width = 31250;
        break;
    case LORA_BW_41:
        band_width = 41670;
        break;
    case LORA_BW_62:
        band_width = 62500;
        break;
    case LORA_BW_125:
        band_width = 125000;
        break;
    case LORA_BW_250:
        band_width = 250000;
        break;
    case LORA_BW_500:
        band_width = 500000;
        break;

    default:
        band_width = 0;
        break;
    }
    return band_width;
}

bool sx1262_init(void) {
    bool res = true;
    static uint8_t call_cnt = 0;
    if(0 == call_cnt) {
        memset(&Sx1262Instance, 0x00, sizeof(Sx1262Instance));
    }
#ifdef HAS_FREE_RTOS
    Sx1262Instance.mutex = xSemaphoreCreateMutexStatic(&Sx1262Instance.xMutexBuffer);
    if(NULL == Sx1262Instance.mutex) {
        res = false;
        LOG_ERROR(LORA, "MutexInitError");
    } else {
        LOG_INFO(LORA, "MutexInitOk");
    }
#endif

#ifdef ESP32
    Sx1262Instance.proc = false;
    res = set_log_level(LORA, LOG_LEVEL_DEBUG);
    Sx1262Instance.debug = true;
    Sx1262Instance.show_ascii = true;
    Sx1262Instance.check_connectivity = false;
#else
#ifdef HAS_LOG
    res = set_log_level(LORA, LOG_LEVEL_INFO);
#endif
    Sx1262Instance.check_connectivity = true;
    Sx1262Instance.debug = false;
    Sx1262Instance.show_bin = false;
#endif

#ifdef HAS_LOG
    LOG_INFO(LORA, "InitSX1262...");
#endif
    Sx1262Instance.tx_mute = false;
    Sx1262Instance.sync_rssi = true;
    call_cnt = 1;
    Sx1262Instance.tx_done = true;
    res = sx1262_load_params(&Sx1262Instance);
    if(false == res) {
#ifdef HAS_LOG
        LOG_WARNING(LORA, "LackOfParam");
#endif
    }

    Sx1262Instance.bit_rate =
        lora_calc_data_rate(Sx1262Instance.mod_params.spreading_factor, Sx1262Instance.mod_params.band_width,
                            Sx1262Instance.mod_params.coding_rate);
#ifdef HAS_LOG
    LOG_INFO(LORA, "data rate %f bit/s %f byte/s", Sx1262Instance.bit_rate, Sx1262Instance.bit_rate / 8);
#endif

#ifdef HAS_LEGAL_BAND_CHECK
    uint32_t bandwidth_hz = bandwidth2num(Sx1262Instance.mod_params.band_width);
    res = is_band_legal(Sx1262Instance.rf_frequency_hz, bandwidth_hz);
    if(false == res) {
#ifdef HAS_LOG
        LOG_WARNING(LORA, "illegal frequencies %u...%u Hz", Sx1262Instance.rf_frequency_hz - bandwidth_hz / 2,
                    Sx1262Instance.rf_frequency_hz + bandwidth_hz / 2);
#endif
    } else {
#ifdef HAS_LOG
        LOG_INFO(LORA, "frequency setting are legal %u...%u Hz", Sx1262Instance.rf_frequency_hz - bandwidth_hz / 2,
                 Sx1262Instance.rf_frequency_hz + bandwidth_hz / 2);
#endif
    }
#endif /*HAS_LEGAL_BAND_CHECK*/
    gpio_set_state(DIO_SX1262_SS, 1);
    res = sx1262_init_gpio() && res;
#ifdef HAS_LOG
    LOG_WARNING(LORA, "StartWaitBusy");
#endif
    res = sx1262_reset() && res;
    res = sx1262_wait_on_busy(10000);
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "ChipBusy");
#endif
    } else {
#ifdef HAS_LOG
        LOG_INFO(LORA, "ChipSpare!");
#endif
    }

    if(res) {
        res = sx1262_is_exist();
    }
    if(true == res) {
#ifdef HAS_LOG
        LOG_INFO(LORA, "Config...");
#endif
        res = sx1262_wakeup() && res;

        res = sx1262_set_packet_type(Sx1262Instance.packet_param.packet_type) && res;

        res = sx1262_set_rf_frequency(Sx1262Instance.rf_frequency_hz, XTAL_FREQ_HZ) && res;

        res = sx1262_set_regulator_mode(REG_MODE_ONLY_LDO) && res;
        // res = sx1262_set_regulator_mode(REG_MODE_DC_DC_LDO) && res;

        res = sx1262_clear_fifo() && res;
        res = sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;

        res = sx1262_clear_dev_error() && res;

        res = sx1262_set_packet_type(Sx1262Instance.packet_param.packet_type) && res;

        res = sx1262_set_standby(STDBY_XOSC);

#ifdef HAS_LOG
        if(false == res) {
            LOG_ERROR(LORA, "SetStandByErr");
        }
#endif

        res = sx1262_set_modulation_params(&Sx1262Instance.mod_params) && res;
#ifdef HAS_LOG
        if(false == res) {
            LOG_ERROR(LORA, "SX1262SetModParErr");
        }
#endif

        res = sx1262_set_packet_params(&Sx1262Instance.packet_param) && res;

        res = sx1262_set_dio_irq_params(IQR_MAIN_INT, IQR_MAIN_INT, IQR_MAIN_INT, IQR_MAIN_INT) && res;
        res = sx1262_set_dio2_as_rf_switch_ctrl(DIO2_RF_SW) && res;

        res = sx1262_set_crc_poly(Sx1262Instance.crc_poly) && res;
        res = sx1262_set_crc_seed(Sx1262Instance.crc_init) && res;
        res = sx1262_set_sync_word(Sx1262Instance.set_sync_word) && res;
        res = sx1262_set_lora_sync_word(Sx1262Instance.lora_sync_word_set) && res;

        res = sx1262_start_rx(0xFFFFFF) && res;
        if(res) {
#ifdef HAS_LOG
            LOG_INFO(LORA, "StartRx");
#endif
        }

        res = sx1262_conf_tx(Sx1262Instance.output_power) && res;
        // res = sx1262_conf_rx() && res;

        // Sx1262Instance.set_sync_word = SYNC_WORD;

        Sx1262Instance.sync_reg = true;
        res = sx1262_start_rx(0xFFFFFF) && res;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "SX1262LinkErr");
#endif
    }
    if(false == res) {
        task_data[TASK_ID_LORA].on = false;
    }
    // Sx1262Instance.check_connectivity = true;
    if(true == res) {
#ifdef HAS_FREE_RTOS
        xTaskCreate(sx1262_thread, "sx1262", 5000, NULL, 6, NULL);
#endif /*HAS_FREE_RTOS*/
    }
    return res;
}

float lora_calc_data_rate(uint8_t sf_code, uint8_t bw_code, uint8_t cr_code) {
    float data_rate = 0.0;
    uint32_t bandwidth_hz = bandwidth2num((BandWidth_t)bw_code);
    data_rate = ((float)(bandwidth_hz * sf_code * 4)) / ((powf(2.0f, (float)sf_code)) * ((float)(4 + cr_code)));
    return data_rate;
}
#ifdef HAS_DEBUG

/*
 * de - Low Data Rate Optimize can
 *           be enabled  (Value of DE = 1) or
 *           disabled (Value of DE = 0)
 * header  -    implicit or explicit, i.e. H of value
 *                   0 indicates it is enabled and it is explicit mode where as H of value
 *                   1 indicates it is disabled and it is implicit mode.
 * n_preamble -  Number of symbols in preamble
 * */
float lora_calc_max_frame_tx_time(uint8_t sf_code, uint8_t bw_code, uint8_t cr_code, uint16_t n_preamble,
                                  uint8_t header, uint8_t low_data_rate_opt, float* Tsym, float* t_preamble) {
    float t_frame = 0.0f;
    float t_payload = 0.0f;
    uint16_t pl = 256;
    *Tsym = powf(2.0f, (float)sf_code) / ((float)bandwidth2num((BandWidth_t)bw_code));
    *t_preamble = (((float)n_preamble) + 4.25f) * (*Tsym);
    float payloadSymbNb =
        8.0f + float_max(((float)(cr_code + 4)) * ceilf(((float)(8 * pl - 4 * sf_code + 44 + 20 * header)) /
                                                        ((float)(4 * (sf_code - 2 * low_data_rate_opt)))),
                         0.0f);
    t_payload = payloadSymbNb * (*Tsym);
    t_frame = *t_preamble + t_payload;
    return t_frame;
}
#endif

bool sx1262_set_fs(void) {
    bool res = false;
    res = sx1262_send_opcode(OPCODE_SET_FS, NULL, 0, NULL, 0);
    return res;
}
