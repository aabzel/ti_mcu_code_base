#include "sx1262_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "bit_utils.h"
#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "gpio_drv.h"
#include "log.h"
#include "spi_drv.h"
#include "str_utils.h"
#include "sx1262_diag.h"
#include "sx1262_drv.h"
#include "table_utils.h"


bool sx1262_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        io_printf("poll chip mode: [%u] %s" CRLF, Sx1262Instance.chip_mode, chip_mode2str(Sx1262Instance.chip_mode));
        io_printf("poll cmd stat: [%u] %s" CRLF, Sx1262Instance.com_stat, cmd_stat2str(Sx1262Instance.com_stat));
        parse_op_error(Sx1262Instance.op_error);
        parse_irq_stat(Sx1262Instance.irq_stat);
        io_printf("status: %u" CRLF, Sx1262Instance.status);
        io_printf("rx len: %u byte" CRLF, Sx1262Instance.rx_payload_length);
        io_printf("rx start addr: %u" CRLF, Sx1262Instance.rx_start_buffer_pointer);
        io_printf("RxStatus: %u" CRLF, Sx1262Instance.rx_status);
        io_printf("RssiSync: %u" CRLF, Sx1262Instance.rssi_sync);
        io_printf("RssiAvg: %u" CRLF, Sx1262Instance.rssi_avg);
        io_printf("RssiPkt: %u" CRLF, Sx1262Instance.rssi_pkt);
        io_printf("SnrPkt: %u" CRLF, Sx1262Instance.snr_pkt);
        io_printf("SignalRssiPkt: %u" CRLF, Sx1262Instance.signal_rssi_pkt);
        io_printf("RssiInst: %d dBm" CRLF, Sx1262Instance.rssi_inst);

        printf_pack_stat(&Sx1262Instance.gfsk, "GFSK");
        printf_pack_stat(&Sx1262Instance.lora, "LoRA");

        uint16_t irq_stat = 0;
        res = sx1262_get_irq_status(&irq_stat);
        if(res) {
            parse_irq_stat(irq_stat);
        }

        // buffer content
        // syncword
        RadioPacketTypes_t packet_type = PACKET_TYPE_UNDEF;
        res = sx1262_get_packet_type(&packet_type);
        if(res) {
            io_printf("packet type: %u" CRLF, packet_type);
        }

        uint64_t sync_word = 0;
        res = sx1262_get_sync_word(&sync_word);
        io_printf("sync_word: 0x%" PRIx64 "" CRLF, sync_word);
        io_printf("busyCnt: %u" CRLF, Sx1262Instance.busy_cnt);

        uint32_t rand_num = 0;
        res = sx1262_get_rand(&rand_num);
        io_printf("rand_num: 0x%" PRIx32 "" CRLF, rand_num);

        uint32_t value = GPIO_readDio(SX1262_SS_DIO_NO);
        io_printf("sx1262 %s selected" CRLF, (0 == value) ? "" : "not");

        value = GPIO_readDio(SX1262_RST_DIO_NO);
        io_printf("sx1262 reset %s" CRLF, (0 == value) ? "active" : "passive");

        value = GPIO_readDio(SX1262_INT_DIO_NO);
        io_printf("INT: %u" CRLF, value);

        value = GPIO_readDio(SX1262_BUSY_DIO_NO);
        io_printf("sx1262 %s" CRLF, (1 == value) ? "busy" : "idle");
    } else {
        LOG_ERROR(LORA, "Usage: sxd");
    }
    return res;
}

bool sx1262_init_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = sx1262_init();
        if(res) {
            LOG_INFO(LORA, "OK");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxi");
    }
    return res;
}

bool sx1262_read_reg_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint16_t reg_addr = 0;
        uint8_t reg_val = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &reg_addr);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract reg addr %s", argv[0]);
            }
        }
        if(true == res) {
            res = sx1262_read_reg(reg_addr, &reg_val);
            if(res) {
                io_printf("val[0x%04x]=0x%02x 0b%s" CRLF, reg_addr, reg_val, utoa_bin8(reg_val));
                LOG_INFO(LORA, "OK");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxi");
    }
    return res;
}

bool sx1262_send_opcode_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint8_t op_code = 0;
        uint8_t tx_array[TX_SIZE];
        memset(tx_array, 0x00, sizeof(tx_array));
        uint16_t tx_array_len = 0;
        uint8_t rx_array[RX_SIZE];
        memset(rx_array, 0x00, sizeof(rx_array));
        uint16_t rx_array_len = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &op_code);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract op_code %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2array(argv[1], tx_array, sizeof(tx_array), &tx_array_len);
            if(false == res) {
                res = true;
                tx_array_len = 0;
                /*some opCode does not demand txdata*/
            }
            io_printf("Read %u bytes" CRLF, tx_array_len);
        }

        if(true == res) {
            res = try_str2uint16(argv[2], &rx_array_len);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract rx_len %s", argv[2]);
            }
        }

        if(true == res) {
            res = sx1262_send_opcode(op_code, tx_array, tx_array_len, rx_array, rx_array_len);
            if(res) {
                print_mem(rx_array, rx_array_len, true);
                LOG_INFO(LORA, "OK");
            } else {
                LOG_ERROR(LORA, "Error");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxo opCode [txData] rxLen");
        LOG_INFO(LORA, "opCode");
        LOG_INFO(LORA, "txData if no xxxx");
        LOG_INFO(LORA, "rxLen");
    }
    return res;
}

static bool sx1262_print_reg_map(char* key_word1, char* key_word2) {
    bool res = false;
    uint8_t i = 0, cnt = 0, num = 0;
    uint8_t reg_val = 0xFF;
    const table_col_t cols[] = {{5, "num"}, {8, "addr"}, {6, "Val"}, {12, "Val"}, {23, "name"}};
    table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
    char temp_str[120];
    for(i = 0; i < SX1262_REG_CNT; i++) {
        res = sx1262_read_reg(RegMap[i].addr, &reg_val);
        if(res) {
            cnt++;
        }
        strcpy(temp_str, TSEP);
        snprintf(temp_str, sizeof(temp_str), "%s 0x%04x " TSEP, temp_str, RegMap[i].addr);
        snprintf(temp_str, sizeof(temp_str), "%s 0x%02x " TSEP, temp_str, reg_val);
        snprintf(temp_str, sizeof(temp_str), "%s 0b%s " TSEP, temp_str, utoa_bin8(reg_val));
        snprintf(temp_str, sizeof(temp_str), "%s %21s " TSEP, temp_str, RegMap[i].reg_name);
        snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
        if(is_contain(temp_str, key_word1, key_word2)) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s", temp_str);
            num++;
        }
    }
    table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    if(SX1262_REG_CNT == cnt) {
        res = true;
    }
    return res;
}

bool sx1262_reg_map_command(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 == argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    } else if(1 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    } else if(2 == argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    } else {
        LOG_ERROR(LORA, "Usage: sxm keyWords");
    }
    if(res) {
        res = sx1262_print_reg_map(keyWord1, keyWord2);
    }
    return res;
}

bool sx1262_tx_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint8_t offset = 0;
        uint32_t timeout_s = 0;
        uint8_t tx_array[TX_SIZE];
        memset(tx_array, 0x00, sizeof(tx_array));
        uint16_t tx_array_len = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &offset);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract offset %s", argv[0]);
            }
        }
        if(true == res) {
            res = try_str2array(argv[1], tx_array, sizeof(tx_array), &tx_array_len);
            if(false == res) {
                res = true;
                tx_array_len = 0;
                /*some opCode does not demand txdata*/
            }
        }
        if(true == res) {
            res = try_str2uint32(argv[2], &timeout_s);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract offset %s", argv[2]);
            }
        }

        if(true == res) {
            res = sx1262_start_tx(offset, tx_array, tx_array_len, timeout_s);
            if(res) {
                LOG_INFO(LORA, "TX OK");
            } else {
                LOG_ERROR(LORA, "tx error");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxtx offset data_hex timeout_s");
        res = false;
    }
    return res;
}

bool sx1262_rx_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t timeout_s = 0;

        if(true == res) {
            res = try_str2uint32(argv[0], &timeout_s);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract offset %s", argv[0]);
            }
        }

        if(true == res) {
            res = sx1262_start_rx(timeout_s);
            if(true == res) {
                LOG_INFO(LORA, "RX OK");
            } else {
                LOG_ERROR(LORA, "rx error");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxrx timeout_s");
        res = false;
    }
    return res;
}

bool sx1262_read_fifo_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint8_t tx_array[1];
        uint8_t rx_array[RX_SIZE + 3];
        tx_array[0] = 0x00;
        res = sx1262_send_opcode(OPCODE_READ_BUFFER, tx_array, 1, rx_array, sizeof(rx_array));
        if(res) {
            print_mem(rx_array, sizeof(rx_array), true);
            LOG_INFO(LORA, "OK");
        } else {
            LOG_ERROR(LORA, "Error");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxf");
    }
    return res;
}

bool sx1262_clear_fifo_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint8_t tx_array[TX_SIZE + 1];
        memset(tx_array, 0x00, sizeof(tx_array));
        uint8_t rx_array[RX_SIZE + 1];
        memset(rx_array, 0x00, sizeof(rx_array));
        res = sx1262_send_opcode(OPCODE_WRITE_BUFFER, tx_array, sizeof(tx_array), rx_array, sizeof(rx_array));
        if(res) {
            print_mem(rx_array, sizeof(rx_array), true);
            LOG_INFO(LORA, "clear fifo OK");
        } else {
            LOG_ERROR(LORA, "Error");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxfc");
    }
    return res;
}
