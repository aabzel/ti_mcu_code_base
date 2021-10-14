#include "tcan4550_diag.h"

#include <string.h>

#include "convert.h"
#include "data_utils.h"
#include "io_utils.h"
#include "tcan4550_drv.h"

bool tcan4550_parse_reg_revision(uint32_t reg_val) {
    bool res = false;
    tCanRegRev_t reg;
    memcpy(&reg, &reg_val, 4);
    io_printf("%s 0x%08x 0b%s" CRLF, tcan4550_get_reg_name(ADDR_SPI_2_REV), reg_val, utoa_bin32(reg_val));
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
    io_printf("%s 0x%08x 0b%s" CRLF, tcan4550_get_reg_name(ADDR_DEVICE_ID_0), reg_val, utoa_bin32(reg_val));
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
    io_printf("0x%08x 0b%s" CRLF, reg_val, utoa_bin32(reg_val));
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
    io_printf("%s 0x%08x 0b%s" CRLF, tcan4550_get_reg_name(ADDR_STATUS), reg_val, utoa_bin32(reg_val));
    io_printf("bit %u: internal_access_active %u" CRLF, 3, reg.internal_access_active);
    io_printf("bit %u: internal_error_interrupt %u" CRLF, 2, reg.internal_error_interrupt);
    io_printf("bit %u: spi_error_interrupt %u" CRLF, 1, reg.spi_error_interrupt);
    io_printf("bit %u: interrupt %u" CRLF, 0, reg.inter);
    return res;
}
