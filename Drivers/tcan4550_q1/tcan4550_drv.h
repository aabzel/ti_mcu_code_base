#ifndef TCAN4550_DRV_H
#define TCAN4550_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "tcan4550_reg.h"
#include "tcan4550_types.h"

#define MRAM_SIZE 2048U /*Byte*/

//#define DEVICE_ID "TCAN4550"
#define OP_CODE_READ 0x41
#define OP_CODE_WRITE 0x61

typedef struct xHeaderCom_t {
    uint8_t op_code;
    uint16_t addr;
    uint8_t words_cnt;
} __attribute__((packed)) HeaderCom_t;

typedef struct xTcan4550Reg_t {
    uint16_t addr;
    char* name;
} Tcan4550Reg_t;

extern const uint64_t exp_dev_id;

extern const Tcan4550Reg_t tCan4550RegLUT[];

bool is_tcan4550_connected(void);
bool tcan4550_init(void);
bool tcan4550_reset(void);
bool tcan4550_send(uint16_t id, uint64_t data);
bool tcan4550_clear_mram(void);
uint16_t tcan4550_get_reg_cnt(void);
bool tcan4550_chip_select(bool state);
bool tcan4550_read(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size);
bool tcan4550_write_reg(uint16_t address, uint32_t reg_val);

bool tcan4550_send_spi_header(uint8_t opcode, uint16_t address, uint8_t words);

bool tcan4550_read_reg(uint16_t address, uint32_t* out_reg);
bool tcan4550_write_reg(uint16_t address, uint32_t reg_val);
bool tcan4550_write_reg_lazy(uint16_t address, uint32_t reg_val);
const char* tcan4550_get_reg_name(uint16_t addr);

DevMode_t tcan4550_get_mode(void);
bool tcan4550_set_mode(DevMode_t dev_mode);

#ifdef __cplusplus
}
#endif

#endif /* TCAN4550_DRV_H  */
