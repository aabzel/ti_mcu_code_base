#ifndef TCAN4550_DRV_H
#define TCAN4550_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define MRAM_SIZE 2048U /*Byte*/
/*ADDRESS*/
#define ADDR_DEVICE_ID_0 0
#define ADDR_DEVICE_ID_1 4
#define ADDR_READ_WRITE_TEST 0x0808
#define ADDR_SPI_2_REV 0x0008
#define ADDR_STATUS 0x000C
#define ADDR_CREL 0x1000
#define ADDR_IR  0x1050
#define ADDR_IE 0x1054
#define ADDR_MRAM 0x8000

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
bool tcan4550_clear_mram(void);
uint16_t tcan4550_get_reg_cnt(void);
bool tcan4550_chip_select(bool state);
bool tcan4550_read(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size);
bool tcan4550_write_reg(uint16_t address, uint32_t reg_val);
bool tcan4550_read_reg(uint16_t address, uint32_t* out_reg);
bool tcan4550_write_reg(uint16_t address, uint32_t reg_val);
const char* tcan4550_get_reg_name(uint16_t addr);
// get mode

#ifdef __cplusplus
}
#endif

#endif /* TCAN4550_DRV_H  */
