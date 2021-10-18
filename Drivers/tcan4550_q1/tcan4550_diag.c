#include "tcan4550_diag.h"

#include <string.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "tcan4550_drv.h"

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

static char* mode2str(uint8_t code) {
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
    memcpy(&reg, &reg_val, 4);
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_SPI_2_REV, tcan4550_get_reg_name(ADDR_SPI_2_REV), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u-%u spi2 0x%x %u" CRLF, 24, 31, reg.spi2, reg.spi2);
    io_printf("bit %u-%u res 0x%x %u" CRLF, 16, 23, reg.res, reg.res);
    io_printf("bit %u-%u major 0x%x %u" CRLF, 8, 15, reg.major_id, reg.major_id);
    io_printf("bit %u-%u minor 0x%x %u" CRLF, 0, 7, reg.minor_id, reg.minor_id);
    return res;
}

bool tcan4550_parse_reg_dev_id0(uint32_t reg_val) {
    bool res = false;
    Type32Union_t un32;
    un32.u32 = reg_val;
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEVICE_ID0, tcan4550_get_reg_name(ADDR_DEVICE_ID0), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u-%u: %c" CRLF, 24, 31, un32.u8[0]);
    io_printf("bit %u-%u: %c" CRLF, 16, 23, un32.u8[1]);
    io_printf("bit %u-%u: %c" CRLF, 8, 15, un32.u8[2]);
    io_printf("bit %u-%u: %c" CRLF, 0, 7, un32.u8[3]);
    return res;
}

bool tcan4550_parse_reg_dev_id1(uint32_t reg_val) {
    bool res = false;
    Type32Union_t un32;
    un32.u32 = reg_val;
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEVICE_ID1, tcan4550_get_reg_name(ADDR_DEVICE_ID1), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u-%u: %c" CRLF, 24, 31, un32.u8[0]);
    io_printf("bit %u-%u: %c" CRLF, 16, 23, un32.u8[1]);
    io_printf("bit %u-%u: %c" CRLF, 8, 15, un32.u8[2]);
    io_printf("bit %u-%u: %c" CRLF, 0, 7, un32.u8[3]);
    return res;
}

bool tcan4550_parse_reg_status(uint32_t reg_val) {
    bool res = false;
    tCanRegStatus_t reg;
    memcpy(&reg, &reg_val, 4);
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_STATUS, tcan4550_get_reg_name(ADDR_STATUS), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u: internal_access_active %u" CRLF, 3, reg.internal_access_active);
    io_printf("bit %u: internal_error_interrupt %u" CRLF, 2, reg.internal_error_interrupt);
    io_printf("bit %u: spi_error_interrupt %u" CRLF, 1, reg.spi_error_interrupt);
    io_printf("bit %u: interrupt %u" CRLF, 0, reg.inter);
    return res;
}

bool tcan4550_parse_reg_mode_op_cfg(uint32_t reg_val) {
    bool res = false;
    tCanRegModeOpPinCfg_t reg;
    memcpy(&reg, &reg_val, 4);
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_DEV_CONFIG, tcan4550_get_reg_name(ADDR_DEV_CONFIG), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u-%u: wake_config %u" CRLF, 30, 31, reg.wake_config);
    io_printf("bit %u-%u: wd_timer %u" CRLF, 28, 29, reg.wd_timer);
    io_printf("bit %u: clk_ref %u %s" CRLF, 27, reg.clk_ref, clk_ref2str(reg.clk_ref));
    io_printf("bit %u-%u: rsvd %u" CRLF, 24, 26, reg.rsvd4);
    io_printf("bit %u%u: gpo2_config %u" CRLF, 22, 23, reg.gpo2_config);
    io_printf("bit %u: test_mode_en %u" CRLF, 21, reg.test_mode_en);
    io_printf("bit %u: rsvd %u" CRLF, 20, reg.rsvd3);
    io_printf("bit %u: nwkrq_voltage %u" CRLF, 19, reg.nwkrq_voltage);
    io_printf("bit %u: wd_bit_set %u" CRLF, 18, reg.wd_bit_set);
    io_printf("bit %u-%u: wd_action %u" CRLF, 16, 17, reg.wd_action);
    io_printf("bit %u-%u: gpio1_config %u" CRLF, 14, 15, reg.gpio1_config);
    io_printf("bit %u: fail_safe_en %u" CRLF, 13, reg.fail_safe_en);
    io_printf("bit %u: rsvd %u" CRLF, 12, reg.rsvd2);
    io_printf("bit %u-%u: gpio1_gpo_config %u" CRLF, 10, 11, reg.gpio1_gpo_config);
    io_printf("bit %u: inh_dis %u" CRLF, 9, reg.inh_dis);
    io_printf("bit %u: nwkrq_config %u" CRLF, 8, reg.nwkrq_config);
    io_printf("bit %u-%u: mode_sel %u %s" CRLF, 6, 7, reg.mode_sel, mode2str(reg.mode_sel));
    io_printf("bit %u-%u: rsvd %u" CRLF, 4, 5, reg.rsvd1);
    io_printf("bit %u: wd_en %u" CRLF, 3, reg.wd_en);
    io_printf("bit %u: device_reset %u" CRLF, 2, reg.device_reset);
    io_printf("bit %u: swe_dis %u" CRLF, 1, reg.swe_dis);
    io_printf("bit %u: test_mode_config %u" CRLF, 0, reg.test_mode_config);

    return res;
}

bool tcan4550_parse_reg_interrupt_flags(uint32_t reg_val) {
    bool res = false;
    tCanRegIntFl_t reg;
    memcpy(&reg, &reg_val, 4);
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_IF, tcan4550_get_reg_name(ADDR_IF), reg_val, utoa_bin32(reg_val));
    io_printf("bit %u: Global Voltage, Temp or WDTO %u" CRLF, 0, reg.vtwd);
    io_printf("bit %u: M_CAN global INT %u" CRLF, 1, reg.m_can_int);
    io_printf("bit %u: SPI Error %u" CRLF, 3, reg.spierr);
    io_printf("bit %u: CAN Error %u" CRLF, 5, reg.canerr);
    io_printf("bit %u: Wake Request %u" CRLF, 6, reg.wkrq);
    io_printf("bit %u: Global Error (Any Fault) %u" CRLF, 7, reg.globalerr);
    io_printf("bit %u: CAN Stuck Dominant %u" CRLF, 8, reg.candom);
    io_printf("bit %u: CAN Silent %u" CRLF, 10, reg.canslnt);
    io_printf("bit %u: Wake Error %u" CRLF, 13, reg.wkerr);
    io_printf("bit %u: Local Wake Up %u" CRLF, 14, reg.lwu);
    io_printf("bit %u: Can Bus Wake Up Interrupt %u" CRLF, 15, reg.canint);
    io_printf("bit %u: Uncorrectable ECC error %u" CRLF, 16, reg.eccerr);
    io_printf("bit %u: Watchdog Time Out %u" CRLF, 18, reg.wdto);
    io_printf("bit %u: Thermal Shutdown %u" CRLF, 19, reg.tsd);
    io_printf("bit %u: Power ON %u" CRLF, 20, reg.pwron);
    io_printf("bit %u: Under Voltage %u" CRLF, 21, reg.uvio);
    io_printf("bit %u: Under Voltage %u" CRLF, 22, reg.uvsup);
    io_printf("bit %u: Sleep Mode Status %u" CRLF, 23, reg.sms);
    io_printf("bit %u: CAN Bus normal %u" CRLF, 31, reg.canbusnom);
    return res;
}

bool tcan4550_parse_reg_bit_timing(uint32_t reg_val) {
    bool res = false;
    tCanRegBitTime_t reg;
    float tq = 0.0f;
    float can_bit_period = 0.0f;
    float can_bit_rate = 0.0f;
    memcpy(&reg, &reg_val, 4);
    io_printf("Addr 0x%04X %s 0x%08x 0b%s" CRLF, ADDR_MCAN_NBTP, tcan4550_get_reg_name(ADDR_MCAN_NBTP), reg_val,
              utoa_bin32(reg_val));
    io_printf("bit %u-%u: Nominal Time Segment After Sample Point %u" CRLF, 0, 6, reg.ntseg2);
    io_printf("bit %u-%u: Nominal Time Segment Before Sample Point %u" CRLF, 8, 15, reg.ntseg1);
    tq = ((float)reg.nbrp) * (1.0f / ((float)CAN_XTAL_HZ));
    io_printf("bit %u-%u: Nominal Bit Rate Prescaler %u bit quanta time: %f s" CRLF, 16, 24, reg.nbrp, tq);
    io_printf("bit %u-%u: Nominal (RE)Synchronization Jump Width %u" CRLF, 25, 31, reg.nsjw);
    can_bit_period = tq * ((float)(reg.ntseg1 + reg.ntseg2));
    can_bit_rate = 1.0f / can_bit_period;
    io_printf("Can Bit Rate: %f Bit/s" CRLF, can_bit_rate);
    return res;
}
