/*

CPOL= 0 and CPHA = 0 in Motorola/Freescale nomenclature.
Bytes follow from senior byte to junior byte (Motorola byte order or network byte order)
speed up to 16 MHz
*/
#include "sx1262_drv.h"

#include <gpio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bit_utils.h"
#include "byte_utils.h"
#include "clocks.h"
#include "data_utils.h"
#include "gpio_drv.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "spi_drv.h"

#define SX1262_CHIP_SELECT(CALL_BACK)                                                                                  \
    do {                                                                                                               \
        res = false;                                                                                                   \
        res = sx1262_wait_on_busy(500);                                                                                \
        if(true == res) {                                                                                              \
            res = true;                                                                                                \
            GPIO_writeDio(SX1262_SS_DIO_NO, 0);                                                                        \
            wait_us(110);                                                                                              \
            res = CALL_BACK;                                                                                           \
            wait_us(2);                                                                                                \
            GPIO_writeDio(SX1262_SS_DIO_NO, 1);                                                                        \
        } else {                                                                                                       \
            Sx1262Instance.busy_cnt++;                                                                                 \
            res = false;                                                                                               \
        }                                                                                                              \
    } while(0);

#define GET_4_BYTE_OPCODE(OP_CODE, OUT_VAL_16_BIT)                                                                       \
    do {                                                                                                               \
        if(OUT_VAL_16_BIT) {                                                                                             \
            uint8_t rx_array[4];                                                                                       \
            memset(rx_array, 0x00, sizeof(rx_array));                                                                  \
            res = sx1262_send_opcode(OP_CODE, NULL, 0, rx_array, sizeof(rx_array));                                    \
            if(res) {                                                                                                  \
                Sx1262Instance.status = rx_array[1];                                                                   \
                *OUT_VAL_16_BIT = copy_and_rev16(&rx_array[2]);                                                          \
            }else{                                  \
                *OUT_VAL_16_BIT = 0xFFFF;   \
            }                                                                                                          \
        }                                                                                                              \
    } while(0);

Sx1262_t Sx1262Instance;

const xSx1262Reg_t RegMap[SX1262_REG_CNT] = {{0x06B8, "WhiteningInitValMSB"},
                                             {0x06B9, "WhiteningInitValLSB"},
                                             {0x06BC, "CRCMSBInitVal0"},
                                             {0x06BD, "CRCLSBInitVal1"},
                                             {0x06BE, "CRCMSBpolynomVal0"},
                                             {0x06BF, "CRCLSBpolynomVal1"},
                                             {SYNC_WORD_0, "SyncWord0"},
                                             {SYNC_WORD_1, "SyncWord1"},
                                             {SYNC_WORD_2, "SyncWord2"},
                                             {SYNC_WORD_3, "SyncWord3"},
                                             {SYNC_WORD_4, "SyncWord4"},
                                             {SYNC_WORD_5, "SyncWord5"},
                                             {SYNC_WORD_6, "SyncWord6"},
                                             {SYNC_WORD_7, "SyncWord7"},
                                             {0x06CD, "NodeAddr"},
                                             {0x06CE, "BroadcastAddr"},
                                             {0x0740, "LoRaSyncWordMSB"},
                                             {0x0741, "LoRaSyncWordLSB"},
                                             {RAND_NUM_GEN_0, "RandNumGen0"},
                                             {RAND_NUM_GEN_1, "RandNumGen1"},
                                             {RAND_NUM_GEN_2, "RandNumGen2"},
                                             {RAND_NUM_GEN_3, "RandNumGen3"},
                                             {0x08AC, "RxGain"},
                                             {0x08E7, "OCPConfig"},
                                             {0x0911, "XTAtrim"},
                                             {0x0912, "XTBtrim"}};

bool sx1262_wait_on_busy(uint32_t time_out_ms) {
    uint32_t start_ms = 0U;
    uint32_t curr_ms = 0U;
    bool res = false;
    bool loop = true;
    uint32_t busy_value = 0;
    start_ms = get_time_ms32();
    while(loop) {
        busy_value = GPIO_readDio(SX1262_BUSY_DIO_NO);
        if(0 == busy_value) {
            res = true;
            loop = false;
            break;
        }
        curr_ms = get_time_ms32();
        if(time_out_ms < (curr_ms - start_ms)) {
            res = false;
            loop = false;
            break;
        }
    }
    return res;
}

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

    Sx1262Instance.chip_mode = CHIPMODE_RX;

    return res;
}

bool sx1262_set_base_addr(uint8_t tx_addr, uint8_t rx_addr) {
    bool res = false;
    uint8_t tx_array[2];
    tx_array[0] = TX_BASE_ADDRESS;
    tx_array[1] = RX_BASE_ADDRESS;
    res = sx1262_send_opcode(OPCODE_SET_BUFFER_BASE_ADDR, tx_array, 2, NULL, 0);
    return res;
}

bool sx1262_reset(void) {
    bool res = true;
    GPIO_writeDio(SX1262_RST_DIO_NO, 1);
    wait_ms(50);
    GPIO_writeDio(SX1262_RST_DIO_NO, 0);
    wait_ms(50);
    GPIO_writeDio(SX1262_RST_DIO_NO, 1);
    return res;
}

bool sx1262_init(void) {
    bool res = true;

    memset(&Sx1262Instance, 0x00, sizeof(Sx1262Instance));
    Sx1262Instance.tx_mode = CHIPMODE_UNDEF;
    res = sx1262_init_gpio() && res;
    res = sx1262_reset() && res;
    res = sx1262_set_rffrequency(868) && res;
    res = sx1262_set_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS) && res;
    Sx1262Instance.chip_mode = CHIPMODE_NONE;
    res = sx1262_start_rx(60) && res;
    return res;
}

bool sx1262_write_buffer(uint8_t offset, uint8_t* payload, uint8_t payload_len) {
    bool res;
    uint8_t buff[payload_len + 1];
    buff[0] = offset;
    memcpy(&buff[1], payload, payload_len);
    res = sx1262_send_opcode(OPCODE_WRITE_BUFFER, buff, payload_len + 1, NULL, 0);
    return res;
}

bool sx1262_set_payload(uint8_t* payload, uint8_t size) {
    bool res;
    res = sx1262_write_buffer(0x00, payload, size);
    return res;
}

bool sx1262_set_tx(uint32_t timeout_s) {
    bool res = true;
    uint8_t buff[3];
    /*from senior byte to junior byte*/
    buff[0] = MASK_8BIT & (timeout_s >> 16);
    buff[1] = MASK_8BIT & (timeout_s >> 8);
    buff[2] = MASK_8BIT & (timeout_s);
    res = sx1262_send_opcode(OPCODE_SET_TX, buff, 3, NULL, 0) && res;
    return res;
}

bool sx1262_start_tx(uint8_t offset, uint8_t* tx_buf, uint8_t tx_buf_len, uint32_t timeout_s) {
    bool res = true;
    if((NULL != tx_buf) && (0 < tx_buf_len)) {
        res = sx1262_write_buffer(offset, tx_buf, tx_buf_len) && res;
        res = sx1262_set_tx(timeout_s) && res;
        Sx1262Instance.chip_mode = CHIPMODE_TX;
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_sync_word(uint64_t* sync_word) {
    bool res = true;
    if(sync_word) {
        res = true;
        Type64Union_t var64bit;
        res = sx1262_read_reg(SYNC_WORD_0, &var64bit.u8[0]) && res;
        res = sx1262_read_reg(SYNC_WORD_1, &var64bit.u8[1]) && res;
        res = sx1262_read_reg(SYNC_WORD_2, &var64bit.u8[2]) && res;
        res = sx1262_read_reg(SYNC_WORD_3, &var64bit.u8[3]) && res;
        res = sx1262_read_reg(SYNC_WORD_4, &var64bit.u8[4]) && res;
        res = sx1262_read_reg(SYNC_WORD_5, &var64bit.u8[5]) && res;
        res = sx1262_read_reg(SYNC_WORD_6, &var64bit.u8[6]) && res;
        res = sx1262_read_reg(SYNC_WORD_7, &var64bit.u8[7]) && res;
        *sync_word = var64bit.u64;
    } else {
        res = false;
    }
    return res;
}

bool sx1262_get_rand(uint32_t* rand_num) {
    bool res = true;
    if(rand_num) {
        res = true;
        Type32Union_t var32bit;
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

bool sx1262_get_irq_status(uint16_t* out_irq_stat) {
    bool res = false;
    GET_4_BYTE_OPCODE(OPCODE_GET_IRQ_STATUS, out_irq_stat);
    return res;
}

bool sx1262_get_dev_err(uint16_t* op_error) {
    bool res = false;
    GET_4_BYTE_OPCODE(OPCODE_GET_DEVICE_ERRORS, op_error);
    return res;
}

bool sx1262_get_packet_type(RadioPacketTypes_t* packet_type) {
    bool res = false;
    uint8_t rx_array[3];
    memset(rx_array, 0x00, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_PACKET_TYPE, NULL, 0, rx_array, 3);
    if(res) {
        Sx1262Instance.status = rx_array[1];
        *packet_type = (RadioPacketTypes_t)rx_array[2];
    } else {
        *packet_type = PACKET_TYPE_NONE;
    }
    return res;
}

/*GetStatus*/
bool sx1262_get_status(uint8_t* out_status) {
    bool res = false;
    if(out_status) {
        uint8_t rx_array[2];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_STATUS, NULL, 0, rx_array, sizeof(rx_array));
        *out_status = rx_array[1];
    } else {
        *out_status = 0xFF;
    }
    return res;
}

bool sx1262_get_rxbuff_status(uint8_t* out_payload_length_rx,
                              uint8_t* out_rx_start_buffer_pointer) {
    bool res = false;
    uint8_t rx_array[4];
    memset(rx_array, 0xFF, sizeof(rx_array));
    res = sx1262_send_opcode(OPCODE_GET_RX_BUFFER_STATUS, NULL, 0, rx_array, sizeof(rx_array));
    if(res) {
        Sx1262Instance.status = rx_array[1];
        *out_payload_length_rx = rx_array[2];
        *out_rx_start_buffer_pointer = rx_array[3];
    }
    return res;
}

bool sx1262_get_packet_status(uint8_t* RxStatus, uint8_t* RssiSync, uint8_t* RssiAvg, uint8_t* RssiPkt, uint8_t* SnrPkt,
                              uint8_t* SignalRssiPkt) {
    bool res = false;
    if((NULL != RxStatus) && (NULL != RssiSync) && (NULL != RssiAvg) && (NULL != RssiPkt) && (NULL != SnrPkt) &&
       (NULL != SignalRssiPkt)) {

        uint8_t rx_array[10];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_PACKET_STATUS, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            *RxStatus = rx_array[2];
            *RssiSync = rx_array[3];
            *RssiAvg = rx_array[4];
            *RssiPkt = rx_array[7];
            *SnrPkt = rx_array[8];
            *SignalRssiPkt = rx_array[9];
        }
    }
    return res;
}

bool sx1262_get_rssi_inst(int8_t* rssi_inst) {
    bool res = false;
    if((NULL != rssi_inst)) {
        uint8_t rx_array[3];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_RSSIINST, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            *rssi_inst = -(rx_array[2] >> 1);
        }
    }
    return res;
}

bool sx1262_get_statistic(PaketStat_t* gfsk, PaketStat_t* lora) {
    bool res = false;
    if(gfsk && lora) {
        uint8_t rx_array[16];
        memset(rx_array, 0xFF, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_GET_STATS, NULL, 0, rx_array, sizeof(rx_array));
        if(res) {
            Sx1262Instance.status = rx_array[1];
            gfsk->nb_pkt_received = copy_and_rev16(&rx_array[2]);
            gfsk->nb_pkt_crc_error = copy_and_rev16(&rx_array[4]);
            gfsk->nb_pkt_length_error = copy_and_rev16(&rx_array[6]);

            lora->nb_pkt_received = copy_and_rev16(&rx_array[10]);
            lora->nb_pkt_crc_error = copy_and_rev16(&rx_array[12]);
            lora->nb_pkt_header_err = copy_and_rev16(&rx_array[14]);
        }
    }
    return res;
}

#define COM_STAT_COM_TX_DONE 0x6 /*Command TX done5 */

/* poll sx1262 registers. Move data from transceiver REG to MCU RAM.
 * verify transceiver and notify user if needed
 * */
bool sx1262_process(void) {
    bool res = false;
    uint8_t dev_status = 0;
    res = sx1262_get_status(&dev_status);
    if(res) {
        res = true;

        Sx1262Instance.com_stat = extract_subval_from_8bit(dev_status, 3, 1);
        switch(Sx1262Instance.com_stat) {
        case COM_STAT_DATA_AVAIL:
            LOG_INFO(LORA, "Data available");
            res = true;
            break;
        case COM_STAT_COM_TIMEOUT:
            LOG_WARNING(LORA, "time out");
            res = false;
            break;
        case COM_STAT_COM_PROC_ERR:
            /*Too frequent call*/
            // LOG_ERROR(LORA,"Error");
            res = false;
            break;
        case COM_STAT_EXE_ERR:
            LOG_ERROR(LORA, "fail to exe");
            res = false;
            break;
        case COM_STAT_COM_TX_DONE:
            LOG_INFO(LORA, "TX done");
            res = true;
            break;
        default:
            res = false;
            break;
        }
        Sx1262Instance.chip_mode = extract_subval_from_8bit(dev_status, 6, 4);
    }

    uint8_t payload_length_rx = 0;
    uint8_t rx_start_buffer_pointer = 0;
    res = sx1262_get_rxbuff_status(&payload_length_rx, &rx_start_buffer_pointer);
    if(res) {
        Sx1262Instance.rx_payload_length = payload_length_rx;
        Sx1262Instance.rx_start_buffer_pointer = rx_start_buffer_pointer;
    }

    uint8_t tempRxStatus = 0;
    uint8_t tempRssiSync = 0;
    uint8_t tempRssiAvg = 0;
    uint8_t tempRssiPkt = 0;
    uint8_t tempSnrPkt = 0;
    uint8_t tempSignalRssiPkt = 0;

    res = sx1262_get_packet_status(&tempRxStatus, &tempRssiSync, &tempRssiAvg, &tempRssiPkt, &tempSnrPkt,
                                   &tempSignalRssiPkt);
    if(res) {
        Sx1262Instance.rx_status = tempRxStatus;
        Sx1262Instance.rssi_sync = tempRssiSync;
        Sx1262Instance.rssi_avg = tempRssiAvg;
        Sx1262Instance.rssi_pkt = tempRssiPkt;
        Sx1262Instance.snr_pkt = tempSnrPkt;
        Sx1262Instance.signal_rssi_pkt = tempSignalRssiPkt;
    }

    int8_t temp_rssi_inst = 0;
    res = sx1262_get_rssi_inst(&temp_rssi_inst);
    if(res) {
        Sx1262Instance.rssi_inst = temp_rssi_inst;
    }

    PaketStat_t gfsk = {0};
    PaketStat_t lora = {0};
    res = sx1262_get_statistic(&gfsk, &lora);
    if(res) {
        Sx1262Instance.gfsk = gfsk;
        Sx1262Instance.lora = lora;
    }
    uint16_t op_error = 0;
    res = sx1262_get_dev_err(&op_error);
    if(res){
        Sx1262Instance.op_error = op_error;
    }
    uint16_t irq_stat;
    res=sx1262_get_irq_status(&irq_stat);
    if(res){
        Sx1262Instance.irq_stat = irq_stat;
    }
    return res;
}
