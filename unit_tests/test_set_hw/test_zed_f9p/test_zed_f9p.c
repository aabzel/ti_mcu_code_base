#include "test_zed_f9p.h"

#include <string.h>

#include "clocks.h"
#include "none_blocking_pause.h"
#include "ubx_protocol.h"
#include "ublox_driver.h"
#include "task_info.h"
#include "unit_test_check.h"
#include "zed_f9p_drv.h"

bool test_zed_f9p_mon_hw(void) {
    task_data[TASK_ID_UBX].on = false;
    uint32_t init_cnt = UbloxProtocol.rx_pkt_cnt;

    EXPECT_TRUE(ubx_send_message(UBX_CLA_MON, UBX_ID_MON_HW, NULL, 0));
    EXPECT_TRUE( wait_in_loop_ms(S_2_MS(3)));
    EXPECT_GR(init_cnt,  UbloxProtocol.rx_pkt_cnt, 0);
    task_data[TASK_ID_UBX].on = true;
    return true;
}

bool test_zed_f9p_nav_clk(void) {
    task_data[TASK_ID_UBX].on = false;
    uint32_t init_cnt = UbloxProtocol.rx_pkt_cnt;

    EXPECT_TRUE(ubx_send_message(UBX_CLA_NAV, UBX_ID_NAV_CLK, NULL, 0));
    EXPECT_TRUE( wait_in_loop_ms(S_2_MS(3)));
    EXPECT_GR(init_cnt,  UbloxProtocol.rx_pkt_cnt, 0);
    task_data[TASK_ID_UBX].on = true;
    return true;
}

bool test_zed_f9p_nav_status(void) {
    task_data[TASK_ID_UBX].on = false;
    uint32_t init_cnt = UbloxProtocol.rx_pkt_cnt;

    EXPECT_TRUE(ubx_send_message(UBX_CLA_NAV, UBX_ID_NAV_STATUS, NULL, 0));
    EXPECT_TRUE( wait_in_loop_ms(S_2_MS(3)));
    EXPECT_GR(init_cnt,  UbloxProtocol.rx_pkt_cnt, 0);
    task_data[TASK_ID_UBX].on = true;
    return true;
}

bool test_zed_f9p_nav_posllh(void) {
    task_data[TASK_ID_UBX].on = false;
    uint32_t init_cnt = UbloxProtocol.rx_pkt_cnt;

    EXPECT_TRUE(ubx_send_message(UBX_CLA_NAV, UBX_ID_NAV_POSLLH, NULL, 0));
    EXPECT_TRUE( wait_in_loop_ms(S_2_MS(3)));
    EXPECT_GR(init_cnt,  UbloxProtocol.rx_pkt_cnt, 0);
    task_data[TASK_ID_UBX].on = true;
    return true;
}

bool test_zed_f9p_types(void){
    EXPECT_EQ(1, sizeof(RTKmode_t));
    EXPECT_EQ(1, sizeof(TimeSystem_t));
    return true;
}

//ubs 0x01 0x02

//ubs 0x27 0x03

