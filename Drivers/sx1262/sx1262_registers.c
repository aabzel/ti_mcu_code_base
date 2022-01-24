#include "sx1262_registers.h"

#include <stdlib.h>
#include <string.h>

#include "byte_utils.h"
#include "data_utils.h"
#include "sx1262_drv.h"
#include "spi_drv.h"
#include "none_blocking_pause.h"

/*TODO: rewrite*/
#define READ_REG_HEADER_SZ 3
static bool sx1262_read_reg_proc(uint16_t reg_addr, uint8_t* reg_val) {
    bool res = false;
    if(NULL != reg_val) {
        res = true;
        *reg_val = 0xFF;
        uint8_t tx_array[4] = {0};
        memset(tx_array, 0x00, sizeof(tx_array));
        Type16Union_t temp_reg_val;
        temp_reg_val.u16 = 0;
        tx_array[0] = OPCODE_READ_REGISTER;
        uint16_t reg_addr_nbo = reverse_byte_order_uint16(reg_addr);
        memcpy(&tx_array[1], &reg_addr_nbo, sizeof(reg_addr_nbo));
        res = spi_write((SpiName_t)SX1262_SPI_NUM, tx_array, READ_REG_HEADER_SZ) && res;
        res = spi_read((SpiName_t)SX1262_SPI_NUM, (uint8_t*)&temp_reg_val.u16, sizeof(temp_reg_val.u16)) && res;
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

/*
  WriteRegister

  The command WriteRegister(...) allows writing a block of bytes in a data memory
  space starting at a specific address. The address is auto incremented after each
  data byte so that data is stored in contiguous memory locations. The SPI data
  transfer is described in the following table.
*/



#define WR_REG_PAYLOAD_SZ (3)
bool sx1262_write_reg(uint16_t reg_addr, uint8_t reg_val) {
    bool res = true;
    uint8_t tx_array[WR_REG_PAYLOAD_SZ];
    memset(tx_array, 0x00, sizeof(tx_array));
    uint16_t reg_addr_be = reverse_byte_order_uint16(reg_addr);
    memcpy(&tx_array[0], &reg_addr_be, sizeof(reg_addr_be));
    tx_array[2] = reg_val;
    res = sx1262_send_opcode(OPCODE_WRITE_REGISTER, tx_array, sizeof(tx_array), NULL, 0);
    return res;
}

bool sx1262_get_rand(uint32_t* rand_num) {
    bool res = true;
    if(rand_num) {
        res = true;
        Type32Union_t var32bit = {0};
        res = sx1262_read_reg(RAND_NUM_GEN_0, &var32bit.u8[0]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_1, &var32bit.u8[1]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_2, &var32bit.u8[2]) && res;
        res = sx1262_read_reg(RAND_NUM_GEN_3, &var32bit.u8[3]) && res;
        *rand_num = var32bit.u32;
    } else {
        res = false;
    }
    return res;
}

bool sx1262_set_lora_sync_word(uint16_t sync_word) {
    bool res = true;
    Type16Union_t var16bit;
    var16bit.u16 = sync_word;
    var16bit.u16 = reverse_byte_order_uint16(var16bit.u16);
    res = sx1262_write_reg(LORA_SYNC_WORD_MSB, var16bit.u8[0]) && res;
    res = sx1262_write_reg(LORA_SYNC_WORD_LSB, var16bit.u8[1]) && res;
    return res;
}

bool sx1262_get_sync_word(uint64_t* sync_word) {
    bool res = true;
    if(sync_word) {
        res = true;
        Type64Union_t var64bit = {0};
        res = sx1262_read_reg(SYNC_WORD_0, &var64bit.u8[0]) && res;
        res = sx1262_read_reg(SYNC_WORD_1, &var64bit.u8[1]) && res;
        res = sx1262_read_reg(SYNC_WORD_2, &var64bit.u8[2]) && res;
        res = sx1262_read_reg(SYNC_WORD_3, &var64bit.u8[3]) && res;
        res = sx1262_read_reg(SYNC_WORD_4, &var64bit.u8[4]) && res;
        res = sx1262_read_reg(SYNC_WORD_5, &var64bit.u8[5]) && res;
        res = sx1262_read_reg(SYNC_WORD_6, &var64bit.u8[6]) && res;
        res = sx1262_read_reg(SYNC_WORD_7, &var64bit.u8[7]) && res;
        *sync_word = reverse_byte_order_uint64(var64bit.u64);
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_lora_sync_word(uint16_t* sync_word) {
    bool res = true;
    if(sync_word) {
        res = true;
        Type16Union_t var16bit = {0};
        res = sx1262_read_reg(LORA_SYNC_WORD_LSB, &var16bit.u8[1]) && res;
        res = sx1262_read_reg(LORA_SYNC_WORD_MSB, &var16bit.u8[0]) && res;

        *sync_word = reverse_byte_order_uint16(var16bit.u16);
    } else {
        res = false;
    }
    return res;
}



bool sx1262_set_sync_word(uint64_t sync_word) {
    bool res = true;
    Type64Union_t var64bit;
    var64bit.u64 = sync_word;
    var64bit.u64 = reverse_byte_order_uint64(var64bit.u64);
    res = sx1262_write_reg(SYNC_WORD_0, var64bit.u8[0]) && res;
    res = sx1262_write_reg(SYNC_WORD_1, var64bit.u8[1]) && res;
    res = sx1262_write_reg(SYNC_WORD_2, var64bit.u8[2]) && res;
    res = sx1262_write_reg(SYNC_WORD_3, var64bit.u8[3]) && res;
    res = sx1262_write_reg(SYNC_WORD_4, var64bit.u8[4]) && res;
    res = sx1262_write_reg(SYNC_WORD_5, var64bit.u8[5]) && res;
    res = sx1262_write_reg(SYNC_WORD_6, var64bit.u8[6]) && res;
    res = sx1262_write_reg(SYNC_WORD_7, var64bit.u8[7]) && res;
    return res;
}


bool sx1262_set_rx_gain(RxGain_t rx_gain){
    bool res = false;
    res = sx1262_write_reg(RX_GAIN, (uint8_t) rx_gain);
    return res;
}

bool sx1262_get_rx_gain(RxGain_t *rx_gain){
    bool res = false;
    if(rx_gain) {
        res = sx1262_read_reg(RX_GAIN,(uint8_t *) rx_gain);
    }
    return res;
}


bool sx1262_set_crc_poly(uint16_t polynomial){
    bool res = true;
    Type16Union_t var16bit;
    var16bit.u16 = polynomial;
    var16bit.u16 = reverse_byte_order_uint16(var16bit.u16);
    res = sx1262_write_reg(CRC_POLYNOMIAL_MSB, var16bit.u8[0]) && res;
    res = sx1262_write_reg(CRC_POLYNOMIAL_LSB, var16bit.u8[1]) && res;
    return res;
}

bool sx1262_set_crc_seed(uint16_t seed){
    bool res = true;
    Type16Union_t var16bit;
    var16bit.u16 = seed;
    var16bit.u16 = reverse_byte_order_uint16(var16bit.u16);
    res = sx1262_write_reg(CRC_INIT_MSB, var16bit.u8[0]) && res;
    res = sx1262_write_reg(CRC_INIT_LSB, var16bit.u8[1]) && res;
    return res;
}

bool sx1262_get_crc_poly(uint16_t *polynomial){
    bool res = false;
    if(polynomial){
        res = true;
        Type16Union_t var16bit = {0};
        res = sx1262_read_reg(CRC_POLYNOMIAL_LSB, &var16bit.u8[1]) && res;
        res = sx1262_read_reg(CRC_POLYNOMIAL_MSB, &var16bit.u8[0]) && res;

        *polynomial = reverse_byte_order_uint16(var16bit.u16);
    }
    return res;
}

bool sx1262_get_crc_seed(uint16_t *seed){
    bool res = false;
    if(seed){
        res = true;
        Type16Union_t var16bit = {0};
        res = sx1262_read_reg(CRC_INIT_LSB, &var16bit.u8[1]) && res;
        res = sx1262_read_reg(CRC_INIT_MSB, &var16bit.u8[0]) && res;

        *seed = reverse_byte_order_uint16(var16bit.u16);
    }
    return res;
}
