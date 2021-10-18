#include "tcan4550_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bit_utils.h"
#include "byte_utils.h"
#include "clocks.h"
#include "data_utils.h"
#include "debug_info.h"
#include "log.h"
#include "spi_drv.h"
#include "sys_config.h"

const uint64_t exp_dev_id = 0x343535305443414E;

const Tcan4550Reg_t tCan4550RegLUT[] = {
    {ADDR_IR, "IntReg"},           {ADDR_IE, "IntEn"},           {ADDR_IF, "IntFlgs"},
    {ADDR_MCAN_NBTP, "BitTiming"}, {ADDR_DEV_CONFIG, "DevCfg"},  {ADDR_DEVICE_ID0, "DevId0"},
    {ADDR_DEVICE_ID1, "DevId1"},   {ADDR_SPI_2_REV, "SPIrev"},   {ADDR_STATUS, "Status"},
    {ADDR_CREL, "CREL"},           {ADDR_MCAN_TXBC, "TxBufCfg"}, {ADDR_MCAN_TXESC, "TxBufElSzCfg"},
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

bool tcan4550_send_spi_header(uint8_t opcode, uint16_t address, uint8_t words) {
    bool res = true;
    uint8_t tx_array[4];
    res = init_spi_header((HeaderCom_t*)tx_array, opcode, address, words);
    res = spi_write(SPI0_INX, tx_array, sizeof(tx_array)) && res;
    return res;
}

static bool tcan4550_read_reg_proc(uint16_t address, uint32_t* out_reg) {
    bool res = true;
    // see page 44 Figure 8-18. Read (Command OpCode 8h41)
    uint32_t read_reg;
    res = tcan4550_send_spi_header(OP_CODE_READ, address, 1);
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

bool tcan4550_send_spi_burst(uint32_t word) {
    bool res = true;
    uint32_t tx_word = reverse_byte_order_uint32(word);
    res = spi_write(SPI0_INX, (uint8_t*)&tx_word, sizeof(tx_word)) && res;
    return res;
}

static bool tcan4550_write_reg_proc(uint16_t address, uint32_t reg_val) {
    bool res = true;
    res = tcan4550_send_spi_header(OP_CODE_WRITE, address, 1);
    res = tcan4550_send_spi_burst(reg_val) && res;
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
    for(curAddr = ADDR_MRAM; curAddr < (ADDR_MRAM + MRAM_SIZE - 4); curAddr += 4) {
        res = tcan4550_write_reg(curAddr, 0) && res;
    }
    return res;
}

/**
 * @brief Clears a SPIERR flag that may be set
 */
bool tcan4550_clear_spi_err(void) {
    bool res = false;
    // Simply write all 1s to attempt to clear a SPIERR that was set
    res = tcan4550_write_reg(ADDR_SPI_STATUS, 0xFFFFFFFF);
    return res;
}

DevMode_t tcan4550_get_mode(void) {
    DevMode_t dev_mode = MODE_UNDEF;
    bool res = false;
    tCanRegModeOpPinCfg_t read_reg;
    read_reg.word = 0;
    res = tcan4550_read_reg(ADDR_DEV_CONFIG, &read_reg.word);
    if(res) {
        dev_mode = (DevMode_t)read_reg.mode_sel;
    }
    return dev_mode;
}

bool tcan4550_set_mode(DevMode_t dev_mode) {
    bool res = false;
    tCanRegModeOpPinCfg_t read_reg;
    read_reg.word = 0;
    res = tcan4550_read_reg(ADDR_DEV_CONFIG, &read_reg.word);
    if(res) {
        read_reg.mode_sel = dev_mode;
        res = tcan4550_write_reg(ADDR_DEV_CONFIG, read_reg.word);
    }
    return res;
}

/**
 * @brief Converts the ESC (Element Size) value to number of bytes that it corresponds to
 *
 * @param inputESCValue is the value from an element size configuration register
 * @return The number of bytes of data (8-64 bytes)
 */
static const uint8_t lookUpTable[8] = {8, 12, 16, 20, 24, 32, 48, 64};
uint8_t txrxesc_2data_bytes(uint8_t code) { return lookUpTable[code & 0x07]; }

/**
 * @brief Converts the CAN message DLC hex value to the number of bytes it corresponds to
 *
 * @param inputDLC is the DLC value from/to a CAN message struct
 * @return The number of bytes of data (0-64 bytes)
 */
static const uint8_t dlcLUT_9[7] = {12, 16, 20, 24, 32, 48, 64};
uint8_t dlc_2_bytes(uint8_t dlc_code) {
    uint8_t size = 0;
    if(dlc_code < 9) {
        size = dlc_code;
    } else if(dlc_code < 16) {
        size = dlcLUT_9[(dlc_code - 9)];
    } else {
        size = 0;
    }
    return size;
}

bool tcan4550_write_tx_buff(uint8_t buf_index, tCanTxHeader_t* header, uint8_t* data_payload) {
    bool res = false;
    tCanRegTxBufCfg_t read_reg;
    read_reg.word = 0;
    uint16_t start_address = 0;
    res = tcan4550_read_reg(ADDR_MCAN_TXBC, &read_reg.word);
    if(res) {
        res = false;
        start_address = ADDR_MRAM + read_reg.tx_buf_start_addr;
        if(32 < read_reg.tfqs) {
            read_reg.tfqs = 32;
        }

        if(32 < read_reg.ndtb) {
            read_reg.ndtb = 32;
        }
        if(buf_index <= (read_reg.ndtb - 1)) {

        } else {
            res = false;
        }
    }

    uint8_t element_size = 0;
    if(res) {
        tCanRegTxBufElmSzCfg_t reg;
        reg.word = 0;
        res = tcan4550_read_reg(ADDR_MCAN_TXESC, &reg.word);
        element_size = txrxesc_2data_bytes(reg.tx_buff_data_size) + 8;
        start_address += ((uint32_t)element_size * buf_index);
        // Convert it to words for easier reading by dividing by 4, and only look at data payload
        element_size = (dlc_2_bytes(header->dlc & 0x0F) + 8) >> 2;
        if(0 < dlc_2_bytes(header->dlc & 0x0F) % 4) { // If we don't have a whole word worth of data...
            // We need to round up to the nearest word (by default it truncates).
            // Can be done by simply adding another word.
            element_size += 1;
        }
        tcan4550_chip_select(true);
        delay_ms(1);
        res = tcan4550_send_spi_header(OP_CODE_WRITE, start_address, element_size);
        W0_t reg_w0;
        reg_w0.word = 0;
        reg_w0.esi = header->esi;
        reg_w0.xtd = header->xtd;
        reg_w0.rtr = header->rtr;
        if(header->xtd) {
            reg_w0.id = header->id & MASK_29BIT;
        } else {
            reg_w0.id = ((uint32_t)header->id & MASK_11BIT) << 18;
        }
        tcan4550_send_spi_burst(reg_w0.word);
        // see Figure 12. Tx FIFO / Buffer Element
        TxBuffW1_t reg_w1;
        reg_w1.word = 0;
        reg_w1.dlc = header->dlc;
        reg_w1.brs = header->brs;
        reg_w1.fdf = header->fdf;
        reg_w1.efc = header->efc;
        reg_w1.mm = header->mm;
        tcan4550_send_spi_burst(reg_w1.word);

        element_size = dlc_2_bytes(header->dlc & 0x0F); // Returns the number of data bytes
        Type32Union_t un32;
        un32.u32 = 0;
        uint16_t i = 0;
        for(i = 0; i < element_size; i += 4) {
            un32.u8[0] = data_payload[i];
            un32.u8[1] = data_payload[i + 1];
            un32.u8[2] = data_payload[i + 2];
            un32.u8[3] = data_payload[i + 3];
            res = tcan4550_send_spi_burst(un32.u32);
        }
        delay_ms(1);
        tcan4550_chip_select(false);
    }

    return res;
}

bool is_tcan4550_protected_reg_locked(tCanRegCCctrl_t* ctrl_reg) {
    bool res = false;
    tCanRegCCctrl_t reg;
    reg.word = 0;
    res = tcan4550_read_reg(ADDR_MCAN_CCCR, &reg.word);
    if(true == res) {
        *ctrl_reg = reg;
        res = false;
        if(0 == reg.cce) {
            res = true;
        }
    }
    return res;
}

/**
 * @brief Disable Protected MCAN Registers
 *
 * Attempts to disable CCCR.CCE and CCCR.INIT to disallow writes to protected registers
 *
 * @return @c true if successfully enabled, otherwise return @c false
 */
bool tcan4550_protected_registers_lock(void) {
    bool res = false;
    tCanRegCCctrl_t reg;
    reg.word = 0;

    res = is_tcan4550_protected_reg_locked(&reg);

    if(false == res) {
        uint8_t i;
        for(i = 0; i < 5; i++) {
            reg.csr = 0;
            reg.csa = 0;
            reg.cce = 0;
            reg.init = 0;
            res = tcan4550_write_reg(ADDR_MCAN_CCCR, reg.word);
            if(res) {
                res = is_tcan4550_protected_reg_locked(&reg);
                if(res) {
                    break;
                }
            }
        }
    }

    return res;
}

bool tcan4550_tx_buff_content(uint8_t buf_index) {
    bool res = false;
    if(buf_index < 31U) {
        uint32_t write_value = 0;
        res = tcan4550_read_reg(ADDR_MCAN_TXBAR, &write_value);
        if(res) {
            write_value |= 1U << buf_index;
            res = tcan4550_write_reg(ADDR_MCAN_TXBAR, write_value);
        }
    }
    return res;
}

bool tcan4550_send(uint16_t id, uint64_t data) {
    bool res = false;
    tCanTxHeader_t header;
    memset(&header, 0x00, sizeof(header));
    header.dlc = 8;
    header.xtd = 0;
    header.id = id;
    header.fdf = 0;
    res = tcan4550_write_tx_buff(0, &header, (uint8_t*)&data);
    if(res) {
        res = tcan4550_tx_buff_content(0);
    }
    return res;
}

/**
 * @brief Writes the MCAN nominal timing settings, using the simple nominal timing struct
 *
 * Writes the data timing information to MCAN using the input from the @c *nomTiming pointer
 *
 * @warning This function writes to protected MCAN registers
 * @note Requires that protected registers have been unlocked using @c TCAN4x5x_MCAN_EnableProtectedRegisters() and @c
 * TCAN4x5x_MCAN_DisableProtectedRegisters() be used to lock the registers after configuration
 *
 * @param *nomTiming is a pointer of a @c TCAN4x5x_MCAN_Nominal_Timing_Simple struct containing the simplified nominal
 * timing information
 * @return @c true if successfully enabled, otherwise return @c false
 */
bool tcan4550_configure_timing_simple(tCanRegBitTime_t* bit_time) {
    bool res = false;
    res = tcan4550_write_reg(ADDR_MCAN_NBTP, bit_time->word);
    return res;
}

bool tcan4550_configure_data_timing_simple(tCanRegDataBitTime_t* data_time) {
    bool res = false;
    res = tcan4550_write_reg(ADDR_MCAN_DBTP, data_time->word);
    return res;
}

bool tcan4550_configure_interrupt(tCanRegIntEn_t* ie) {
    bool res = tcan4550_write_reg(ADDR_IE, ie->word);
    return res;
}

/**
 * @brief Read the device interrupts
 *
 * Reads the device interrupts and updates a @c TCAN4x5x_Device_Interrupts struct that is passed to the function
 *
 * @param *ir is a pointer to a @c TCAN4x5x_Device_Interrupts struct containing the interrupt bit fields that will be
 * updated
 */
bool tcan4550_read_interrupt(tCanRegIntFl_t* ir) {
    bool res = false;
    res = tcan4550_read_reg(ADDR_IF, &ir->word);
    return res;
}

/**
 * @brief Clear the device interrupts
 *
 * Will attempt to clear any interrupts that are marked as a '1' in the passed @c TCAN4x5x_Device_Interrupts struct
 *
 * @param *ir is a pointer to a @c TCAN4x5x_Device_Interrupts struct containing the interrupt bit fields that will be
 * updated
 */
bool tcan4550_read_clear_interrupt(tCanRegIntFl_t* ir) {
    bool res = false;
    res = tcan4550_write_reg(ADDR_IF, ir->word);
    return res;
}

bool is_tcan4550_protected_reg_unlock(tCanRegCCctrl_t* reg) {
    bool res = false;
    tCanRegCCctrl_t ctr_reg;
    ctr_reg.word = 0;
    res = tcan4550_read_reg(ADDR_MCAN_CCCR, &ctr_reg.word);
    if(res) {
        *reg = ctr_reg;
        if((1 == ctr_reg.init) && (1 == ctr_reg.cce)) {
            res = true;
        }
    }
    return res;
}

/**
 * @brief Enable Protected MCAN Registers
 *
 * Attempts to enable CCCR.CCE and CCCR.INIT to allow writes to protected registers, needed for MCAN configuration
 *
 * @return @c true if successfully enabled, otherwise return @c false
 */
bool tcan4550_protected_registers_unlock(void) {
    uint8_t i = 0;
    bool res = false;
    tCanRegCCctrl_t ctr_reg;
    ctr_reg.word = 0;

    res = is_tcan4550_protected_reg_unlock(&ctr_reg);
    if(true == res) {
        res = true;
    }

    // Try up to 5 times to set the CCCR register, if not, then fail config, since we need these bits set to configure
    // the device.
    for(i = 0; i < 5; i++) {
        // Unset the CSA and CSR bits since those will be set if we're in standby mode.
        // Writing a 1 to these bits will force a clock stop event and prevent the return to normal mode
        ctr_reg.csa = 0;
        ctr_reg.csr = 0;
        ctr_reg.cce = 1;
        ctr_reg.init = 1;
        res = tcan4550_write_reg(ADDR_MCAN_CCCR, ctr_reg.word);
        if(res) {
            res = is_tcan4550_protected_reg_unlock(&ctr_reg);
            if(true == res) {
                break;
            }
        }
    }
    return res;
}

/**
 * @brief Configures the MCAN global filter configuration register, using the passed Global Filter Configuration struct.
 *
 * Configures the default behavior of the MCAN controller when receiving messages. This can include accepting or
 * rejecting CAN messages by default.
 *
 * @warning This function writes to protected MCAN registers
 * @note Requires that protected registers have been unlocked using @c TCAN4x5x_MCAN_EnableProtectedRegisters() and @c
 * TCAN4x5x_MCAN_DisableProtectedRegisters() be used to lock the registers after configuration
 *
 * @param *gfc is a pointer of a @c TCAN4x5x_MCAN_Global_Filter_Configuration struct containing the register values
 * @return @c true if successfully enabled, otherwise return @c false
 */
bool tcan4550_configure_global_filter(tCanRegGloFiltCfg_t* gfc) {
    bool res = false;
    res = tcan4550_write_reg(ADDR_MCAN_GFC, gfc->word);

#ifdef TCAN4x5x_MCAN_VERIFY_CONFIGURATION_WRITES
#endif
    return res;
}

bool tcan4550_configure_mcan_interrupt(tCanRegIntEn_t *mcan_ie){
    bool res = true;
    res = tcan4550_write_reg(ADDR_MCAN_IE, mcan_ie->word)&& res;
    tCanRegIntLine_t it_reg;
    it_reg.word=0;
    it_reg.eint0=1;
    // This is necessary to enable the MCAN Int mux to the output nINT pin
    res = tcan4550_write_reg(ADDR_MCAN_ILE, it_reg.word)&& res;
    return res;
}

bool tcan4550_device_configure(tCanRegModeOpPinCfg_t *dev_cfg){
    bool res = true;
    return res;
}

bool tcan4550_init(void) {
    bool res = true;
    GPIO_writeDio(DIO_CAN_SS, 1);
    GPIO_writeDio(DIO_CAN_RST, 1);

    res = tcan4550_reset();

    res = is_tcan4550_connected();
    if(res) {
        res = tcan4550_clear_spi_err() && res;
        tCanRegIntEn_t ie;
        ie.word = 0;
        res = tcan4550_configure_interrupt(&ie) && res;

        tCanRegIntFl_t dev_ir;
        dev_ir.word = 0;
        res = tcan4550_read_interrupt(&dev_ir) && res;
        if(res) {
            if(1 == dev_ir.pwron) {
                res = tcan4550_read_clear_interrupt(&dev_ir);
            }
        }

        tCanRegBitTime_t reg_bit_time;
        reg_bit_time.word = 0;
        reg_bit_time.nbrp = 2;
        reg_bit_time.ntseg1 = 32;
        reg_bit_time.ntseg2 = 8;

        tCanRegDataBitTime_t data_time;
        data_time.word = 0;
        data_time.dtseg1 = 15;
        data_time.dtseg2 = 5;
        data_time.dbrp = 1;

        tCanRegGloFiltCfg_t gfc;
        gfc.word = 0;
        gfc.rrfe = 1;
        gfc.rrfs = 1;
        gfc.anfe = 0;
        gfc.anfs = 0;

        res = tcan4550_protected_registers_unlock() && res;

        res = tcan4550_configure_timing_simple(&reg_bit_time) && res;
        res = tcan4550_configure_data_timing_simple(&data_time) && res; // Setup CAN FD timing
        res = tcan4550_configure_global_filter(&gfc) && res;

        res = tcan4550_clear_mram() && res;

        res = tcan4550_protected_registers_lock() && res;


        tCanRegIntEn_t mcan_ie;
        mcan_ie.word = 0;
        mcan_ie.rf0ne = 1;

        res = tcan4550_configure_mcan_interrupt(&mcan_ie) && res;

        tCanRegModeOpPinCfg_t dev_cfg_reg;
        dev_cfg_reg.word=0;
        dev_cfg_reg.device_reset = 0;
        dev_cfg_reg.clk_ref = 1;
        dev_cfg_reg.wd_en = 0;
        res = tcan4550_device_configure(&dev_cfg_reg) &&res;

        // Set to normal mode, since configuration is done. This line turns on the transceiver
        res = tcan4550_set_mode(MODE_NORMAL)&&res;

        res = tcan4550_write_reg(ADDR_MCAN_IR, 0xFFFFFFFF)&&res;


    }
    return res;
}
