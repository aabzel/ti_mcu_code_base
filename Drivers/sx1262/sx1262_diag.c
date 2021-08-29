#include "sx1262_diag.h"

#include <inttypes.h>
#include <stdio.h>

#include "bit_utils.h"
#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "str_utils.h"
#include "sx1262_drv.h"

const char* cmd_stat2str(uint8_t cmd_stat) {
    const char* name = "undef";
    switch(cmd_stat) {
    case 0:
        name = "Rsvd";
        break;
    case 2:
        name = "DataAvailToHost";
        break;
    case 3:
        name = "timeout";
        break;
    case 4:
        name = "processingError";
        break;
    case 5:
        name = "FailureExecute";
        break;
    case 6:
        name = "TxDone";
        break;
    default:
        name = "error";
        break;
    }
    return name;
}

const char* pack_type2str(RadioPacketType_t packet_type) {
    const char* name = "def";
    switch(packet_type) {
    case PACKET_TYPE_GFSK:
        name = "gfsk";
        break;
    case PACKET_TYPE_LORA:
        name = "LoRa";
        break;
    default:
        name = "undef";
        break;
    }
    return name;
}

const char* chip_mode2str(uint8_t chip_mode) {
    const char* name = "undef";
    switch(chip_mode) {
    case 0:
        name = "unUsed";
        break;
    case 2:
        name = "STBY_RC";
        break;
    case 3:
        name = "STBY_XOSC";
        break;
    case 4:
        name = "FS";
        break;
    case 5:
        name = "RX";
        break;
    case 6:
        name = "TX";
        break;
    default:
        name = "error";
        break;
    }
    return name;
}
// sx126x_convert_freq_in_hz_to_pll_step
bool parse_dev_stat(uint8_t dev_stat) {
    uint8_t code = 0;
    io_printf("status: 0x%02x 0b%s" CRLF, dev_stat, utoa_bin8(dev_stat));
    code = extract_subval_from_8bit(dev_stat, 3, 1);
    io_printf("cmd_stat: [%u] %s" CRLF, code, cmd_stat2str(code));

    code = extract_subval_from_8bit(dev_stat, 6, 4);
    io_printf("chip_mode: [%u] %s" CRLF, code, chip_mode2str(code));
    return true;
}

bool parse_op_error(uint16_t op_error) {
    bool res = false;
    io_printf("op_error: 0x%04x 0b%s" CRLF, op_error, utoa_bin16(MASK_9BIT & op_error));
    if((1 << OP_ERR_BIT_RC64K_CALIB_ERR) == (op_error & (1 << OP_ERR_BIT_RC64K_CALIB_ERR))) {
        LOG_ERROR(LORA, "RC64k");
        res = true;
    }
    if((1 << OP_ERR_BIT_RC13M_CALIB_ERR) == (op_error & (1 << OP_ERR_BIT_RC13M_CALIB_ERR))) {
        LOG_ERROR(LORA, "RC13M");
        res = true;
    }
    if((1 << OP_ERR_BIT_PLL_CALIB_ERR) == (op_error & (1 << OP_ERR_BIT_PLL_CALIB_ERR))) {
        LOG_ERROR(LORA, "PLL");
        res = true;
    }
    if((1 << OP_ERR_BIT_ADC_CALIB_ERR) == (op_error & (1 << OP_ERR_BIT_ADC_CALIB_ERR))) {
        LOG_ERROR(LORA, "ADC calibration failed");
        res = true;
    }
    if((1 << OP_ERR_BIT_IMG_CALIB_ERR) == (op_error & (1 << OP_ERR_BIT_IMG_CALIB_ERR))) {
        LOG_ERROR(LORA, "IMG");
        res = true;
    }
    if((1 << OP_ERR_BIT_XOSC_START_ERR) == (op_error & (1 << OP_ERR_BIT_XOSC_START_ERR))) {
        LOG_ERROR(LORA, "XOSC");
        res = true;
    }
    if((1 << OP_ERR_BIT_PLL_LOCK_ERR) == (op_error & (1 << OP_ERR_BIT_PLL_LOCK_ERR))) {
        LOG_ERROR(LORA, "PLL lock");
        res = true;
    }
    if((1 << OP_ERR_BIT_PA_RAMP_ERR) == (op_error & (1 << OP_ERR_BIT_PA_RAMP_ERR))) {
        LOG_ERROR(LORA, "PA ramping");
        res = true;
    }
    return res;
}

bool parse_irq_stat(uint16_t irq_stat) {
    bool res = false;
    io_printf("irq_stat: 0x%04x 0b%s" CRLF, irq_stat, utoa_bin16(irq_stat));
    if((1 << IRQ_BIT_TXDONE) == (irq_stat & (1 << IRQ_BIT_TXDONE))) {
        LOG_INFO(LORA, "TX done");
        res = true;
    }
    if((1 << IRQ_BIT_RXDONE) == (irq_stat & (1 << IRQ_BIT_RXDONE))) {
        LOG_INFO(LORA, "RX done");
        res = true;
    }
    if((1 << IRQ_BIT_PREAMBLEDETECTED) == (irq_stat & (1 << IRQ_BIT_PREAMBLEDETECTED))) {
        LOG_NOTICE(LORA, "preamble detected");
        res = true;
    }
    if((1 << IRQ_BIT_SYNCWORDVALID) == (irq_stat & (1 << IRQ_BIT_SYNCWORDVALID))) {
        LOG_NOTICE(LORA, "sync word valid");
        res = true;
    }
    if((1 << IRQ_BIT_HEADERVALID) == (irq_stat & (1 << IRQ_BIT_HEADERVALID))) {
        LOG_NOTICE(LORA, "LoRa header received");
        res = true;
    }
    if((1 << IRQ_BIT_CRCERR) == (irq_stat & (1 << IRQ_BIT_CRCERR))) {
        LOG_NOTICE(LORA, "Wrong CRC received");
        res = true;
    }
    if((1 << IRQ_BIT_CADDONE) == (irq_stat & (1 << IRQ_BIT_CADDONE))) {
        LOG_NOTICE(LORA, "Channel activity detection finished");
        res = true;
    }
    if((1 << IRQ_BIT_CADDETECTED) == (irq_stat & (1 << IRQ_BIT_CADDETECTED))) {
        LOG_NOTICE(LORA, "Channel activity detected ");
        res = true;
    }
    if((1 << IRQ_BIT_TIMEOUT) == (irq_stat & (1 << IRQ_BIT_TIMEOUT))) {
        LOG_WARNING(LORA, "Rx or Tx timeout");
        res = true;
    }
    return res;
}

bool printf_pack_stat(PaketStat_t* pack_stat, char* name) {
    bool res = false;
    if(pack_stat && name) {
        res = true;
        io_printf("%s:crc_error %u pkt" CRLF, name, pack_stat->nb_pkt_crc_error);
        io_printf("%s:header_err %u pkt" CRLF, name, pack_stat->nb_pkt_header_err);
        io_printf("%s:length_error %u pkt" CRLF, name, pack_stat->nb_pkt_length_error);
        io_printf("%s:received %u pkt" CRLF, name, pack_stat->nb_pkt_received);
    }
    return res;
}

bool print_int_diag(Sx1262IrqCnt_t* irq_cnt) {
    bool res = false;

    if(0 < irq_cnt->total) {
        io_printf("total: %u " CRLF, irq_cnt->total);
        res = true;
    }

    if(0 < irq_cnt->rx_done) {
        io_printf("rx done: %u " CRLF, irq_cnt->rx_done);
        res = true;
    }
    if(0 < irq_cnt->tx_done) {
        io_printf("tx done: %u " CRLF, irq_cnt->tx_done);
        res = true;
    }
    if(0 < irq_cnt->timeout) {
        io_printf("timeout: %u" CRLF, irq_cnt->timeout);
        res = true;
    }
    if(0 < irq_cnt->crc_err) {
        io_printf("crc err: %u" CRLF, irq_cnt->crc_err);
        res = true;
    }
    if(0 < irq_cnt->preamble_detected) {
        io_printf("preamble_detected: %u" CRLF, irq_cnt->preamble_detected);
        res = true;
    }
    if(0 < irq_cnt->header_valid) {
        io_printf("header valid: %u" CRLF, irq_cnt->header_valid);
        res = true;
    }
    if(0 < irq_cnt->cad_done) {
        io_printf("cad done: %u" CRLF, irq_cnt->cad_done);
        res = true;
    }
    if(0 < irq_cnt->cad_detected) {
        io_printf("cad_detected: %u" CRLF, irq_cnt->cad_detected);
        res = true;
    }
    if(0 < irq_cnt->header_err) {
        io_printf("header err: %u" CRLF, irq_cnt->header_err);
        res = true;
    }
    if(0 < irq_cnt->syncword_valid) {
        io_printf("syncword valid: %u" CRLF, irq_cnt->syncword_valid);
        res = true;
    }
    if(0 < irq_cnt->cad_done) {
        io_printf("cad done: %u " CRLF, irq_cnt->cad_done);
        res = true;
    }
    return res;
}
