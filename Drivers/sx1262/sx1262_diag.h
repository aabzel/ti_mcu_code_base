#ifndef SX1262_DIAG_H
#define SX1262_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "sx1262_drv.h"

const char* cmd_stat2str(uint8_t cmd_stat);
const char* chip_mode2str(uint8_t chip_mode);
bool parse_dev_stat(uint8_t dev_stat);
bool print_int_diag(Sx1262IrqCnt_t* irq_cnt);
bool parse_op_error(uint16_t op_error);
bool parse_irq_stat(uint16_t irq_stat);
bool printf_pack_stat(PaketStat_t* pack_stat, char* name);
#ifdef __cplusplus
}
#endif

#endif /* SX1262_DIAG_H */
