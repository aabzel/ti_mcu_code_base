#ifndef TCAN4550_DIAG_H
#define TCAN4550_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct xtCanRegRev_t {
    uint8_t minor_id;
    uint8_t major_id;
    uint8_t res;
    uint8_t spi2;
} __attribute__((packed)) tCanRegRev_t;

typedef struct xtCanRegStatus_t {
    uint8_t inter: 1;
    uint8_t spi_error_interrupt : 1;
    uint8_t internal_error_interrupt : 1;
    uint8_t internal_access_active : 1;
    uint8_t read_fifo_available : 1;
    uint8_t write_fifo_available : 1;
    uint16_t rsvd1 : 10;
    uint8_t read_underflow : 1;
    uint8_t read_overflow : 1;
    uint8_t write_underflow : 1;
    uint8_t write_overflow : 1;
    uint8_t invalid_command : 1;
    uint8_t spi_end_error : 1;
    uint8_t rsvd2 : 2;
    uint8_t write_fifo_overflow : 1;
    uint8_t read_fifo_empty : 1;
    uint8_t read_fifo_underflow : 1;
    uint8_t internal_error_log_write : 1;
    uint8_t internal_write_error : 1;
    uint8_t internal_read_error : 1;
    uint8_t rsvd3 : 2;
} __attribute__((packed)) tCanRegStatus_t;

bool tcan4550_parse_reg_revision(uint32_t reg_val);
bool tcan4550_parse_reg_dev_id0(uint32_t reg_val);
bool tcan4550_parse_reg_dev_id1(uint32_t reg_val);
bool tcan4550_parse_reg_status(uint32_t reg_val);

#ifdef __cplusplus
}
#endif

#endif /* TCAN4550_DIAG_H  */
