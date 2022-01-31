#include "test_sx1262.h"

#include <string.h>

#include "sx1262_drv.h"
#include "sx1262_registers.h"
#include "unit_test_check.h"
#include "none_blocking_pause.h"
#include "sx1262_diag.h"

static bool test_one_sync_word(uint64_t sync_word) {
    uint64_t read_sync_word = 0;
    EXPECT_TRUE(sx1262_set_sync_word(sync_word));
    EXPECT_TRUE(sx1262_get_sync_word(&read_sync_word));
    EXPECT_EQ(sync_word, read_sync_word);
    return true;
}



static bool test_one_lora_sync_word(uint16_t sync_word) {
    uint16_t read_sync_word = 0;
    EXPECT_TRUE(sx1262_set_lora_sync_word(sync_word));
    EXPECT_TRUE(sx1262_get_lora_sync_word(&read_sync_word));
    EXPECT_EQ(sync_word, read_sync_word);
    return true;
}

bool test_sx1262_lora_sync_word(void){
    uint16_t orig_sync_word = 0;
    EXPECT_TRUE(sx1262_get_lora_sync_word(&orig_sync_word));
    EXPECT_EQ(0x1424, orig_sync_word);

    EXPECT_TRUE(test_one_lora_sync_word(0x0012));
    EXPECT_TRUE(test_one_lora_sync_word(0x1122));
    EXPECT_TRUE(test_one_lora_sync_word(0x9988));

    EXPECT_TRUE(sx1262_set_lora_sync_word(orig_sync_word));
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

bool test_sx1262_status(void){

    EXPECT_EQ(1, sizeof(Sx1262Status_t));

    uint8_t status1=0;
    uint8_t status2=0;
    RadioPacketType_t packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_get_packet_type(&packet_type, &status2));
    EXPECT_TRUE(sx1262_get_status(&status1));
    EXPECT_EQ(status1, status2);


    return true;
}

#if 0
static bool test_one_pack_type(RadioPacketType_t pack_type) {
    RadioPacketType_t read_packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_set_packet_type(pack_type));
    EXPECT_TRUE(sx1262_get_packet_type(&read_packet_type));
    EXPECT_EQ(pack_type, read_packet_type);
    return true;
}
#endif

/*fails*/
bool test_sx1262_packet_type(void) {
    EXPECT_EQ(1, sizeof(RadioPacketType_t));
    RadioPacketType_t orig_packet_type = PACKET_TYPE_UNDEF;
    uint8_t status=0;
    EXPECT_TRUE(sx1262_get_packet_type(&orig_packet_type,&status));
    EXPECT_EQ(PACKET_TYPE_LORA, orig_packet_type);
#if 0
    EXPECT_TRUE(test_one_pack_type(PACKET_TYPE_GFSK));
    EXPECT_TRUE(test_one_pack_type(PACKET_TYPE_LORA));

    EXPECT_TRUE(sx1262_set_sync_word(orig_packet_type));
    RadioPacketType_t packet_type = PACKET_TYPE_UNDEF;
    EXPECT_TRUE(sx1262_get_packet_type(&packet_type));
#endif
    return true;
}

static bool test_sx1262_fifo_one(uint8_t pattern) {
    uint8_t write_payload[FIFO_SIZE] = {0};
    uint8_t read_payload[FIFO_SIZE] = {0};
    memset(read_payload, 0xFF, sizeof(read_payload));
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

/*Test requre second device. Fails*/
bool test_sx1262_rx_addr(void) {
    uint8_t read_rx_addr = 0;

    uint8_t payload_length_rx = 0;
    //EXPECT_TRUE(sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, set_rx_addr));
    EXPECT_TRUE(sx1262_set_buffer_base_addr(TX_BASE_ADDRESS, RX_BASE_ADDRESS));
    //EXPECT_TRUE( wait_in_loop_ms(100));
    EXPECT_TRUE(sx1262_get_rxbuff_status(&payload_length_rx, &read_rx_addr));
    EXPECT_EQ(0,payload_length_rx);
    EXPECT_EQ(RX_BASE_ADDRESS, read_rx_addr); /*Error*/
    return true;
}

bool test_sx1262_rand(void) {
    uint32_t rand_num = 0;
    EXPECT_TRUE(sx1262_get_rand(&rand_num));
    EXPECT_NE(0, rand_num);
    EXPECT_NE(0xFFFFFFFF, rand_num);
    return true;
}

static bool test_sx1262_rx_gain_one(RxGain_t rx_gain) {
    RxGain_t read_rx_gain = RXGAIN_UNDEF;
    EXPECT_TRUE(sx1262_set_rx_gain(rx_gain));
    EXPECT_TRUE(sx1262_get_rx_gain(&read_rx_gain));
    EXPECT_EQ(rx_gain, read_rx_gain);
    return true;
}

bool test_sx1262_rx_gain(void){
    RxGain_t orig_rx_gain = RXGAIN_UNDEF;
    EXPECT_TRUE(sx1262_get_rx_gain(&orig_rx_gain));

    EXPECT_TRUE(test_sx1262_rx_gain_one(RXGAIN_POWER_SAVING));
    EXPECT_TRUE(test_sx1262_rx_gain_one(RXGAIN_BOOSTED));

    EXPECT_TRUE(sx1262_set_rx_gain(orig_rx_gain));
    return true;
}

static bool test_crc_poly(uint16_t crc_poly) {
    uint16_t read_crc_poly = 0;
    EXPECT_TRUE(sx1262_set_crc_poly(crc_poly));
    EXPECT_TRUE(sx1262_get_crc_poly(&read_crc_poly));
    EXPECT_EQ(crc_poly, read_crc_poly);
    return true;
}

bool test_sx1262_crc_poly(void){
    uint16_t orig_crc_poly = 0;
    EXPECT_TRUE(sx1262_get_crc_poly(&orig_crc_poly));

    EXPECT_TRUE(test_crc_poly(0xaa55));
    EXPECT_TRUE(test_crc_poly(0x55aa));
    EXPECT_TRUE(test_crc_poly(0x1234));

    EXPECT_TRUE(sx1262_set_crc_poly(orig_crc_poly));

    return true;
}

static bool test_crc_init(uint16_t crc_init) {
    uint16_t read_crc_init = 0;
    EXPECT_TRUE(sx1262_set_crc_seed(crc_init));
    EXPECT_TRUE(sx1262_get_crc_seed(&read_crc_init));
    EXPECT_EQ(crc_init, read_crc_init);
    return true;
}

bool test_sx1262_crc_init(void){
    uint16_t orig_crc_init = 0;
    EXPECT_TRUE(sx1262_get_crc_seed(&orig_crc_init));

    EXPECT_TRUE(test_crc_init(0xaa55));
    EXPECT_TRUE(test_crc_init(0x55aa));
    EXPECT_TRUE(test_crc_init(0x1234));

    EXPECT_TRUE(sx1262_set_crc_seed(orig_crc_init));

    return true;
}

bool test_sx1262_tx(void){
    uint8_t tx_buf[4]={0x55,0xaa,0x10,0x01};

    Sx1262Instance.tx_done= true;
    EXPECT_TRUE( sx1262_start_tx(tx_buf, sizeof(tx_buf), 0));
    EXPECT_TRUE(  wait_in_loop_ms(1000));
    EXPECT_TRUE( Sx1262Instance.tx_done);

    Sx1262Instance.tx_done= true;
    EXPECT_TRUE( sx1262_start_tx(tx_buf, sizeof(tx_buf), 0));
    EXPECT_TRUE(  wait_in_loop_ms(1000));
    EXPECT_TRUE( Sx1262Instance.tx_done);

    return true;
}

bool test_sx1262_chip_mode(void){
    Sx1262Status_t dev_status;
    EXPECT_TRUE( sx1262_set_fs());
    EXPECT_TRUE(  sx1262_get_status( &dev_status.byte));
    LOG_INFO(TEST,"ChipMode: %s", chip_mode2str(dev_status.chip_mode));
    EXPECT_EQ(CHP_MODE_FS, dev_status.chip_mode);
    RadioPacketType_t packet_type=PACKET_TYPE_UNDEF;
    dev_status.byte=0;
    EXPECT_TRUE( sx1262_get_packet_type(&packet_type, &dev_status.byte));
    EXPECT_EQ(CHP_MODE_FS, dev_status.chip_mode);
    EXPECT_TRUE( sx1262_init());
    return true;
}

bool test_sx1262_types(void) {
    EXPECT_EQ(1, sizeof(Sx1262Status_t));
    EXPECT_EQ(6, sizeof(LoRaPacketParam_t));
    EXPECT_EQ(2, sizeof(RadioIrqMasks_t));
    EXPECT_EQ(1, sizeof(BandWidth_t));
    EXPECT_EQ(1, sizeof(LoRaCodingRate_t));
    return true;
}

bool test_sx1262_connected(void) {
    EXPECT_TRUE(sx1262_is_exist());
    /* TODO: try undef op code command*/
    return true;
}

