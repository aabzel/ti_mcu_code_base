#include "test_firmware.h"

#include <string.h>

#include "sys_config.h"
#include "unit_test_check.h"
#include "tbfp_protocol.h"

bool test_interfaces(void) {
    uint8_t array[299];
    memset(array,0x55,sizeof(array));
#ifdef HAS_UART1
    EXPECT_TRUE(  tbfp_send(array, sizeof(array), IF_UART1, 0 ,ACK_NO_NEED));
#endif
#ifdef HAS_RS232
    EXPECT_TRUE(  tbfp_send(array, sizeof(array), IF_RS232, 0 ,ACK_NO_NEED));
#endif
#ifdef HAS_LORA
    EXPECT_TRUE(  tbfp_send(array, sizeof(array), IF_LORA, 0 ,ACK_NO_NEED));
#endif
#ifdef HAS_CAN
    EXPECT_TRUE(  tbfp_send(array, sizeof(array), IF_CAN, 0 ,ACK_NO_NEED));
#endif

    EXPECT_TRUE(  tbfp_send(array, sizeof(array), IF_LOOPBACK, 0 ,ACK_NO_NEED));

    return true;
}
