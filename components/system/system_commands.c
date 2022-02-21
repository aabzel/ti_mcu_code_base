#include "system_commands.h"

#include "log.h"
#include "tbfp_protocol.h"
#ifdef HAS_RTCM
#include "rtcm3_protocol.h"
#endif
#include "uart_common.h"
#include "io_utils.h"
#include "system.h"
#include "writer_config.h"
#include "table_utils.h"

bool sys_bypass_nmea_rs232_command(int32_t argc, char* argv[]){
    bool res = false;
    if (0 == argc) {
#ifdef HAS_HARVESTER_V1
        res = sys_bypass_nmea_rs232();
#endif
        if(res){
            LOG_INFO(SYS, "Ok!");
        }
    } else {
        LOG_ERROR(SYS, "Usage: nmrs");
    }
    return res;
}

bool sys_rate_command(int32_t argc, char* argv[]){
    bool res = true;
    static const table_col_t cols[] = {{8, "interf"},
                                       {7, "proto"},
                                       {9, "rxMin"},
                                       {9, "rx"},
                                       {9, "rxMax"},
                                       {9, "txMin"},
                                       {9, "tx"},
                                       {9, "txMax"}
    };

    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    uint8_t uart_num = 0;
    for(uart_num = 0; uart_num < UART_COUNT; uart_num++) {
        io_printf(TSEP);
        io_printf(" %6s " TSEP, huart[uart_num].name);
        io_printf(" ----- " TSEP);
        io_printf(" %7u " TSEP, huart[uart_num].rx_rate.min);
        io_printf(" %7u " TSEP, huart[uart_num].rx_rate.cur);
        io_printf(" %7u " TSEP, huart[uart_num].rx_rate.max);
        io_printf(" %7u " TSEP, huart[uart_num].tx_rate.min);
        io_printf(" %7u " TSEP, huart[uart_num].tx_rate.cur);
        io_printf(" %7u " TSEP, huart[uart_num].tx_rate.max);
        io_printf(CRLF);
    }
#ifdef HAS_RTCM
    uint8_t interface = 0;
    for(interface = 0; interface < ARRAY_SIZE(Rtcm3Protocol); interface++) {
        if(interface==Rtcm3Protocol[interface].interface){
            io_printf(TSEP);
            io_printf(" %6s " TSEP, interface2str((Interfaces_t)Rtcm3Protocol[interface].interface));
            io_printf(" RTCM3 " TSEP);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].rx_rate.min);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].rx_rate.cur);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].rx_rate.max);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].tx_rate.min);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].tx_rate.cur);
            io_printf(" %7u " TSEP, Rtcm3Protocol[interface].tx_rate.max);
            io_printf(CRLF);
            res = true;
        }
    }
#endif
#ifdef HAS_TBFP
    for(interface = (Interfaces_t)0; interface < ARRAY_SIZE(TbfpProtocol); interface++) {
        if(TbfpProtocol[interface].interface==interface){
            io_printf(TSEP);
            io_printf(" %6s " TSEP, interface2str(TbfpProtocol[interface].interface));
            io_printf("  TBFP " TSEP);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.min);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.cur);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].rx_rate.max);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.min);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.cur);
            io_printf(" %7u " TSEP, TbfpProtocol[interface].tx_rate.max);
            io_printf(CRLF);
            res = true;
        }
    }
#endif /*HAS_TBFP*/

    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    return res;
}
