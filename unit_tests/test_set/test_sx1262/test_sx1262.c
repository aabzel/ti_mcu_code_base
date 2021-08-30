#include "test_sx1262.h"

#include <string.h>

#include "sx1262_drv.h"
#include "unit_test_check.h"

static bool test_one_sync_word(uint64_t sync_word) {
    uint64_t read_sync_word = 0;
    EXPECT_TRUE(sx1262_set_sync_word(sync_word));
    EXPECT_TRUE(sx1262_get_sync_word(&read_sync_word));
    EXPECT_EQ(sync_word, read_sync_word);
    return true;
}

bool test_sx1262_sync_word(void) {
    uint64_t orig_sync_word = 0;
    EXPECT_TRUE(sx1262_get_sync_word(&orig_sync_word));

    EXPECT_TRUE(test_one_sync_word(0x0012345678abcdef));
    EXPECT_TRUE(test_one_sync_word(0x1122334455667788));
    EXPECT_TRUE(test_one_sync_word(0x9988776655443322));

    EXPECT_TRUE(sx1262_set_sync_word(orig_sync_word));
    return true;
}

static bool test_one_pack_type(RadioPacketType_t pack_type) {
    RadioPacketType_t read_packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_set_packet_type(pack_type));
    EXPECT_TRUE(sx1262_get_packet_type(&read_packet_type));
    EXPECT_EQ(pack_type, read_packet_type);
    return true;
}

bool test_sx1262_packet_type(void) {
    EXPECT_EQ(1, sizeof(RadioPacketType_t));
    RadioPacketType_t orig_packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_get_packet_type(&orig_packet_type));

    EXPECT_TRUE(test_one_pack_type(PACKET_TYPE_GFSK));
    EXPECT_TRUE(test_one_pack_type(PACKET_TYPE_LORA));

    EXPECT_TRUE(sx1262_set_sync_word(orig_packet_type));
    RadioPacketType_t packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_get_packet_type(&packet_type));
    EXPECT_EQ_VARS(PACKET_TYPE_GFSK, PACKET_TYPE_LORA, packet_type);
    return true;
}

static bool test_sx1262_fifo_one(uint8_t pattern) {
    uint8_t write_payload[FIFO_SIZE] = {0};
    uint8_t read_payload[FIFO_SIZE] = {0};
    memset(write_payload, pattern, sizeof(write_payload));

    EXPECT_TRUE(sx1262_write_buffer(0, write_payload, FIFO_SIZE));
    EXPECT_TRUE(sx1262_read_buffer(0, read_payload, FIFO_SIZE));

    EXPECT_EQ(0, memcmp(write_payload, read_payload, FIFO_SIZE));
    return true;
}

bool test_sx1262_fifo(void) {
    uint8_t orig_payload[FIFO_SIZE] = {0};
    EXPECT_TRUE(sx1262_read_buffer(0, orig_payload, FIFO_SIZE));

    EXPECT_TRUE(test_sx1262_fifo_one(0xAA));
    EXPECT_TRUE(test_sx1262_fifo_one(0x55));

    EXPECT_TRUE(sx1262_write_buffer(0, orig_payload, FIFO_SIZE));
    return true;
}

bool test_sx1262_rx_addr(void) {
    uint8_t read_rx_addr = 0;
    uint8_t set_rx_addr = 200;
    uint8_t PayloadLengthRx = 0;
    EXPECT_TRUE(sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, set_rx_addr));
    EXPECT_TRUE(sx1262_get_rxbuff_status(&PayloadLengthRx, &read_rx_addr));
    EXPECT_EQ(set_rx_addr, read_rx_addr);
    EXPECT_TRUE(sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS));
    return true;
}

bool test_sx1262(void) {

    EXPECT_EQ(2, sizeof(RadioIrqMasks_t));

    EXPECT_TRUE(sx1262_is_connected());

    uint32_t rand_num = 0;
    EXPECT_TRUE(sx1262_get_rand(&rand_num));
    EXPECT_NE(0, rand_num);
    EXPECT_NE(0xFFFFFFFF, rand_num);

    // try undef op code command

    return true;
}
