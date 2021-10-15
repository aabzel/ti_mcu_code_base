#include "tcan4550_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "byte_utils.h"
#include "clocks.h"
#include "data_utils.h"
#include "debug_info.h"
#include "log.h"
#include "spi_drv.h"
#include "sys_config.h"

const uint64_t exp_dev_id = 0x343535305443414E;

const Tcan4550Reg_t tCan4550RegLUT[] = {
    {ADDR_IR, "IntReg"},         {ADDR_IE, "IntEn"},          {ADDR_IF, "IntFlgs"},
    {ADDR_DEV_CONFIG, "DevCfg"}, {ADDR_DEVICE_ID0, "DevId0"}, {ADDR_DEVICE_ID1, "DevId1"},
    {ADDR_SPI_2_REV, "SPIrev"},  {ADDR_STATUS, "Status"},     {ADDR_CREL, "CREL"},
};

const char* tcan4550_get_reg_name(uint16_t addr) {
    const char* reg_name = "undef";
    uint16_t cnt = tcan4550_get_reg_cnt();
    uint16_t i = 0;
    for(i = 0; i < cnt; i++) {
        if(tCan4550RegLUT[i].addr == addr) {
            reg_name = tCan4550RegLUT[i].name;
        }
    }
    return reg_name;
}

uint16_t tcan4550_get_reg_cnt(void) {
    uint16_t reg_cnt = ARRAY_SIZE(tCan4550RegLUT);
    return reg_cnt;
}

bool tcan4550_chip_select(bool state) {
    bool res = false;
    if(true == state) {
        GPIO_writeDio(DIO_SX1262_SS, 1);
        GPIO_writeDio(DIO_CAN_SS, 0);
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

#define TCAN4550_CHIP_SELECT(CALL_BACK)                                                                                \
    do {                                                                                                               \
        res = tcan4550_chip_select(true);                                                                              \
        delay_ms(1);                                                                                                   \
        res = CALL_BACK;                                                                                               \
        delay_ms(1);                                                                                                   \
        res = tcan4550_chip_select(false);                                                                             \
    } while(0);

static bool init_spi_header(HeaderCom_t* header, uint8_t op_code, uint16_t address, uint8_t words_cnt) {
    bool res = false;
    if(header) {
        header->op_code = op_code;
        header->addr = reverse_byte_order_uint16(address);
        header->words_cnt = words_cnt;
        res = true;
    }
    return res;
}
static bool tcan4550_read_proc(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size) {
    bool res = true;
    uint8_t tx_array[4];
    res = init_spi_header((HeaderCom_t*)tx_array, OP_CODE_READ, address, len);
    // tx_array[0] = OP_CODE_READ;
    //  memcpy(&tx_array[1], &address, 2);
    //  tx_array[3] = len;
    res = spi_write(SPI0_INX, tx_array, sizeof(tx_array)) && res;
    res = spi_read(SPI0_INX, out_array, (uint16_t)(len * 4)) && res;
    return res;
}

bool tcan4550_read(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size) {
    bool res = true;
    if((NULL != out_array) && ((len * 4) <= size)) {
        TCAN4550_CHIP_SELECT(tcan4550_read_proc(address, len, out_array, size));
    } else {
        res = false;
    }
    return res;
}

bool is_tcan4550_connected(void) {
    bool res = false;
    uint64_t read_dev_id = 0;
    res = tcan4550_read(ADDR_DEVICE_ID0, 2, (uint8_t*)&read_dev_id, sizeof(read_dev_id));
    if(res) {
        if(exp_dev_id == read_dev_id) {
            res = true;
            LOG_INFO(CAN, "spot dev id: 0x%llx", read_dev_id);
            print_mem((uint8_t*)&read_dev_id, sizeof(read_dev_id), true, true);
        } else {
            res = false;
            LOG_ERROR(CAN, "Unable to spot dev id: 0x%llx", read_dev_id);
        }
    } else {
        LOG_ERROR(CAN, "Unable to read reg");
    }
    return res;
}

bool tcan4550_reset(void) {
    bool res = true;
    /*see page 25 in spec 8.3.8 RST Pin*/
    GPIO_writeDio(DIO_CAN_RST, 1);
    delay_ms(1);
    GPIO_writeDio(DIO_CAN_RST, 0);
    /*tPULSE_WIDTH Width of the input pulse 30 us*/
    delay_ms(1);
    return res;
}

bool tcan4550_init(void) {
    bool res = true;
    GPIO_writeDio(DIO_CAN_SS, 1);
    GPIO_writeDio(DIO_CAN_RST, 1);

    res = tcan4550_reset();

    res = is_tcan4550_connected();
    if(res) {
    }
    return res;
}

static bool tcan4550_read_reg_proc(uint16_t address, uint32_t* out_reg) {
    bool res = true;
    // see page 44 Figure 8-18. Read (Command OpCode 8h41)
    uint32_t read_reg;
    uint8_t tx_array[4];
    res = init_spi_header((HeaderCom_t*)tx_array, OP_CODE_READ, address, 1);
    res = spi_write(SPI0_INX, tx_array, sizeof(tx_array)) && res;
    res = spi_read(SPI0_INX, (uint8_t*)&read_reg, 4) && res;
    *out_reg = reverse_byte_order_uint32(read_reg);
    return res;
}

bool tcan4550_read_reg(uint16_t address, uint32_t* out_reg) {
    bool res = true;
    if(NULL != out_reg) {
        TCAN4550_CHIP_SELECT(tcan4550_read_reg_proc(address, out_reg));
    } else {
        res = false;
    }
    return res;
}

static bool tcan4550_write_reg_proc(uint16_t address, uint32_t reg_val) {
    bool res = true;
    uint8_t tx_array[8];
    res = init_spi_header((HeaderCom_t*)tx_array, OP_CODE_WRITE, address, 1);
    uint32_t tx_reg_val = reverse_byte_order_uint32(reg_val);
    memcpy(&tx_array[4], &tx_reg_val, 4);
    res = spi_write(SPI0_INX, tx_array, sizeof(tx_array)) && res;
    return res;
}

bool tcan4550_write_reg(uint16_t address, uint32_t reg_val) {
    bool res = true;
    TCAN4550_CHIP_SELECT(tcan4550_write_reg_proc(address, reg_val));
    return res;
}

bool tcan4550_write_reg_lazy(uint16_t address, uint32_t reg_val) {
    bool res = false;
    return res;
}

bool tcan4550_clear_mram(void) {
    bool res = true;
    uint16_t curAddr = ADDR_MRAM;
    for(curAddr = ADDR_MRAM; curAddr < (ADDR_MRAM + MRAM_SIZE); curAddr += 4) {
        res = tcan4550_write_reg(curAddr, 0) && res;
    }
    return res;
}

bool tcan4550_send(uint16_t id, uint64_t data) {
    bool res = false;
    return res;
}

DevMode_t tcan4550_get_mode(void) {
    DevMode_t dev_mode = MODE_UNDEF;
    bool res = false;
    tCanRegModeOpPinCfg_t read_reg;
    read_reg.word=0;
    res = tcan4550_read_reg(ADDR_DEV_CONFIG, &read_reg.word);
    if(res) {
        dev_mode = read_reg.mode_sel;
    }
    return dev_mode;
}

bool tcan4550_set_mode(DevMode_t dev_mode) {

    bool res = false;
    tCanRegModeOpPinCfg_t read_reg;
    read_reg.word=0;
    res = tcan4550_read_reg(ADDR_DEV_CONFIG, &read_reg.word);
    if(res) {
        read_reg.mode_sel = dev_mode;
        res= tcan4550_write_reg(  ADDR_DEV_CONFIG,   read_reg.word) ;
    }
    return res;
}
