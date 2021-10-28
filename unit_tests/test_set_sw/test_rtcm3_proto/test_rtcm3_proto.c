#include "test_rtcm3_proto.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "rtcm3_protocol.h"
#ifndef X86_64
#include "uart_drv.h"
#endif
#include "unit_test_check.h"

const uint8_t rtcm3_message[36] = {0xD3, 0x00, 0x1E, 0x44, 0x60, 0x00, 0x1C, 0x77, 0xD7, 0x82, 0x00, 0x00,
                                   0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
                                   0x54, 0x50, 0x7C, 0x15, 0x9E, 0x12, 0x2C, 0xDA, 0x60, 0x6A, 0x26, 0x5D};

bool test_rtcm3_proto_1(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint8_t i = 0;
#ifndef X86_64
    EXPECT_TRUE(uart_deinit(1));
#endif
    EXPECT_TRUE(is_rtcm3_frame(rtcm3_message, sizeof(rtcm3_message)));
    rtcm3_reset_rx(&Rtcm3Porotocol[RT_UART_ID]);
    Rtcm3Porotocol[RT_UART_ID].rx_pkt_cnt = 0;
    for(i = 0; i < sizeof(rtcm3_message); i++) {
        rtcm3_proc_byte(&Rtcm3Porotocol[RT_UART_ID], rtcm3_message[i]);
    }
    EXPECT_EQ(0, Rtcm3Porotocol[RT_UART_ID].load_len);
    EXPECT_EQ(1, Rtcm3Porotocol[RT_UART_ID].rx_pkt_cnt);

    return true;
}

bool test_rtcm3_types(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    EXPECT_EQ(3, sizeof(Rtcm3Header_t));
    EXPECT_EQ(2, sizeof(Rtcm3Len_t));
#ifndef X86_64
    EXPECT_EQ(1, sizeof(Rtcm3ProtState_t));
#endif
    return true;
}
