#include "sx1262_commands.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "spi_drv.h"
#include "str_utils.h"
#include "sx1262_diag.h"
#include "sx1262_drv.h"
#include "table_utils.h"
#include "writer_config.h"

bool sx1262_mute_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = try_str2bool(argv[0], &Sx1262Instance.tx_mute);
        if(false == res) {
            LOG_ERROR(LORA, "UnableToParse tx_mute %s", argv[0]);
        } else {
            LOG_INFO(LORA, "%u", Sx1262Instance.tx_mute);
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxu state");
    }
    return res;
}

bool sx1262_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 <= argc) {
        res = try_str2bool(argv[0], &Sx1262Instance.debug);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract debug %s", argv[0]);
        }
    }
    if(2 <= argc) {
        res = try_str2bool(argv[1], &Sx1262Instance.show_bin);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract show_bin %s", argv[1]);
        }
    }
    if(3 <= argc) {
        res = try_str2bool(argv[2], &Sx1262Instance.show_ascii);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract show_ascii %s", argv[2]);
        }
    }
    if(4 <= argc) {
        res = try_str2bool(argv[3], &Sx1262Instance.is_packet);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract is_packet %s", argv[3]);
        }
    }
    if((0 <= argc) && (argc <= 4)) {
        res = true;
#ifdef HAS_DEBUG
        float data_rate =
            lora_calc_data_rate(Sx1262Instance.mod_params.spreading_factor, Sx1262Instance.mod_params.band_width,
                                Sx1262Instance.mod_params.coding_rate);
        float t_preamble = 0.0f;
        float Tsym = 0.0;
        float t_frame = lora_calc_max_frame_tx_time(
            Sx1262Instance.mod_params.spreading_factor, Sx1262Instance.mod_params.band_width,
            Sx1262Instance.mod_params.coding_rate, Sx1262Instance.packet_param.proto.lora.preamble_length,
            Sx1262Instance.packet_param.proto.lora.header_type, Sx1262Instance.mod_params.low_data_rate_optimization,
            &Tsym, &t_preamble);
        LOG_INFO(LORA, "data rate %f bit/s %f byte/s", data_rate, data_rate / 8);
        LOG_INFO(LORA, "Tframe %f s", t_frame);
#endif
        // float rssi_watts=0.0;
        LOG_INFO(LORA, "ChipMode: [%u] %s", Sx1262Instance.dev_status.chip_mode,
                 chip_mode2str(Sx1262Instance.dev_status.chip_mode));
        // float rssi_inst = dbm2watts((int32_t) Sx1262Instance.rssi_inst);
        LOG_INFO(LORA, "RssiInst: %d dBm=%8.7f uW", Sx1262Instance.rssi_inst,
                 1000000.0 * dbm2watts((int32_t)Sx1262Instance.rssi_inst));
        LOG_INFO(LORA, "RssiPkt: %d=%8.7f uW", Sx1262Instance.rssi_pkt,
                 1000000.0 * dbm2watts((int32_t)Sx1262Instance.rssi_pkt));
        LOG_INFO(LORA, "SnrPkt: %u", Sx1262Instance.snr_pkt);
        LOG_INFO(LORA, "SignalRssiPkt: %d=%8.7f uW", Sx1262Instance.signal_rssi_pkt,
                 1000000.0 * dbm2watts((int32_t)Sx1262Instance.signal_rssi_pkt));
        // io_printf("RxStatus: %u" CRLF, Sx1262Instance.rx_status);
        // io_printf("RssiSync: %u" CRLF, Sx1262Instance.rssi_sync);
        // LOG_INFO(LORA,"RssiAvg: %d dBm=%f W", Sx1262Instance.rssi_avg, rssi_watts);
        LOG_INFO(LORA, "packet type: %s", pack_type2str(Sx1262Instance.packet_type));
        LOG_INFO(LORA, "rx_payload_len: %u byte", Sx1262Instance.rx_payload_len);
        LOG_INFO(LORA, "rx_buffer_pointer: %u 0x%x", Sx1262Instance.rx_buffer_pointer,
                 Sx1262Instance.rx_buffer_pointer);
        LOG_INFO(LORA, "DataAval cnt: %u", Sx1262Instance.data_aval_cnt);
        LOG_INFO(LORA, "DataRate %f bit/s %f byte/s", Sx1262Instance.bit_rate, Sx1262Instance.bit_rate / 8);
        io_printf("int cnt: %u" CRLF, Sx1262Instance.int_cnt);

        LOG_INFO(LORA, "busyCnt: %u", Sx1262Instance.busy_cnt);
        io_printf("debug: %u" CRLF, Sx1262Instance.debug);
        io_printf("showBin: %u" CRLF, Sx1262Instance.show_bin);
        io_printf("showAscii: %u" CRLF, Sx1262Instance.show_ascii);
        io_printf("isPacket: %u" CRLF, Sx1262Instance.is_packet);
        io_printf("status: %u" CRLF, Sx1262Instance.status);
        if(0 < Sx1262Instance.processing_error) {
            LOG_ERROR(LORA, "ProcErr: %u", Sx1262Instance.processing_error);
        }
        LOG_INFO(LORA, "cmdStat: [%u] %s", Sx1262Instance.dev_status.command_status,
                 cmd_stat2str(Sx1262Instance.dev_status.command_status));
        printf_pack_stat(&Sx1262Instance.lora, "LoRA");
        parse_op_error(Sx1262Instance.op_error);
        parse_irq_stat(Sx1262Instance.irq_stat);
        io_printf("read sync_word: 0x%" PRIx64 "" CRLF, Sx1262Instance.get_sync_word);
        io_printf("set sync_word: 0x%" PRIx64 "" CRLF, Sx1262Instance.set_sync_word);

        // buffer content
        io_printf("rand_num: 0x%" PRIx32 "" CRLF, Sx1262Instance.rand_num);

        io_printf("sx1262 %s selected" CRLF, (0 == Sx1262Instance.wire_nss) ? "" : "not");
        io_printf("sx1262 reset %s" CRLF, (0 == Sx1262Instance.wire_rst) ? "active" : "passive");
        io_printf("INT: %u" CRLF, Sx1262Instance.wire_int);
        LOG_DEBUG(LORA, "sx1262 %s" CRLF, (1 == Sx1262Instance.wire_busy) ? "busy" : "idle");
        //  res = print_int_diag(&Sx1262Instance.irq_cnt);
        // printf_pack_stat(&Sx1262Instance.gfsk, "GFSK");
    } else {
        res = false;
    }

    if(false == res) {
        LOG_ERROR(LORA, "Usage: sxd debug bin hex");
    }
    return res;
}

bool sx1262_sleep_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint8_t sleep_config = 0;
        res = try_str2uint8(argv[0], &sleep_config);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract offset %s", argv[0]);
        }
        if(res) {
            res = sx1262_set_sleep(sleep_config);
            if(res) {
                LOG_INFO(LORA, "set sleep OK");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxs");
    }
    return res;
}

bool sx1262_int_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        io_printf("IntCnt: %u" CRLF, Sx1262Instance.int_cnt);
        res = print_int_diag(&Sx1262Instance.irq_cnt);
        if(false == res) {
            LOG_INFO(LORA, "lack IRQ");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxid");
    }
    return res;
}

bool sx1262_clear_err_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = sx1262_clear_dev_error();
        if(res) {
            LOG_INFO(LORA, "OK");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxce");
    }
    return res;
}

bool sx1262_reset_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = sx1262_reset();
        if(res) {
            LOG_INFO(LORA, "reset OK");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxr");
    }
    return res;
}

bool sx1262_init_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
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
        LOG_ERROR(LORA, "Usage: sxrr");
    }
    return res;
}

bool sx1262_set_freq_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t rf_frequency_hz = 0;
    uint32_t freq_xtal_hz = XTAL_FREQ_HZ;
    if(1 <= argc) {
        res = true;
        res = try_str2uint32(argv[0], &rf_frequency_hz);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract rf_frequency_mhz %s", argv[0]);
        }
    }

    if(2 <= argc) {
        res = true;
        res = try_str2uint32(argv[1], &freq_xtal_hz);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract freq_xtal_hz %s", argv[1]);
        }
    }

    if(2 < argc) {
        LOG_ERROR(LORA, "Usage: sxsf rf_frequency_mhz freq_xtal_hz");
    }

    if(true == res) {
        res = sx1262_set_rf_frequency(rf_frequency_hz, freq_xtal_hz);
        if(res) {
            LOG_INFO(LORA, "set freq OK");
        }
    }

    return res;
}

bool sx1262_set_irq_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(4 == argc) {
        res = true;
        uint16_t irqMask = 0;
        uint16_t dio1Mask = 0;
        uint16_t dio2Mask = 0;
        uint16_t dio3Mask = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &irqMask);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract irqMask %s", argv[0]);
            }
        }
        if(true == res) {
            res = try_str2uint16(argv[1], &dio1Mask);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract irqMask %s", argv[1]);
            }
        }

        if(true == res) {
            res = try_str2uint16(argv[2], &dio2Mask);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract irqMask %s", argv[2]);
            }
        }

        if(true == res) {
            res = try_str2uint16(argv[3], &dio3Mask);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract irqMask %s", argv[3]);
            }
        }

        if(true == res) {
            res = sx1262_set_dio_irq_params(irqMask, dio1Mask, dio2Mask, dio3Mask);
            if(res) {
                LOG_INFO(LORA, "set IRQ OK");
            } else {
                LOG_ERROR(LORA, "set IRQ Error");
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxir dio1Mask dio2Mask dio3Mask");
        LOG_INFO(LORA, "dio1Mask");
        LOG_INFO(LORA, "dio2Mask");
        LOG_INFO(LORA, "dio3Mask");
    }
    return res;
}

bool sx1262_send_opcode_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;
        uint8_t op_code = 0;
        uint8_t tx_array[TX_SIZE] = {0};
        memset(tx_array, 0x00, sizeof(tx_array));
        uint32_t tx_array_len = 0;
        uint8_t rx_array[RX_SIZE] = {0};
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
                print_mem(rx_array, rx_array_len, true, true, true, true);
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
#ifdef HAS_SX1262_DEBUG
    uint8_t i = 0, cnt = 0, num = 0;
    uint8_t reg_val = 0xFF;
    const table_col_t cols[] = {{5, "num"}, {8, "addr"}, {6, "Val"}, {12, "Val"}, {23, "name"}};
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    char temp_str[120];
    char suffix_str[120];
    for(i = 0; i < SX1262_REG_CNT; i++) {
        res = sx1262_read_reg(RegMap[i].addr, &reg_val);
        if(res) {
            cnt++;
        }
        strcpy(temp_str, TSEP);
        snprintf(suffix_str, sizeof(suffix_str), "0x%04x " TSEP, RegMap[i].addr);
        strncat(temp_str, suffix_str, sizeof(temp_str));

        snprintf(suffix_str, sizeof(suffix_str), "0x%02x " TSEP, reg_val);
        strncat(temp_str, suffix_str, sizeof(temp_str));

        snprintf(suffix_str, sizeof(suffix_str), "0b%s " TSEP, utoa_bin8(reg_val));
        strncat(temp_str, suffix_str, sizeof(temp_str));

        snprintf(suffix_str, sizeof(suffix_str), "%21s " TSEP, RegMap[i].reg_name);
        strncat(temp_str, suffix_str, sizeof(temp_str));

        if(is_contain(temp_str, key_word1, key_word2)) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s", temp_str);
            num++;
        }
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    if(SX1262_REG_CNT == cnt) {
        res = true;
    }
#endif
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
    uint8_t tx_array[TX_SIZE] = {0};
    memset(tx_array, 0x00, sizeof(tx_array));
    uint32_t tx_array_len = 0;
    uint32_t timeout_s = 0;
    if(1 <= argc) {
        res = try_str2array(argv[0], tx_array, sizeof(tx_array), &tx_array_len);
        if(false == res) {
            /*treat as plane text*/
            LOG_NOTICE(LORA, "Unable to extract hex array %s", argv[0]);
            strncpy((char*)tx_array, argv[0], sizeof(tx_array));
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            res = true;
        }
    }

    if(2 <= argc) {
        res = true;
        res = try_str2uint32(argv[1], &timeout_s);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract timeout %s", argv[1]);
        }
    }

    if((3 < argc) || (0 == argc)) {
        LOG_ERROR(LORA, "Usage: sxt offset data_hex timeout_s");
        res = false;
    }
    if(true == res) {
        LOG_INFO(LORA, "LoRa tx");
        print_mem(tx_array, tx_array_len, true, true, true, true);
        res = sx1262_start_tx(tx_array, tx_array_len, timeout_s);
        if(res) {
            LOG_INFO(LORA, "TX OK");
        } else {
            LOG_ERROR(LORA, "tx error");
        }
    }
    return res;
}

/*
sxrx 10000
*/
bool sx1262_rx_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t timeout_s = 0;
    if(1 == argc) {
        res = true;
        res = try_str2uint32(argv[0], &timeout_s);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract timeout_s %s", argv[0]);
        }
    } else if(0 == argc) {
        res = true;
        timeout_s = 0;
    } else {
        LOG_ERROR(LORA, "Usage: sxr timeout_s");
        res = false;
    }

    if(res) {
        res = sx1262_start_rx(MASK_24BIT & timeout_s);
        if(true == res) {
            LOG_INFO(LORA, "RX OK");
        } else {
            LOG_ERROR(LORA, "rx error");
        }
    }
    return res;
}

bool sx1262_read_fifo_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t offset = 0;
    uint8_t rx_array[FIFO_SIZE] = {0};
    memset(rx_array, 0x00, sizeof(rx_array));
    uint16_t payload_len = FIFO_SIZE;
    if(0 == argc) {
        res = true;
        offset = 0;
        memset(rx_array, 0x00, sizeof(rx_array));
        payload_len = FIFO_SIZE;
    }

    if(1 <= argc) {
        res = true;
        res = try_str2uint8(argv[0], &offset);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract offset %s", argv[0]);
        }
    }

    if(2 <= argc) {
        res = true;
        res = try_str2uint16(argv[1], &payload_len);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract payload len %s", argv[1]);
        }
    }

    if(2 < argc) {
        LOG_ERROR(LORA, "Usage: sxrf offset len");
        LOG_INFO(LORA, "Usage: offset [0..255]");
        LOG_INFO(LORA, "Usage: len [0..256]");
    }

    if(res) {
        res = sx1262_read_buffer(offset, rx_array, payload_len);
        if(res) {
            print_mem(rx_array, payload_len, true, true, true, true);
            LOG_INFO(LORA, "read buff OK");
        } else {
            LOG_ERROR(LORA, "read buff Error");
        }
    }
    return res;
}

bool sx1262_clear_fifo_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = sx1262_clear_fifo();
        if(res) {
            LOG_INFO(LORA, "clear fifo OK");
        } else {
            LOG_ERROR(LORA, "Error");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxcf");
    }
    return res;
}

#ifdef HAS_SX1262_EX_DEBUG
bool sx1262_set_modulation_command(int32_t argc, char* argv[]) {
    bool res = false;
    ModulationParams_t modParams;
    modParams.band_width = LORA_BW_41;
    modParams.coding_rate = LORA_CR_4_5;
    modParams.spreading_factor = SF5;
    modParams.low_data_rate_optimization = 0;
    if(0 == argc) {
        res = true;
        modParams.band_width = LORA_BW_41;
        modParams.coding_rate = LORA_CR_4_5;
        modParams.spreading_factor = SF5;
    }

    if(0 < argc) {
        LOG_ERROR(LORA, "Usage: sxsmd band_width coding_rate spreading_factor");
    }
    if(res) {
        res = sx1262_set_modulation_params(&modParams);
        if(res) {
            LOG_INFO(LORA, "Set modulation OK");
        } else {
            LOG_ERROR(LORA, "Set modulation error");
        }
    }
    return res;
}
#endif

#ifdef HAS_SX1262_EX_DEBUG
bool sx1262_set_packet_param_command(int32_t argc, char* argv[]) {
    bool res = false;
    PacketParam_t packetParam;
    packetParam.packet_type = PACKET_TYPE_LORA;
    packetParam.proto.lora.preamble_length = 8;
    packetParam.proto.lora.header_type = LORA_VAR_LEN_PACT;
    packetParam.proto.lora.payload_length = 255;
    packetParam.proto.lora.crc_type = LORA_CRC_ON;
    packetParam.proto.lora.invert_iq = IQ_SETUP_STANDARD;
    if(0 == argc) {
        res = true;
        packetParam.packet_type = PACKET_TYPE_LORA;
        packetParam.proto.lora.preamble_length = 8;
        packetParam.proto.lora.header_type = LORA_VAR_LEN_PACT;
        packetParam.proto.lora.payload_length = 255;
        packetParam.proto.lora.crc_type = LORA_CRC_ON;
        packetParam.proto.lora.invert_iq = IQ_SETUP_STANDARD;
    }

    if(0 < argc) {
        LOG_ERROR(LORA, "Usage: sxpp preamble_length header_type payload_length crc_type invert_iq");
    }
    if(res) {
        res = sx1262_set_packet_params(&packetParam);
        if(res) {
            LOG_INFO(LORA, "Set packet param OK");
        } else {
            LOG_ERROR(LORA, "Set packet param error");
        }
    }
    return res;
}
#endif

bool sx1262_write_reg_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint16_t reg_addr = 0;
    uint8_t reg_val = 0;
    if(2 == argc) {
        res = true;
        if(res) {
            res = try_str2uint16(argv[0], &reg_addr);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract reg_addr %s", argv[0]);
            }
        }

        if(res) {
            res = try_str2uint8(argv[1], &reg_val);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract reg_val %s", argv[1]);
            }
        }

    } else {
        LOG_ERROR(LORA, "Usage: sxwr reg_addr reg_val");
    }

    if(res) {
        res = sx1262_write_reg(reg_addr, reg_val);
        if(res) {
            LOG_INFO(LORA, "write_reg OK");
        } else {
            LOG_ERROR(LORA, "write_regerror");
        }
    }
    return res;
}

bool sx1262_set_sync_word_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint64_t sync_word = 0;
    if(1 == argc) {
        res = true;
        if(res) {
            res = try_str2uint64(argv[0], &sync_word);
            if(false == res) {
                LOG_ERROR(LORA, "Unable to extract sync_word %s", argv[0]);
            }
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxssw sync_word64bit");
    }

    if(res) {
        res = sx1262_set_sync_word(sync_word);
        if(res) {
            LOG_INFO(LORA, "set syncWord OK");
        } else {
            LOG_ERROR(LORA, "set syncWord error");
        }
    }
    return res;
}

bool sx1262_read_rx_payload_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint8_t rx_payload[FIFO_SIZE] = {0};
        uint16_t rx_size = 0;
        uint8_t crc = 0;
        res = sx1262_get_rx_payload(rx_payload, &rx_size, sizeof(rx_payload), &crc);
        if(res) {
            LOG_INFO(LORA, "load %u byte", rx_size);
            print_mem(rx_payload, rx_size, true, true, true, true);
        } else {
            LOG_ERROR(LORA, "Error");
        }
    } else {
        LOG_ERROR(LORA, "Usage: sxrp");
    }
    return res;
}

static bool sx1262_statistic(void) {
    bool res = true;
    static uint32_t tx_done_cnt_prev = 0;
    static uint32_t rx_done_cnt_prev = 0;
    uint32_t tx_done_cnt_diff = Sx1262Instance.tx_done_cnt - tx_done_cnt_prev;
    uint32_t rx_done_cnt_diff = Sx1262Instance.rx_done_cnt - rx_done_cnt_prev;
    io_printf("tx done cnt: %u+%u" CRLF, Sx1262Instance.tx_done_cnt, tx_done_cnt_diff);
    io_printf("rx done cnt: %u+%u" CRLF, Sx1262Instance.rx_done_cnt, rx_done_cnt_diff);
    io_printf("rx size max: %u" CRLF, Sx1262Instance.rx_size_max);
    io_printf("tx size max: %u" CRLF, Sx1262Instance.tx_size_max);
    tx_done_cnt_prev = Sx1262Instance.tx_done_cnt;
    rx_done_cnt_prev = Sx1262Instance.rx_done_cnt;
    return res;
}

bool sx1262_test_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t wait_pause_ms = 5000, tx_array_len = 0;
    uint32_t try_cnt = 0;
    uint32_t try_num = 0;
    uint8_t tx_array[TX_SIZE] = {0};
    if(1 <= argc) {
        res = try_str2uint32(argv[0], &try_cnt);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract try_cnt %s", argv[0]);
        }
    }

    if(2 <= argc) {
        res = try_str2uint32(argv[1], &wait_pause_ms);
        if(false == res) {
            LOG_ERROR(LORA, "Unable to extract wait_pause_ms %s", argv[1]);
        }
    }

    if((2 < argc) || (0 == argc)) {
        LOG_ERROR(LORA, "Usage: sxs try_cnt pause_ms");
    }

    if(res) {
        Sx1262Instance.tx_done_cnt = 0;
        Sx1262Instance.rx_done_cnt = 0;
        for(try_num = 1; try_num <= try_cnt; try_num++) {
            snprintf((char*)tx_array, sizeof(tx_array), "ping_%u", try_num);
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            LOG_INFO(LORA, "send %u/%u [%s] pause %u ms", try_num, try_cnt, tx_array, wait_pause_ms);
            res = sx1262_start_tx(tx_array, tx_array_len, 0);
            if(res) {
                LOG_INFO(LORA, "tx OK");
            } else {
                LOG_ERROR(LORA, "tx error");
            }
            wait_in_loop_ms(wait_pause_ms);
        }
        sx1262_statistic();
    }
    return res;
}

bool sx1262_statistic_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 <= argc) {
        res = sx1262_statistic();
    } else {
        LOG_ERROR(LORA, "Usage: sxst");
    }
    return res;
}

#ifdef HAS_SX1262_EX_DEBUG
static bool sx1262_calc_diag(char* key_word1, char* key_word2) {
    bool res = false;
    uint8_t sf = 0, cr = 0, bw = 0;
    static const table_col_t cols[] = {{5, "No"},     {7, "SF,Ch/s"}, {8, "BW,kHz"},    {5, "CR"},       {9, "bit/s"},
                                       {9, "Byte/s"}, {9, "Tsym,ms"}, {9, "Tpream,ms"}, {9, "Tframe,ms"}};
    uint16_t num = 1;
    float data_rate = 0.0f, t_frame;
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    char temp_str[200];
    for(sf = SF5; sf <= SF12; sf++) {
        for(bw = 0; bw < 11; bw++) {
            for(cr = LORA_CR_4_5; cr <= LORA_CR_4_8; cr++) {
                res = is_valid_bandwidth((BandWidth_t)bw);
                if(res) {
                    data_rate = lora_calc_data_rate(sf, bw, cr);
                    float t_preamble = 0.0f;
                    float Tsym = 0.0;
                    t_frame = lora_calc_max_frame_tx_time(
                        sf, bw, cr, Sx1262Instance.packet_param.proto.lora.preamble_length,
                        Sx1262Instance.packet_param.proto.lora.header_type,
                        Sx1262Instance.mod_params.low_data_rate_optimization, &Tsym, &t_preamble);
                    strcpy(temp_str, TSEP);
                    snprintf(temp_str, sizeof(temp_str), "%s %5u " TSEP, temp_str, (uint32_t)powf(2.0f, (float)sf));
                    snprintf(temp_str, sizeof(temp_str), "%s %6.2f " TSEP, temp_str,
                             ((float)bandwidth2num((BandWidth_t)bw)) / 1000.0f);
                    snprintf(temp_str, sizeof(temp_str), "%s %3s " TSEP, temp_str,
                             coding_rate2str((LoRaCodingRate_t)cr));
                    snprintf(temp_str, sizeof(temp_str), "%s %7.1f " TSEP, temp_str, data_rate);
                    snprintf(temp_str, sizeof(temp_str), "%s %7.1f " TSEP, temp_str, data_rate / 8);
                    snprintf(temp_str, sizeof(temp_str), "%s %7.1f " TSEP, temp_str, Tsym * 1000.0f);
                    snprintf(temp_str, sizeof(temp_str), "%s %7.1f " TSEP, temp_str, t_preamble * 1000.0f);
                    snprintf(temp_str, sizeof(temp_str), "%s %7.1f " TSEP, temp_str, t_frame * 1000.0f);
                    snprintf(temp_str, sizeof(temp_str), "%s", temp_str);
                    if(is_contain(temp_str, key_word1, key_word2)) {
                        io_printf(TSEP " %3u ", num);
                        io_printf("%s" CRLF, temp_str);
                        num++;
                    }
                }
            }
        }
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}
#endif

#ifdef HAS_SX1262_EX_DEBUG
bool sx1262_calc_command(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 <= argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    }
    if(1 <= argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    }
    if(2 <= argc) {
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    }

    if(res) {
        res = sx1262_calc_diag(keyWord1, keyWord2);
    } else {
        LOG_ERROR(LORA, "Usage: sxc");
    }
    return res;
}
#endif /*HAS_SX1262_EX_DEBUG*/
