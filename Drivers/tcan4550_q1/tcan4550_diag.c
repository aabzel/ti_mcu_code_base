#include "tcan4550_diag.h"

#include <string.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "tcan4550_drv.h"

const char* lec2str(uint8_t lec) {
    char* name = "undf";
    switch(lec) {
    case LEC_NO_ERROR:
        name = "noError";
        break;
    case LEC_STUFF_ERROR:
        name = "StuffError";
        break;
    case LEC_FORM_ERROR:
        name = "FormError";
        break;
    case LEC_ACK_ERROR:
        name = "AckError";
        break;
    case LEC_BIT1_ERROR:
        name = "Bit1Error";
        break;
    case LEC_BIT0_ERROR:
        name = "Bit0_Error";
        break;
    case LEC_CRC_ERROR:
        name = "CrcError";
        break;
    case LEC_NO_CHANGE:
        name = "NoChange";
        break;
    default:
        name = "undef";
        break;
    }
    return name;
}

static char* clk_ref2str(uint8_t code) {
    char* name = "undf";
    switch(code) {
    case CLK_REF_20MHZ:
        name = "20Mhz";
        break;
    case CLK_REF_40MHZ:
        name = "40Mhz";
        break;
    }
    return name;
}

char* can_mode2str(uint8_t code) {
    char* name = "undf";
    switch(code) {
    case MODE_SLEEP:
        name = "sleep";
        break;
    case MODE_STANDBY:
        name = "standby";
        break;
    case MODE_NORMAL:
        name = "norm";
        break;
    case MODE_RESERVED:
        name = "resvd";
        break;
    }
    return name;
}

bool tcan4550_parse_reg_revision(uint32_t reg_val) {
    bool res = false;
    tCanRegRev_t reg;
    LOG_DEBUG(CAN, CRLF);
    memcpy(&reg, &reg_val, 4);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_SPI_2_REV, tcan4550_get_reg_name(ADDR_SPI_2_REV), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u-%u spi2 0x%x %u" CRLF, 24, 31, reg.spi2, reg.spi2);
    LOG_DEBUG(CAN, "bit %u-%u res 0x%x %u" CRLF, 16, 23, reg.res, reg.res);
    LOG_DEBUG(CAN, "bit %u-%u major 0x%x %u" CRLF, 8, 15, reg.major_id, reg.major_id);
    LOG_DEBUG(CAN, "bit %u-%u minor 0x%x %u" CRLF, 0, 7, reg.minor_id, reg.minor_id);
    return res;
}

bool tcan4550_parse_reg_dev_id0(uint32_t reg_val) {
    bool res = false;
    Type32Union_t un32;
    un32.u32 = reg_val;
    LOG_DEBUG(CAN, CRLF);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEVICE_ID0, tcan4550_get_reg_name(ADDR_DEVICE_ID0), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 24, 31, un32.u8[0]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 16, 23, un32.u8[1]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 8, 15, un32.u8[2]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 0, 7, un32.u8[3]);
    return res;
}

bool tcan4550_parse_reg_dev_id1(uint32_t reg_val) {
    bool res = false;
    Type32Union_t un32;
    un32.u32 = reg_val;
    LOG_DEBUG(CAN, CRLF);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEVICE_ID1, tcan4550_get_reg_name(ADDR_DEVICE_ID1), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 24, 31, un32.u8[0]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 16, 23, un32.u8[1]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 8, 15, un32.u8[2]);
    LOG_DEBUG(CAN, "bit %u-%u: %c" CRLF, 0, 7, un32.u8[3]);
    return res;
}

bool tcan4550_parse_reg_status(uint32_t reg_val) {
    bool res = false;
    tCanRegStatus_t reg = {0};
    memcpy(&reg, &reg_val, 4);
    LOG_DEBUG(CAN, CRLF);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_SPI_STATUS, tcan4550_get_reg_name(ADDR_SPI_STATUS), reg_val,
              utoa_bin32(reg_val));
    if(reg.internal_read_error) {
        LOG_DEBUG(CAN, "bit %u: Internal read received an error response %u" CRLF, 29, reg.internal_read_error);
    }
    if(reg.internal_write_error) {
        LOG_DEBUG(CAN, "bit %u: Internal write received an error response %u" CRLF, 28, reg.internal_write_error);
    }
    if(reg.internal_error_log_write) {
        LOG_DEBUG(CAN, "bit %u: Entry written to the Internal error log %u" CRLF, 27, reg.internal_error_log_write);
    }
    if(reg.read_fifo_underflow) {
        LOG_DEBUG(CAN, "bit %u: Read FIFO underflow after 1 or more read data words returned %u" CRLF, 26,
                  reg.read_fifo_underflow);
    }
    if(reg.read_fifo_empty) {
        LOG_DEBUG(CAN, "bit %u: Read FIFO empty for first read data word to return %u" CRLF, 25, reg.read_fifo_empty);
    }
    if(reg.write_fifo_overflow) {
        LOG_DEBUG(CAN, "bit %u: Write/command FIFO overflow %u" CRLF, 24, reg.write_fifo_overflow);
    }
    if(reg.spi_end_error) {
        LOG_DEBUG(CAN, "bit %u: SPI transfer did not end on a byte boundary %u" CRLF, 21, reg.spi_end_error);
    }
    if(reg.invalid_command) {
        LOG_DEBUG(CAN, "bit %u: Invalid SPI command received %u" CRLF, 20, reg.invalid_command);
    }
    if(reg.write_overflow) {
        LOG_DEBUG(CAN, "bit %u: SPI write sequence had continue requests after the data transfer was completed %u" CRLF,
                  19, reg.write_overflow);
    }
    if(reg.write_underflow) {
        LOG_DEBUG(CAN, "bit %u: SPI write sequence ended with less data transferred then requested %u" CRLF, 18,
                  reg.write_underflow);
    }
    if(reg.read_overflow) {
        LOG_DEBUG(CAN, "bit %u: SPI read sequence had continue requests after the data transfer was completed %u" CRLF,
                  17, reg.read_overflow);
    }
    if(reg.read_underflow) {
        LOG_DEBUG(CAN, "bit %u: SPI read sequence ended with less data transferred then requested %u" CRLF, 16,
                  reg.read_underflow);
    }
    if(reg.write_fifo_available) {
        LOG_DEBUG(CAN, "bit %u: write fifo empty entries is greater than or equal to the write_fifo_threshold %u" CRLF,
                  5, reg.write_fifo_available);
    }
    if(reg.read_fifo_available) {
        LOG_DEBUG(CAN, "bit %u: Read fifo entries is greater than or equal to the read_fifo_threshold %u" CRLF, 4,
                  reg.read_fifo_available);
    }
    if(reg.internal_access_active) {
        LOG_DEBUG(CAN, "bit %u: Internal Multiple transfer mode access in progress %u" CRLF, 3,
                  reg.internal_access_active);
    }
    if(reg.internal_error_interrupt) {
        LOG_DEBUG(CAN, "bit %u: Unmasked Internal error set %u" CRLF, 2, reg.internal_error_interrupt);
    }
    if(reg.spi_error_interrupt) {
        LOG_DEBUG(CAN, "bit %u: Unmasked SPI error set %u" CRLF, 1, reg.spi_error_interrupt);
    }
    if(reg.inter) {
        LOG_DEBUG(CAN, "bit %u: interrupt %u" CRLF, 0, reg.inter);
    }
    return res;
}

bool tcan4550_parse_reg_mode_op_cfg(uint32_t reg_val) {
    bool res = false;
    tCanRegModeOpPinCfg_t reg;
    memcpy(&reg, &reg_val, 4);
    LOG_DEBUG(CAN, CRLF);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEV_CONFIG, tcan4550_get_reg_name(ADDR_DEV_CONFIG), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u-%u: wake_config %u" CRLF, 30, 31, reg.wake_config);
    LOG_DEBUG(CAN, "bit %u-%u: wd_timer %u" CRLF, 28, 29, reg.wd_timer);
    LOG_DEBUG(CAN, "bit %u: clk_ref %u %s" CRLF, 27, reg.clk_ref, clk_ref2str(reg.clk_ref));
    LOG_DEBUG(CAN, "bit %u-%u: rsvd %u" CRLF, 24, 26, reg.rsvd4);
    LOG_DEBUG(CAN, "bit %u%u: gpo2_config %u" CRLF, 22, 23, reg.gpo2_config); // NC
    LOG_DEBUG(CAN, "bit %u: test_mode_en %u" CRLF, 21, reg.test_mode_en);
    LOG_DEBUG(CAN, "bit %u: rsvd %u" CRLF, 20, reg.rsvd3);
    LOG_DEBUG(CAN, "bit %u: nwkrq_voltage %u" CRLF, 19, reg.nwkrq_voltage);
    LOG_DEBUG(CAN, "bit %u: wd_bit_set %u" CRLF, 18, reg.wd_bit_set);
    LOG_DEBUG(CAN, "bit %u-%u: wd_action %u" CRLF, 16, 17, reg.wd_action);
    LOG_DEBUG(CAN, "bit %u-%u: gpio1_config %u" CRLF, 14, 15, reg.gpio1_config);
    LOG_DEBUG(CAN, "bit %u: fail_safe_en %u" CRLF, 13, reg.fail_safe_en);
    LOG_DEBUG(CAN, "bit %u: rsvd %u" CRLF, 12, reg.rsvd2);
    LOG_DEBUG(CAN, "bit %u-%u: gpio1_gpo_config %u" CRLF, 10, 11, reg.gpio1_gpo_config); // NC
    LOG_DEBUG(CAN, "bit %u: inh_dis %u" CRLF, 9, reg.inh_dis);
    LOG_DEBUG(CAN, "bit %u: nwkrq_config %u" CRLF, 8, reg.nwkrq_config);
    LOG_DEBUG(CAN, "bit %u-%u: mode_sel %u %s" CRLF, 6, 7, reg.mode_sel, can_mode2str(reg.mode_sel));
    LOG_DEBUG(CAN, "bit %u-%u: rsvd %u" CRLF, 4, 5, reg.rsvd1);
    LOG_DEBUG(CAN, "bit %u: wd_en %u" CRLF, 3, reg.wd_en);
    LOG_DEBUG(CAN, "bit %u: device_reset %u" CRLF, 2, reg.device_reset);
    LOG_DEBUG(CAN, "bit %u: swe_dis %u" CRLF, 1, reg.swe_dis);
    LOG_DEBUG(CAN, "bit %u: test_mode_config %u" CRLF, 0, reg.test_mode_config);

    return res;
}

bool tcan4550_parse_reg_interrupt_flags(uint32_t reg_val) {
    bool res = false;
    tCanRegIntFl_t reg;
    memcpy(&reg, &reg_val, 4);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_IF, tcan4550_get_reg_name(ADDR_IF), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u: Global Voltage, Temp or WDTO %u" CRLF, 0, reg.vtwd);
    LOG_DEBUG(CAN, "bit %u: M_CAN global INT %u" CRLF, 1, reg.m_can_int);
    LOG_DEBUG(CAN, "bit %u: SPI Error %u" CRLF, 3, reg.spierr);
    LOG_DEBUG(CAN, "bit %u: CAN Error %u" CRLF, 5, reg.canerr);
    LOG_DEBUG(CAN, "bit %u: Wake Request %u" CRLF, 6, reg.wkrq);
    LOG_DEBUG(CAN, "bit %u: Global Error (Any Fault) %u" CRLF, 7, reg.globalerr);
    LOG_DEBUG(CAN, "bit %u: CAN Stuck Dominant %u" CRLF, 8, reg.candom);
    LOG_DEBUG(CAN, "bit %u: CAN Silent %u" CRLF, 10, reg.canslnt);
    LOG_DEBUG(CAN, "bit %u: Wake Error %u" CRLF, 13, reg.wkerr);
    LOG_DEBUG(CAN, "bit %u: Local Wake Up %u" CRLF, 14, reg.lwu);
    LOG_DEBUG(CAN, "bit %u: Can Bus Wake Up Interrupt %u" CRLF, 15, reg.canint);
    LOG_DEBUG(CAN, "bit %u: Uncorrectable ECC error %u" CRLF, 16, reg.eccerr);
    LOG_DEBUG(CAN, "bit %u: Watchdog Time Out %u" CRLF, 18, reg.wdto);
    LOG_DEBUG(CAN, "bit %u: Thermal Shutdown %u" CRLF, 19, reg.tsd);
    LOG_DEBUG(CAN, "bit %u: Power ON %u" CRLF, 20, reg.pwron);
    LOG_DEBUG(CAN, "bit %u: Under Voltage %u" CRLF, 21, reg.uvio);
    LOG_DEBUG(CAN, "bit %u: Under Voltage %u" CRLF, 22, reg.uvsup);
    LOG_DEBUG(CAN, "bit %u: Sleep Mode Status %u" CRLF, 23, reg.sms);
    LOG_DEBUG(CAN, "bit %u: CAN Bus normal %u" CRLF, 31, reg.canbusnom);
    return res;
}

bool tcan4550_parse_reg_bit_timing(uint32_t reg_val) {
    bool res = false;
    tCanRegBitTime_t reg;
    float tq = 0.0f;
    float can_bit_period = 0.0f;
    float can_bit_rate = 0.0f;
    memcpy(&reg, &reg_val, 4);
    LOG_DEBUG(CAN, CRLF);
    LOG_DEBUG(CAN, "Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_MCAN_NBTP, tcan4550_get_reg_name(ADDR_MCAN_NBTP), reg_val,
              utoa_bin32(reg_val));
    LOG_DEBUG(CAN, "bit %u-%u: Nominal Time Segment After Sample Point %u" CRLF, 0, 6, reg.ntseg2);
    LOG_DEBUG(CAN, "bit %u-%u: Nominal Time Segment Before Sample Point %u" CRLF, 8, 15, reg.ntseg1);
    tq = ((float)reg.nbrp) * (1.0f / ((float)CAN_XTAL_HZ));
    LOG_DEBUG(CAN, "bit %u-%u: Nominal Bit Rate Prescaler %u bit quanta time: %f s" CRLF, 16, 24, reg.nbrp, tq);
    LOG_DEBUG(CAN, "bit %u-%u: Nominal (RE)Synchronization Jump Width %u" CRLF, 25, 31, reg.nsjw);
    can_bit_period = tq * ((float)(reg.ntseg1 + reg.ntseg2));
    can_bit_rate = 1.0f / can_bit_period;
    LOG_DEBUG(CAN, "Can Bit Rate: %f Bit/s" CRLF, can_bit_rate);
    return res;
}

bool tcan4550_parse_reg_proto_state(uint32_t reg_val) {
    bool res = false;
    tCanRegProtStat_t ProtoState = {0};
    LOG_DEBUG(CAN, CRLF);
    memcpy(&ProtoState.word, &reg_val, 4);
    LOG_DEBUG(CAN, "bit %u-%u: Transmitter Delay Compensation Value [%u]" CRLF, 16, 22, ProtoState.tdcv);
    LOG_DEBUG(CAN, "bit %u: Protocol Exception Event [%u]" CRLF, 14, ProtoState.pxe);
    LOG_DEBUG(CAN, "bit %u:Received a CAN FD Message [%u]" CRLF, 13, ProtoState.rfdf);
    LOG_DEBUG(CAN, "bit %u:BRS flag of last received CAN FD Message [%u]" CRLF, 12, ProtoState.rbrs);
    LOG_DEBUG(CAN, "bit %u:ESI flag of last received CAN FD Message [%u]" CRLF, 11, ProtoState.resi);
    LOG_DEBUG(CAN, "bit %u-%u: Data Phase Last Error Code [%u]" CRLF, 8, 10, ProtoState.dlec);
    LOG_DEBUG(CAN, "bit %u:Bus_Off Status [%u]" CRLF, 7, ProtoState.bo);
    LOG_DEBUG(CAN, "bit %u:Warning Status [%u]" CRLF, 6, ProtoState.ew);
    LOG_DEBUG(CAN, "bit %u:Error Passive [%u]" CRLF, 5, ProtoState.ep);
    LOG_DEBUG(CAN, "bit %u-%u: Activity [%u]" CRLF, 3, 4, ProtoState.act);
    LOG_DEBUG(CAN, "bit %u-%u: Last Error Code [%u] %s" CRLF, 0, 2, ProtoState.lec, lec2str(ProtoState.lec));
    return res;
}
