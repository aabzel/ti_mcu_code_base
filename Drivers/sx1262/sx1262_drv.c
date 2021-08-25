/*
Motorola byte order
CPOL= 0 and CPHA = 0 in Motorola/Freescale nomenclature.
network byte order , from senior byte to junior byte
speed up to 16 MHz
*/
#include "sx1262_drv.h"

#include <gpio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bit_utils.h"
#include "byte_utils.h"
#include "data_utils.h"
#include "gpio_drv.h"
#include "none_blocking_pause.h"
#include "spi_drv.h"

#define SX1262_CHIP_SELECT(CALL_BACK)                                                                                  \
    do {                                                                                                               \
        res = false;                                                                                                   \
        uint32_t busy_value = GPIO_readDio(SX1262_BUSY_DIO_NO);                                                        \
        if(0 == busy_value) {                                                                                          \
            res = true;                                                                                                \
            GPIO_writeDio(SX1262_SS_DIO_NO, 0);                                                                        \
            wait_us(110);                                                                                              \
            res = CALL_BACK;                                                                                           \
            wait_us(2);                                                                                                \
            GPIO_writeDio(SX1262_SS_DIO_NO, 1);                                                                        \
        } else {                                                                                                       \
            res = false;                                                                                               \
        }                                                                                                              \
    } while(0);

ChipMode_t sx1262_chip_mode;

const xSx1262Reg_t RegMap[SX1262_REG_CNT] = {{0x06B8, "WhiteningInitValMSB"},
                                             {0x06B9, "WhiteningInitValLSB"},
                                             {0x06BC, "CRCMSBInitVal0"},
                                             {0x06BD, "CRCLSBInitVal1"},
                                             {0x06BE, "CRCMSBpolynomVal0"},
                                             {0x06BF, "CRCLSBpolynomVal1"},
                                             {0x06C0, "SyncWord0"},
                                             {0x06C1, "SyncWord1"},
                                             {0x06C2, "SyncWord2"},
                                             {0x06C3, "SyncWord3"},
                                             {0x06C4, "SyncWord4"},
                                             {0x06C5, "SyncWord5"},
                                             {0x06C6, "SyncWord6"},
                                             {0x06C7, "SyncWord7"},
                                             {0x06CD, "NodeAddr"},
                                             {0x06CE, "BroadcastAddr"},
                                             {0x0740, "LoRaSyncWordMSB"},
                                             {0x0741, "LoRaSyncWordLSB"},
                                             {0x0819, "RandNumGen0"},
                                             {0x081A, "RandNumGen1"},
                                             {0x081B, "RandNumGen2"},
                                             {0x081C, "RandNumGen3"},
                                             {0x08AC, "RxGain"},
                                             {0x08E7, "OCPConfig"},
                                             {0x0911, "XTAtrim"},
                                             {0x0912, "XTBtrim"}};

static bool sx1262_init_gpio(void) {
    bool res = true;
    GPIO_setConfig(CONFIG_GPIO_LORA_RST, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONFIG_GPIO_LORA_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONFIG_GPIO_LORA_INT, GPIO_CFG_IN_NOPULL);
    GPIO_setConfig(CONFIG_GPIO_LORA_BSY, GPIO_CFG_IN_NOPULL);

    GPIO_writeDio(SX1262_SS_DIO_NO, 1);
    GPIO_writeDio(SX1262_RST_DIO_NO, 1);

    return res;
}

static bool sx1262_write_reg_proc(uint16_t reg_addr, uint8_t reg_val) {
    bool res = true;

    uint8_t tx_array[4];
    memset(tx_array, 0x00, sizeof(tx_array));

    tx_array[0] = OPCODE_WRITE_REGISTER;
    uint16_t reg_addr_nbo = reverse_byte_order_uint16(reg_addr);
    memcpy(&tx_array[1], &reg_addr_nbo, sizeof(reg_addr_nbo));
    tx_array[3] = reg_val;
    res = spi_write(SX1262_SPI_NUM, tx_array, sizeof(tx_array)) && res;

    return res;
}

bool sx1262_write_reg(uint16_t reg_addr, uint8_t reg_val) {
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_write_reg_proc(reg_addr, reg_val));
    return res;
}

#define READ_REG_HEADER_SZ 3
static bool sx1262_read_reg_proc(uint16_t reg_addr, uint8_t* reg_val) {
    bool res = false;
    if(NULL != reg_val) {
        res = true;
        *reg_val = 0xFF;
        uint8_t tx_array[4];
        memset(tx_array, 0x00, sizeof(tx_array));
        Type16Union_t temp_reg_val;
        temp_reg_val.u16 = 0;
        tx_array[0] = OPCODE_READ_REGISTER;
        uint16_t reg_addr_nbo = reverse_byte_order_uint16(reg_addr);
        memcpy(&tx_array[1], &reg_addr_nbo, sizeof(reg_addr_nbo));
        res = spi_write(SX1262_SPI_NUM, tx_array, READ_REG_HEADER_SZ) && res;
        res = spi_read(SX1262_SPI_NUM, (uint8_t*)&temp_reg_val.u16, sizeof(temp_reg_val.u16)) && res;
        temp_reg_val.u16 = reverse_byte_order_uint16(temp_reg_val.u16);
        *reg_val = temp_reg_val.u8[0];
    } else {
        res = false;
    }

    return res;
}

bool sx1262_read_reg(uint16_t reg_addr, uint8_t* reg_val) {
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_read_reg_proc(reg_addr, reg_val));
    return res;
}

static bool sx1262_send_opcode_proc(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                                    uint16_t rx_array_len) {
    bool res = true;

    uint8_t tempTxArray[tx_array_len + OPCODE_SIZE];
    tempTxArray[0] = op_code;
    uint16_t temp_tx_arr_len = tx_array_len + OPCODE_SIZE;

    if((NULL != tx_array) && (0 == tx_array_len)) {
        memcpy(&tempTxArray[1], &tx_array, temp_tx_arr_len);
    }

    res = spi_write(SX1262_SPI_NUM, tempTxArray, temp_tx_arr_len) && res;
    if((0 < rx_array_len) && (NULL != rx_array)) {
        res = spi_read(SX1262_SPI_NUM, rx_array, rx_array_len) && res;
    }

    return res;
}

bool sx1262_send_opcode(uint8_t op_code, uint8_t* tx_array, uint16_t tx_array_len, uint8_t* rx_array,
                        uint16_t rx_array_len) {
    bool res = false;
    SX1262_CHIP_SELECT(sx1262_send_opcode_proc(op_code, tx_array, tx_array_len, rx_array, rx_array_len));
    return res;
}

bool sx1262_set_rffrequency(float rf_frequency_mhz) {
    bool res = false;
    uint32_t frf_code = ((uint32_t)rf_frequency_mhz) * MHZ_TO_FRF;
    uint8_t buff[4];
    Type32Union_t u32val;
    u32val.f32 = frf_code;
    u32val.u32 = reverse_byte_order_uint32(u32val.u32);
    memcpy(buff, &u32val.u32, sizeof(u32val.u32));
    res = sx1262_send_opcode(OPCODE_SET_RF_FREQUENCY, buff, sizeof(buff), NULL, 0);
    return res;
}

bool sx1262_start_rx(uint32_t timeout_s) {
    bool res = true;
    uint8_t buff[3];
    /*from senior byte to junior byte*/
    buff[0] = MASK_8BIT & (timeout_s >> 16);
    buff[1] = MASK_8BIT & (timeout_s >> 8);
    buff[2] = MASK_8BIT & (timeout_s);

    res = sx1262_send_opcode(OPCODE_SET_RX, buff, sizeof(buff), NULL, 0) && res;

    sx1262_chip_mode = CHIPMODE_RX;

    return res;
}

bool sx1262_init(void) {
    bool res = true;
    res = sx1262_init_gpio() && res;
    res = sx1262_set_rffrequency(868) && res;
    sx1262_chip_mode = CHIPMODE_NONE;
    res = sx1262_start_rx(60) && res;
    return res;
}

bool SX126x_start_tx(uint8_t offset, uint8_t* tx_buf, uint8_t pkt_len, uint32_t timeout_s) {
    bool res = true;
    uint8_t buff[pkt_len + 1];
    buff[0] = offset;
    memcpy(&buff[1], tx_buf, pkt_len);
    res = sx1262_send_opcode(OPCODE_WRITE_BUFFER, buff, pkt_len + 1, NULL, 0) && res;

    /*from senior byte to junior byte*/
    buff[0] = MASK_8BIT & (timeout_s >> 16);
    buff[1] = MASK_8BIT & (timeout_s >> 8);
    buff[2] = MASK_8BIT & (timeout_s);
    res = sx1262_send_opcode(OPCODE_SET_TX, buff, 3, NULL, 0) && res;
    sx1262_chip_mode = CHIPMODE_TX;
    return res;
}
